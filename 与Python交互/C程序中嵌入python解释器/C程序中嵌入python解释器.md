# C程序中嵌入python解释器

上一篇`C中调用Python模块`中,数据流固定是从C程序到Python程序再到C程序的.这种模式Python解释器并不能访问C应用程序本身的功能.更多的时候我们会希望Python解释器可以访问C应用程序,这样只要提供入口,Python就可以作为C程序的脚本直接控制C程序了,即数据流是Python脚本->Python模块->C应用.本篇要介绍的就是这种用法.

我们依然从一个helloworld开始

```C++
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <exception>
#include <filesystem>
#include "scope_guard.hpp"

// 模块的部分
// C中全局变量
int numargs = 0;

// 包装为模块
// python模块中获取numargs的值
static PyObject* emb_numargs(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}
// python模块中设置numargs的值
static PyObject* emb_setnumargs(PyObject* self, PyObject* args) {
    int i;
    auto ok = PyArg_ParseTuple(args, "i:numargs", &i);
    if (!ok) {
        return NULL;
    }
    numargs = i;
    Py_RETURN_NONE;
}
// 注册python模块中的方法
static PyMethodDef EmbMethods[] = {{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
                                   {"setnumargs", emb_setnumargs, METH_VARARGS, "Set the number of numargs received by the process."},
                                   {NULL, NULL, 0, NULL}};
// 构造python模块
static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};
// 初始化模块的函数
PyMODINIT_FUNC PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}

// 应用部分
class AppException : public std::runtime_error {
   public:
    AppException(const char* err) : std::runtime_error(err) {}
};

void init_py(char* programname, char* envpath, char* pymodulepath, bool debugmod) {
    // 参数校验变量赋值
    if (programname == NULL) {
        throw AppException("Fatal error: programname must set");
    }
    wchar_t* program;
    auto guard_program = sg::make_scope_guard([&program]() noexcept { PyMem_RawFree(program); });
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        throw AppException("Fatal error: cannot decode programname");
    }
    // 初始化python设置
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    auto guard_config = sg::make_scope_guard([&config]() noexcept {
        PyConfig_Clear(&config);
        printf("python init config clear\n");
    });

    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set program_name get error");
    }
    // 加载默认配置
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: PyConfig_Read get error");
    }
    // 设置python的sys.path用于查找模块
    wchar_t* pymodule_dir_name;
    auto guard_pymodule_dir_name = sg::make_scope_guard([&pymodule_dir_name]() noexcept { PyMem_RawFree(pymodule_dir_name); });
    std::filesystem::path pymodule_dir;
    if (pymodulepath == NULL) {
        pymodule_dir = std::filesystem::current_path();
    } else {
        pymodule_dir = pymodulepath;
        if (pymodule_dir.is_relative()) {
            pymodule_dir = std::filesystem::absolute(pymodule_dir);
        }
    }
    const char* _pymodule_dir_name = nullptr;
    {
        auto _pymodule_dir_name_str = pymodule_dir.string();
        _pymodule_dir_name = _pymodule_dir_name_str.c_str();
    }
    pymodule_dir_name = Py_DecodeLocale(_pymodule_dir_name, NULL);
    if (pymodule_dir_name == NULL) {
        throw AppException("Fatal error: cannot decode pymodule_dir_name");
    } else {
        if (debugmod) {
            printf("pymodule_dir %s \n", _pymodule_dir_name);
        }
    }
    config.module_search_paths_set = 1;
    status = PyWideStringList_Append(&config.module_search_paths, pymodule_dir_name);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set module_search_paths get error");
    }

    // 设置虚拟环境
    wchar_t* env_dir_name;
    auto guard_env_dir_name = sg::make_scope_guard([&env_dir_name, &envpath]() noexcept {
        if (envpath != NULL) {
            PyMem_RawFree(env_dir_name);
        }
    });
    if (envpath != NULL) {
        std::filesystem::path env_dir = envpath;
        if (env_dir.is_relative()) {
            env_dir = std::filesystem::absolute(env_dir);
        }
        const char* _env_dir_name = nullptr;
        {
            auto _env_dir_name_str = env_dir.string();
            _env_dir_name = _env_dir_name_str.c_str();
        }
        env_dir_name = Py_DecodeLocale(_env_dir_name, NULL);
        if (env_dir_name == NULL) {
            throw AppException("Fatal error: cannot decode _env_dir_name");
        } else {
            if (debugmod) {
                printf("use virtual environments %s \n", _env_dir_name);
            }
        }
        status = PyConfig_SetString(&config, &config.prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set prefix get error");
        }
        status = PyConfig_SetString(&config, &config.exec_prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set exec_prefix get error");
        }
    }
    // 提前初始化模块`emb`
    numargs = 10;
    PyImport_AppendInittab("emb", &PyInit_emb);

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        if (PyStatus_IsExit(status)) {
            // return status.exitcode;
            throw AppException("Fatal error: PyStatus_IsExit");
        }
        // 抛出错误
        Py_ExitStatusException(status);
    }
    if (debugmod) {
        PyRun_SimpleString("import sys;print(sys.path);print(sys.prefix)");
    }
}

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    printf("finalize_py ok\n");
    return 0;
}

int main(int argc, char* argv[]) {
    // 初始化python解释器
    try {
        init_py(argv[0], (char*)"env/", NULL, false);
        PyRun_SimpleString("import emb;print('Number of arguments', emb.numargs())");
        PyRun_SimpleString("emb.setnumargs(20);print('Number of arguments', emb.numargs())");
        printf("get numargs now is %d\n", numargs);
    } catch (const AppException& ex) {
        fprintf(stderr, "%s", ex.what());
        return 1;
    }
    return finalize_py();
}
```

核心思路就是

1. 将要让python脚本可以调度的C对象的对应操作包装成python模块.
2. 在初始化python解释器前先初始化该模块

## 用C构造Python模块

用C直接编写Python模块可以参考[官网的文档](https://docs.python.org/zh-cn/3/extending/extending.html#extracting-parameters-in-extension-functions),还是比较详尽的.这边仅用上面的例子给出一个简单的框架.

用C构造Python模块的基本结构是:

1. 构造模块中的函数

    ```C++
    ...
    // python模块中获取numargs的值
    static PyObject* emb_numargs(PyObject* self, PyObject* args) {
        if (!PyArg_ParseTuple(args, ":numargs"))
            return NULL;
        return PyLong_FromLong(numargs);
    }
    // python模块中设置numargs的值
    static PyObject* emb_setnumargs(PyObject* self, PyObject* args) {
        int i;
        auto ok = PyArg_ParseTuple(args, "i:numargs", &i);
        if (!ok) {
            return NULL;
        }
        numargs = i;
        Py_RETURN_NONE;
    }
    ...
    ```

    在Python的CAPI中实际只有方法没有函数,函数可以理解为是模块的方法.构造方法有如下几点注意点:
    + 通常都定义为静态函数,以避免多个模块中出现同名函数
    + 参数和返回值的类型都是`PyObject*`.
    + 参数中第一位`self`表示从属的对象,对于模块来说就是这个模块对象,对于对象的方法则是这个对象
    + 参数中第二位和第三位(如果有的话)表示函数接收的参数,我们需要在实现中解析他们
    + 返回值需要使用上篇文章中[数据类型转换相关的接口](https://blog.hszofficial.site/TutorialForCLang/#/%E4%B8%8EPython%E4%BA%A4%E4%BA%92/C%E4%B8%AD%E8%B0%83%E7%94%A8Python%E6%A8%A1%E5%9D%97/C%E4%B8%AD%E8%B0%83%E7%94%A8Python%E6%A8%A1%E5%9D%97?id=%e6%95%b0%e6%8d%ae%e7%b1%bb%e5%9e%8b%e8%bd%ac%e6%8d%a2)先转换为`PyObject*`后返回,如果没有返回或要返回python的`None`,需要使用`Py_RETURN_NONE;`**而不是**直接`return Py_None`

2. 构造模块方法的列表

    ```C++
    ...
    // 注册python模块中的方法
    static PyMethodDef EmbMethods[] = {{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
                                    {"setnumargs", emb_setnumargs, METH_VARARGS, "Set the number of numargs received by the process."},
                                    {NULL, NULL, 0, NULL}};
    ...
    ```

    构造模块方法的列表就是将上面构造的函数与在python模块中的名字,参数模式,以及文档字符串对应.[PyMethodDef](https://docs.python.org/zh-cn/3/c-api/structures.html?highlight=pymethoddef#c.PyMethodDef)是一个`array`,且固定最后一个元素为`{NULL, NULL, 0, NULL}`

3. 构造模块定义

    ```C++
    ...
    // 构造python模块
    static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};
    ...
    ```

    利用上面定义的方法列表就可以构造一个python的模块对象了.[PyModuleDef](https://docs.python.org/zh-cn/3/c-api/module.html?highlight=pymoduledef#c.PyModuleDef)是一个结构体,我们通常只要填写其中的前5位即可,其中第一位固定为`PyModuleDef_HEAD_INIT`;第二位为模块名;第三位为模块的文档字符串;第四位为模块的内存分配量,是为多子解释器的情况下内部数据安全而设置的,通常取`-1`表示模块具有全局状态且不支持子解释器;第五位则是上面定义的方法列表.

4. 构造模块的初始化函数

    ```C++
    ...
    // 初始化模块的函数
    PyMODINIT_FUNC PyInit_emb(void) {
        return PyModule_Create(&EmbModule);
    }
    ...
    ```

    这个函数通常命名为`PyInit_{模块名}`,用于在初始化python解释器前初始化模块使用.个人建议不要将它设置为静态.它干的事就是调用[PyModule_Create](https://docs.python.org/zh-cn/3/c-api/module.html?highlight=pymodule_create#c.PyModule_Create)并返回创建出的模块对象.我们习惯上用宏`PyMODINIT_FUNC`作为返回,它本质上核心就是`PyObject*`

### 参数解析

Python的CAPI支持两种类型的方法定义,他们的区别就在于参数解析

1. 仅位置参数的方法

    + 定义的函数签名为

        ```C++
        static PyObject* emb_setnumargs(PyObject* self, PyObject* args)
        ```

    + 使用[PyArg_ParseTuple](https://docs.python.org/zh-cn/3/c-api/arg.html?highlight=pyarg_parsetuple#c.PyArg_ParseTuple)

    + `PyModuleDef`中第三个参数为`METH_VARARGS`

2. 允许关键字参数的方法

    + 定义的函数签名为

        ```C++
        static PyObject* emb_setnumargs(PyObject* self, PyObject* args, PyObject* kwargs)
        ```

    + 使用[PyArg_ParseTupleAndKeywords](https://docs.python.org/zh-cn/3/c-api/arg.html?highlight=pyarg_parsetuple#c.PyArg_ParseTupleAndKeywords)

    + `PyModuleDef`中第三个参数为`METH_VARARGS | METH_KEYWORDS`

无论使用哪种方式,解析参数都是通过对应的接口将参数放入对应的变量地址中.而这个放的过程则通过一种[内部语法规则的格式化字符串](https://docs.python.org/zh-cn/3/c-api/arg.html?highlight=pyarg_parsetuple#parsing-arguments-and-building-values)来定义的.比如例子中我们希望参数转化为一个int类型的数据,就写成

```C++
int i;
auto ok = PyArg_ParseTuple(args, "i:numargs", &i);
if (!ok) {
    return NULL;
}
```

这个`"i:numargs"`就是格式化字符串.其中`:`是格式单元的列表结束标志冒号后的字符串`numargs`被用来作为错误消息中的函数名;而`i`表示仅有一个参数且第一个参数需要转化为int类型.后面的`&i`则表示解析出的值写入变量`i`的地址
比如我们的参数有3个int类型,则可以写成:

```C++
int i;
int j;
int k;
auto ok = PyArg_ParseTuple(args, "iii:numargs", &i,&j,&k);
if (!ok) {
    return NULL;
}
```

其他的常用类型还包括:

| `格式化字符串中的字符` | 对应python类型                              | 对应C类型                                                | 说明                                                                                                        |
| ---------------------- | ------------------------------------------- | -------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| `O`                    | `object`                                    | `PyObject *`                                             | 不做转换直接保存到变量地址,注意这个操作不会创建`强引用`,也就是说变量获得的是一个`借入引用`.                 |
| `O!`                   | `object`                                    | `typeobject`和`PyObject *`                               | 和`O`用途一致,只是它接受两个C变量,第一个指定python类型,第二个指定python对象.                                |
| `S`                    | `bytes`                                     | `PyBytesObject *`                                        | 类似`O`,只是对应的python类型固定为`bytes`                                                                   |
| `Y`                    | `bytearray`                                 | `PyByteArrayObject *`                                    | 类似`O`,只是对应的python类型固定为`bytearray`                                                               |
| `U`                    | `str`                                       | `PyObject *`                                             | 类似`O`,只是对应的python类型固定为`str`                                                                     |
| `p`                    | `bool`                                      | `int`                                                    | 真值为1,假值为0                                                                                             |
| `b`                    | `int`                                       | `unsigned char`                                          | 非负的`Python`整数转换为无符号的微整数                                                                      |
| `B`                    | `int`                                       | `unsigned char`                                          | 不进行溢出检查                                                                                              |
| `h`                    | `int`                                       | `short int`                                              | ---                                                                                                         |
| `H`                    | `int`                                       | `unsigned short int`                                     | 不进行溢出检查                                                                                              |
| `i`                    | `int`                                       | `int`                                                    | ---                                                                                                         |
| `I`                    | `int`                                       | `unsigned int`                                           | 不进行溢出检查                                                                                              |
| `l`                    | `int`                                       | `long`                                                   | ---                                                                                                         |
| `k`                    | `int`                                       | `unsigned long`                                          | 不进行溢出检查                                                                                              |
| `L`                    | `int`                                       | `long long`                                              | ---                                                                                                         |
| `K`                    | `int`                                       | `unsigned long long`                                     | 而不进行溢出检查                                                                                            |
| `n`                    | `int`                                       | `Py_ssize_t`                                             | ---                                                                                                         |
| `c`                    | `bytes`或长度为1的`bytearray`               | `char`                                                   | ---                                                                                                         |
| `C`                    | 长度为1的`str`                              | `int`                                                    | ---                                                                                                         |
| `f`                    | `float`                                     | `float`                                                  | ---                                                                                                         |
| `d`                    | `float`                                     | `double`                                                 | ---                                                                                                         |
| `D`                    | `complex`                                   | `Py_complex`                                             | ---                                                                                                         |
| `s`                    | `str`                                       | `const char *`                                           | 使用utf-8编码                                                                                               |
| `y`                    | `read-only bytes-like object`               | `const char *`                                           | `s`的字节流版本,注意不可包含嵌入的空字节                                                                    |
| `y#`                   | `read-only bytes-like object`               | `const char *`, `Py_ssize_t`                             | 和`y`功能一致,只是它可以接收字节流,且它接受两个C变量,第一个是utf-8编码格式的结果C字符串,第二个为C字符串长度 |
| `s#`                   | `str`或`read-only bytes-like object`        | `const char *`和`Py_ssize_t`                             | 和`y#`用途类似,只是它可以接收`str`                                                                          |
| `z`                    | `str`或`None`                               | `const char *`                                           | 类似`s`,只是它也可以接收`None`,如果接收了`None`那么结果会被置为`NULL`                                       |
| `z#`                   | `str`,`read-only bytes-like object`或`None` | `const char *, Py_ssize_t`,类似`s#`,只是带对`None`的处理 |

除了上面的简单元素外,格式化字符串还支持使用`()`包裹元素对应符号的形式指代tuple类型

比如python中的`(1,0.5,"a")`可以用格式化字符串表示为`(idU)`,比如下面的例子,我们希望构造的函数是类似下面这样的签名

```python
def func(a:int,b:tuple[int,float,str]):
    pass
```

那么C实现的函数的定义和解析可以写成:

```C++
static PyObject* emb_setnumargs(PyObject* self, PyObject* args) {
    int a;
    int b_0;
    double b_1;
    char * b_2;
    auto ok = PyArg_ParseTuple(args, "i(idU):numargs", &a,&b_0,&b_1,&b_2);
    if (!ok) {
        return NULL;
    }
    numargs = i;
    Py_RETURN_NONE;
}
```

#### 关键字参数的解析

上面的例子我们一直使用的是位置参数.如果是关键字参数我们怎么解析呢?假如我们的函数希望接口形式为

```python
def func(a:int,b:float):
    pass
```

且允许其使用关键字参数调用,那么定义可以写成:

```C++
// 定义带关键字参数的函数
static PyObject* efunc(PyObject* self, PyObject* args, PyObject* kwargs) {
    // 定义函数的参数
    static const char* keywords[] = {"a", "b", NULL};

    int a;
    double b;

    // 使用 PyArg_ParseTupleAndKeywords 解析参数
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "id:numargs", (char**)keywords, &a, &b)) {
        return NULL;  // 解析失败，返回错误
    }
    ...
}
```

位置解析是没有变化的,不同之处在于我们还需要将每个参数所在位置的对应关键子名传给解析函数`PyArg_ParseTupleAndKeywords`.这个传入的对应关系是一个元素为`char*`类型的array,以元素`NULL`结束.如果定义中有仅位置参数,这个对应位置填`""`:

```python
def func(a:int,/,b:float,*,c:int):
    # a为仅位置参数,c为仅关键字参数
    pass
```

且允许其使用关键字参数调用,那么定义可以写成:

```C++
// 定义带关键字参数的函数
static PyObject* example_function(PyObject* self, PyObject* args, PyObject* kwargs) {
    // 定义函数的参数
    static const char* keywords[] = {"", "b", "c", NULL};
    int a;
    double b;
    int c;
    // 使用 PyArg_ParseTupleAndKeywords 解析参数
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "id:numargs", (char**)keywords, &a, &b,&c)) {
        return NULL;  // 解析失败，返回错误
    }
    ...
}
```

#### [`*`]使用格式化字符串构造Python对象

格式化字符串除了可以将Python对象解析为C的值外也可以反过来用于从C的值转换为Python对象.使用的接口为[Py_BuildValue](https://docs.python.org/zh-cn/3/c-api/arg.html?highlight=pyarg_parsetuple#c.Py_BuildValue),返回的是一个强引用.这个方法不太直观个人不太推荐使用,但熟悉了就会觉得很方便.

```C++
...
int x = 10;
PyObject * o = Py_BuildValue("i", x)
...
```

比如我们要构造位置参数用于调用,可以写成:

```C++
int arg1 = 10;
int arg2 = 10;
PyObject * arglist = Py_BuildValue("(ii)", arg1,arg2);
PyObject * result = PyObject_CallObject(my_call, arglist);
```

除了结构`()`对应python的`tuple`类型外.在构造Python对象时还可以使用`[]`对应python的`list`以及`{}`对应python的`dict`.`dict`使用连续两个位置表示键值对,比如

```python
{
    "a":1,
    "b":1.5
}
```

对应的格式化字符串即为

```C
"{UiUd}""
```

### [*]编译构造python扩展模块

虽然一来本系列是介绍C/C++的的文章不是介绍python的文章;二来这块相对简单,讲的人也多;三来用Cython写这种扩展更加简单,这个技术用的真不算多的,就没啥在这里讲的必要.但毕竟已经写到这里了不讲似乎又缺点啥,这边就简单用一个例子作为额外的非重点内容介绍下吧.

我们的这个例子是上面`helloworld`中的代码拆出来构造的,在[emb](https://github.com/hsz1273327/TutorialForCLang/tree/master/%E4%B8%8EPython%E4%BA%A4%E4%BA%92/C%E7%A8%8B%E5%BA%8F%E4%B8%AD%E5%B5%8C%E5%85%A5python%E8%A7%A3%E9%87%8A%E5%99%A8/emb)中

我们把定义模块的部分都抽出来,剩下的就是写好`setup.py`并执行`build`操作了

+ `setup.py`,在`setup.py`中要用`ext_modules`参数指定扩展的编译信息

    ```python
    setup(
        ext_modules=[
            Extension(
                name="emb",  
                sources=["emb/emb.cpp"],
                # include_dirs=[
                #     "/Users/mac/micromamba/envs/py3.10/include/python3.10"],
                # library_dirs=["/Users/mac/micromamba/envs/py3.10/lib"],
                # libraries=["python3.10"],
                extra_compile_args=["-std=c++20"],
                language="c++"
            ),
        ]
    )
    ```

    注意不需要指定python相关的include和lib配置,编译时会使用运行`setup.py`的python使用的环境.

+ build操作,本机就是简单的`python -m build --wheel`,当然了我们也可以借助`github action`和`cibuildwheel`做多平台build.

## 提前加载模型

有构造模块的初始化函数,我们只要在初始化Python解释器前调用`PyImport_AppendInittab(模块名, &初始化函数);`即可.

```C++
...
// 先注册为内置模块
PyImport_AppendInittab("emb", &PyInit_emb);
// 再初始化python解释器
status = Py_InitializeFromConfig(&config);
...
```

[`int PyImport_AppendInittab(const char *name, PyObject *(*initfunc)(void))`](https://docs.python.org/zh-cn/3/c-api/import.html?highlight=pyimport_appendinittab#c.PyImport_AppendInittab)接口用于将一个模块注册为内置模块.这样就可以直接`import`使用而不需要再去查找了.

这边建议在初始化好python解释器后第一时间先`import`我们初始化的模型

```C++
PyMODINIT_FUNC init_pymodule(char* Module_Name) {
    auto pName = PyUnicode_DecodeFSDefault(Module_Name);  // 将模块名类型转为python对象字符串
    auto guard = sg::make_scope_guard([&pName]() noexcept {
        Py_DECREF(pName);  // 释放对象pName的gc计数器
    });
    auto pModule = PyImport_Import(pName);  // 导入模块
    return pModule;
}

...

auto pModule = init_pymodule((char *)"emb");
auto guard_pModule = sg::make_scope_guard([&pModule]() noexcept {
    Py_XDECREF(pModule);  // 释放pModule
});
...
```

这样下次代码中再出现`import`这个模型的操作时就不用额外加载了

### 隔离配置

很多时候我们希望自己仅仅是嵌入了一个python解析器,并不希望受到系统环境变量的影响,那就可以使用隔离配置来初始化python解释器.具体来说就是用[PyConfig_InitIsolatedConfig](https://docs.python.org/zh-cn/3/c-api/init_config.html?highlight=pythonioencoding#c.PyConfig_InitIsolatedConfig)替代`PyConfig_InitPythonConfig`初始化python解释器的配置.
这样设置后会有如下几个变化:

+ 全局配置变量,环境变量,命令行参数()`PyConfig.argv`将不会被解析和用户目录下的`.python`目录等都不会生效
+ C标准流和`LC_CTYPE`语言区域保持不变
+ 信号处理句柄将不会被安装
+ `PYTHON_HOME`将不会默认加载,需要手动使用`PyConfig.home`指定

同时对于预配置项来说也会有变化:

| 字段               | 用途                                       | 一般配置 | 隔离配置 |
| ------------------ | ------------------------------------------ | -------- | -------- |
| `configure_locale` | 将`LC_CTYPE`语言区域设为用户选择的语言区域 | `1`      | `0`      |
将 LC_CTYPE 语言区域设为用户选择的语言区域。
`coerce_c_locale`|如果等于`2`,强制转换C语言区域;如果等于`1`,则读取`LC_CTYPE`语言区域来确定其是否应当被强制转换|`-1`|`0`
`coerce_c_locale_warn`|如为非零值则会在C语言区域被强制转换时发出警告|`-1`|`0`
`dev_mode`|即`PyConfig.dev_mode`,是否开启开发模式,非0为启用|`-1`|`0`
`isolated`|即`PyConfig.isolated`,是否启用隔离配置|`0`|`1`
`parse_argv`|非0值则解析`Args`|`1`|`0`
`use_environment`|是否使用环境变量|`1`|`0`
`utf8_mode`|是否使用utf-8模式|`-1`|`0`

由于无论如何我们需要兼容非英文的路径字符串,我们就需要比之前的例子中额外多出一步预配置的处理:

```C++
//预初始化
PyStatus statusp;
PyPreConfig preconfig;
PyPreConfig_InitIsolatedConfig(&preconfig);
preconfig.utf8_mode = 1;
statusp = Py_PreInitialize(&preconfig);
if (PyStatus_Exception(statusp)) {
    Py_ExitStatusException(statusp);
}
```

这样之后我们就可以在隔离配置条件下启动python解释器了.

至此,这边给出一个相对比较通用的python初始化函数:

```C++

/**
 * @fn BINARY_VECTOR_P VEC_init(float,float)
 * @brief 相对通用的初始化python解释器函数
 * @param[programname]  应用名.
 * @param[envpath]  虚拟环境路径.为NULL则不使用虚拟环境;为相对路径则从应用所在文件夹开始查找
 * @param[pymodulepath]  python模块的额外查找路径.为NULL则将当前应用所在文件夹加入;为相对路径则从应用所在文件夹开始查找
 * @param[pyhomepath]  设置python_home.为相对路径则从应用所在文件夹开始查找.如果`isolated`为true则不能为NULL
 * @param[tabs] 需要预先加载的模块名模块初始化函数信息
 * @param[isolated]  是否使用隔离配置初始化python解释器
 * @param[debugmod]  是否打印debug用的文本
 * @return void
 * @exception <AppException> { 应用级别异常 }
 */
void init_py(char* programname,
             char* envpath,
             char* pymodulepath,
             char* pyhomepath,
             const std::map<std::string, PyObject* (*)(void)>* tabs,
             bool isolated,
             bool debugmod) {
    // 参数校验变量赋值
    if (programname == NULL) {
        throw AppException("Fatal error: programname must set");
    }
    wchar_t* program;
    auto guard_program = sg::make_scope_guard([&program]() noexcept { PyMem_RawFree(program); });
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        throw AppException("Fatal error: cannot decode programname");
    }

    // 初始化
    if (isolated) {
        // 预设置
        PyStatus statusp;
        PyPreConfig preconfig;
        PyPreConfig_InitIsolatedConfig(&preconfig);
        preconfig.utf8_mode = 1;
        statusp = Py_PreInitialize(&preconfig);
        if (PyStatus_Exception(statusp)) {
            Py_ExitStatusException(statusp);
        }
    }
    // 基本设置
    //  隔离模式
    PyStatus status;
    PyConfig config;
    if (isolated) {
        PyConfig_InitIsolatedConfig(&config);
    } else {
        PyConfig_InitPythonConfig(&config);
    }
    auto guard_config = sg::make_scope_guard([&config]() noexcept {
        PyConfig_Clear(&config);
        printf("python init config clear\n");
    });
    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set program_name get error");
    }

    // 设置python_home
    wchar_t* pyhome;
    auto guard_pyhome = sg::make_scope_guard([&pyhome, &pyhomepath]() noexcept {
        if (pyhomepath != NULL) {
            PyMem_RawFree(pyhome);
        }
    });
    if (pyhomepath != NULL) {
        std::filesystem::path pyhome_dir = pyhomepath;
        if (pyhome_dir.is_relative()) {
            pyhome_dir = std::filesystem::absolute(pyhome_dir);
        }
        const char* _pyhome_dir_name = nullptr;
        {
            auto _pyhome_dir_name_str = pyhome_dir.string();
            _pyhome_dir_name = _pyhome_dir_name_str.c_str();
        }

        pyhome = Py_DecodeLocale(_pyhome_dir_name, NULL);
        if (pyhome == NULL) {
            throw AppException("Fatal error: cannot decode pyhome");
        } else {
            if (debugmod) {
                printf("use python_home %s \n", pyhome);
            }
        }
        status = PyConfig_SetString(&config, &config.home, pyhome);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set home get error");
        }
    } else {
        if (isolated) {
            throw AppException("Fatal error: isolated config must set pyhomepath");
        }
    }

    // 加载默认配置
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: PyConfig_Read get error");
    }

    // 设置python的sys.path用于查找模块
    wchar_t* pymodule_dir_name;
    auto guard_pymodule_dir_name = sg::make_scope_guard([&pymodule_dir_name]() noexcept { PyMem_RawFree(pymodule_dir_name); });
    std::filesystem::path pymodule_dir;
    if (pymodulepath == NULL) {
        pymodule_dir = std::filesystem::current_path();
    } else {
        pymodule_dir = pymodulepath;
        if (pymodule_dir.is_relative()) {
            pymodule_dir = std::filesystem::absolute(pymodule_dir);
        }
    }
    const char* _pymodule_dir_name = nullptr;
    {
        auto _pymodule_dir_name_str = pymodule_dir.string();
        _pymodule_dir_name = _pymodule_dir_name_str.c_str();
    }

    pymodule_dir_name = Py_DecodeLocale(_pymodule_dir_name, NULL);
    if (pymodule_dir_name == NULL) {
        throw AppException("Fatal error: cannot decode pymodule_dir_name");
    } else {
        if (debugmod) {
            printf("pymodule_dir %s \n", _pymodule_dir_name);
        }
    }
    config.module_search_paths_set = 1;
    status = PyWideStringList_Append(&config.module_search_paths, pymodule_dir_name);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set module_search_paths get error");
    }
    // 设置虚拟环境
    wchar_t* env_dir_name;
    auto guard_env_dir_name = sg::make_scope_guard([&env_dir_name, &envpath]() noexcept {
        if (envpath != NULL) {
            PyMem_RawFree(env_dir_name);
        }
    });
    if (envpath != NULL) {
        std::filesystem::path env_dir = envpath;
        if (env_dir.is_relative()) {
            env_dir = std::filesystem::absolute(env_dir);
        }
        const char* _env_dir_name = nullptr;
        {
            auto _env_dir_name_str = env_dir.string();
            _env_dir_name = _env_dir_name_str.c_str();
        }

        env_dir_name = Py_DecodeLocale(_env_dir_name, NULL);
        if (env_dir_name == NULL) {
            throw AppException("Fatal error: cannot decode _env_dir_name");
        } else {
            if (debugmod) {
                printf("use virtual environments %s \n", _env_dir_name);
            }
        }
        status = PyConfig_SetString(&config, &config.prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set prefix get error");
        }
        status = PyConfig_SetString(&config, &config.exec_prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set exec_prefix get error");
        }
    }

    // 提前初始化模块`emb`
    if (tabs != NULL) {
        for (const auto& [key, value] : *tabs) {
            PyImport_AppendInittab(key.c_str(), value);
        }
    }

    // 初始化python解释器
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        if (PyStatus_IsExit(status)) {
            // return status.exitcode;
            throw AppException("Fatal error: PyStatus_IsExit");
        }
        // 抛出错误
        Py_ExitStatusException(status);
    }
    if (debugmod) {
        PyRun_SimpleString("import sys;print(sys.path);print(sys.prefix)");
    }
}
```

隔离环境的本意是让应用专门绑定一个非系统环境的Python实现,这样就实现了和系统环境的隔离.因此`python home`的设置就是必须的.

## 更进一步的例子

我们做个粗糙的例子来演示C程序中嵌入Python解释器具体是怎么用的,具体又是怎么写的代码

这个例子时上面helloworld的一个服务化实现,将启动一个http服务,它会在调用`/api`接口后返回其中维护的一个变量的取值;调用`/submit`可以传入一个python脚本,而这个脚本可以使用模块`emb`中的方法来改变.

这个项目的结构如下:

```txt
helloworldserv\
              |---main.cpp # 程序主体
              |---crow_all.h # http服务框架库
              |---scope_guard.hpp # scope guard实现库
              |---testscript.py #上传给submit接口的python脚本
              |---testquery.py  #调用submit接口上传脚本的python代码

和上一篇文章中[外部线程和服务化](https://blog.hszofficial.site/TutorialForCLang/#/%E4%B8%8EPython%E4%BA%A4%E4%BA%92/C%E4%B8%AD%E8%B0%83%E7%94%A8Python%E6%A8%A1%E5%9D%97/C%E4%B8%AD%E8%B0%83%E7%94%A8Python%E6%A8%A1%E5%9D%97?id=%e5%a4%96%e9%83%a8%e7%ba%bf%e7%a8%8b%e5%92%8c%e6%9c%8d%e5%8a%a1%e5%8c%96)的例子一样,我们实现代码逻辑使用的[PyRun_SimpleString](https://docs.python.org/zh-cn/3/c-api/veryhigh.html?highlight=pyrun_simplestring#c.PyRun_SimpleString)接口也需要使用GIL限制并发避免出错.

```C++
// 开始执行python调用
// PyGILState_STATE gstate;
auto _save = PyEval_SaveThread();
auto gstate = PyGILState_Ensure();
auto guard_gstate = sg::make_scope_guard([&gstate, &_save]() noexcept {
    PyGILState_Release(gstate);
    PyEval_RestoreThread(_save);
    CROW_LOG_INFO << "PyGILState_Release ok";
});
CROW_LOG_INFO << "PyGILState_Ensure ok";
// 指定传进来的代码
int res = PyRun_SimpleString(code);
if (res == 0){
    CROW_LOG_INFO << "PyRun_SimpleString ok";
    crow::json::wvalue x({{"status", "ok"}});
    x["result"] = numargs;
    return crow::response(x);
}else{
    // 处理异常
    CROW_LOG_ERROR << "Python code get error";
    return crow::response(crow::status::INTERNAL_SERVER_ERROR);
}
```

我们一样使用`sg::make_scope_guard`来维护GIL的释放.不过要注意`PyRun_SimpleString`这个接口并不能捕获报错的信息,我们就只能笼统的告诉提交方脚本有错误.

最终我们的代码如下:

```C++
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <exception>
#include <filesystem>
#include <map>
#include <string>
#include "crow_all.h"
#include "scope_guard.hpp"

// 模块的部分
// C中全局变量
int numargs = 0;

// 包装为模块
// python模块中获取numargs的值
static PyObject* emb_numargs(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}
// python模块中设置numargs的值
static PyObject* emb_setnumargs(PyObject* self, PyObject* args) {
    int i;
    auto ok = PyArg_ParseTuple(args, "i:numargs", &i);
    if (!ok) {
        return NULL;
    }
    numargs = i;
    Py_RETURN_NONE;
}
// 注册python模块中的方法
static PyMethodDef EmbMethods[] = {{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
                                   {"setnumargs", emb_setnumargs, METH_VARARGS, "Set the number of numargs received by the process."},
                                   {NULL, NULL, 0, NULL}};
// 构造python模块
static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};
// 初始化模块的函数
PyMODINIT_FUNC PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}

// 应用部分
class AppException : public std::runtime_error {
   public:
    AppException(const char* err) : std::runtime_error(err) {}
};

/**
 * @fn BINARY_VECTOR_P VEC_init(float,float)
 * @brief 相对通用的初始化python解释器函数
 * @param[programname]  应用名.
 * @param[envpath]  虚拟环境路径.为NULL则不使用虚拟环境;为相对路径则从应用所在文件夹开始查找
 * @param[pymodulepath]  python模块的额外查找路径.为NULL则将当前应用所在文件夹加入;为相对路径则从应用所在文件夹开始查找
 * @param[pyhomepath]  设置python_home.为相对路径则从应用所在文件夹开始查找.如果`isolated`为true则不能为NULL
 * @param[tabs] 需要预先加载的模块名模块初始化函数信息
 * @param[isolated]  是否使用隔离配置初始化python解释器
 * @param[debugmod]  是否打印debug用的文本
 * @return void
 * @exception <AppException> { 应用级别异常 }
 */
void init_py(char* programname,
             char* envpath,
             char* pymodulepath,
             char* pyhomepath,
             const std::map<std::string, PyObject* (*)(void)>* tabs,
             bool isolated,
             bool debugmod) {
    // 参数校验变量赋值
    if (programname == NULL) {
        throw AppException("Fatal error: programname must set");
    }
    wchar_t* program;
    auto guard_program = sg::make_scope_guard([&program]() noexcept { PyMem_RawFree(program); });
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        throw AppException("Fatal error: cannot decode programname");
    }

    // 初始化
    if (isolated) {
        // 预设置
        PyStatus statusp;
        PyPreConfig preconfig;
        PyPreConfig_InitIsolatedConfig(&preconfig);
        preconfig.utf8_mode = 1;
        statusp = Py_PreInitialize(&preconfig);
        if (PyStatus_Exception(statusp)) {
            Py_ExitStatusException(statusp);
        }
    }
    // 基本设置
    //  隔离模式
    PyStatus status;
    PyConfig config;
    if (isolated) {
        PyConfig_InitIsolatedConfig(&config);
    } else {
        PyConfig_InitPythonConfig(&config);
    }
    auto guard_config = sg::make_scope_guard([&config]() noexcept {
        PyConfig_Clear(&config);
        printf("python init config clear\n");
    });
    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set program_name get error");
    }

    // 设置python_home
    wchar_t* pyhome;
    auto guard_pyhome = sg::make_scope_guard([&pyhome, &pyhomepath]() noexcept {
        if (pyhomepath != NULL) {
            PyMem_RawFree(pyhome);
        }
    });
    if (pyhomepath != NULL) {
        std::filesystem::path pyhome_dir = pyhomepath;
        if (pyhome_dir.is_relative()) {
            pyhome_dir = std::filesystem::absolute(pyhome_dir);
        }
        const char* _pyhome_dir_name = nullptr;
        {
            auto _pyhome_dir_name_str = pyhome_dir.string();
            _pyhome_dir_name = _pyhome_dir_name_str.c_str();
        }

        pyhome = Py_DecodeLocale(_pyhome_dir_name, NULL);
        if (pyhome == NULL) {
            throw AppException("Fatal error: cannot decode pyhome");
        } else {
            if (debugmod) {
                printf("use python_home %s \n", pyhome);
            }
        }
        status = PyConfig_SetString(&config, &config.home, pyhome);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set home get error");
        }
    } else {
        if (isolated) {
            throw AppException("Fatal error: isolated config must set pyhomepath");
        }
    }

    // 加载默认配置
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: PyConfig_Read get error");
    }

    // 设置python的sys.path用于查找模块
    wchar_t* pymodule_dir_name;
    auto guard_pymodule_dir_name = sg::make_scope_guard([&pymodule_dir_name]() noexcept { PyMem_RawFree(pymodule_dir_name); });
    std::filesystem::path pymodule_dir;
    if (pymodulepath == NULL) {
        pymodule_dir = std::filesystem::current_path();
    } else {
        pymodule_dir = pymodulepath;
        if (pymodule_dir.is_relative()) {
            pymodule_dir = std::filesystem::absolute(pymodule_dir);
        }
    }
    const char* _pymodule_dir_name = nullptr;
    {
        auto _pymodule_dir_name_str = pymodule_dir.string();
        _pymodule_dir_name = _pymodule_dir_name_str.c_str();
    }

    pymodule_dir_name = Py_DecodeLocale(_pymodule_dir_name, NULL);
    if (pymodule_dir_name == NULL) {
        throw AppException("Fatal error: cannot decode pymodule_dir_name");
    } else {
        if (debugmod) {
            printf("pymodule_dir %s \n", _pymodule_dir_name);
        }
    }
    config.module_search_paths_set = 1;
    status = PyWideStringList_Append(&config.module_search_paths, pymodule_dir_name);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set module_search_paths get error");
    }
    // 设置虚拟环境
    wchar_t* env_dir_name;
    auto guard_env_dir_name = sg::make_scope_guard([&env_dir_name, &envpath]() noexcept {
        if (envpath != NULL) {
            PyMem_RawFree(env_dir_name);
        }
    });
    if (envpath != NULL) {
        std::filesystem::path env_dir = envpath;
        if (env_dir.is_relative()) {
            env_dir = std::filesystem::absolute(env_dir);
        }
        const char* _env_dir_name = nullptr;
        {
            auto _env_dir_name_str = env_dir.string();
            _env_dir_name = _env_dir_name_str.c_str();
        }

        env_dir_name = Py_DecodeLocale(_env_dir_name, NULL);
        if (env_dir_name == NULL) {
            throw AppException("Fatal error: cannot decode _env_dir_name");
        } else {
            if (debugmod) {
                printf("use virtual environments %s \n", _env_dir_name);
            }
        }
        status = PyConfig_SetString(&config, &config.prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set prefix get error");
        }
        status = PyConfig_SetString(&config, &config.exec_prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set exec_prefix get error");
        }
    }

    // 提前初始化模块`emb`
    if (tabs != NULL) {
        for (const auto& [key, value] : *tabs) {
            PyImport_AppendInittab(key.c_str(), value);
        }
    }

    // 初始化python解释器
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        if (PyStatus_IsExit(status)) {
            // return status.exitcode;
            throw AppException("Fatal error: PyStatus_IsExit");
        }
        // 抛出错误
        Py_ExitStatusException(status);
    }
    if (debugmod) {
        PyRun_SimpleString("import sys;print(sys.path);print(sys.prefix)");
    }
}

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    printf("finalize_py ok\n");
    return 0;
}

PyObject* init_pymodule(char* Module_Name) {
    auto pName = PyUnicode_DecodeFSDefault(Module_Name);  // 将模块名类型转为python对象字符串
    auto guard = sg::make_scope_guard([&pName]() noexcept {
        Py_DECREF(pName);  // 释放对象pName的gc计数器
    });
    auto pModule = PyImport_Import(pName);  // 导入模块
    return pModule;
}

int main(int argc, char* argv[]) {
    // 初始化python解释器
    numargs = 10;
    std::map<std::string, PyObject* (*)(void)> tabs{{"emb", PyInit_emb}};
    try {
        // init_py(argv[0], (char*)"env", NULL, (char*)"/Users/mac/micromamba/envs/py3.10", NULL, true, true);
        init_py(argv[0], NULL, NULL, (char*)"/Users/mac/micromamba/envs/py3.10", &tabs, true, true);
    } catch (const AppException& ex) {
        fprintf(stderr, ex.what());
        return 1;
    }

    auto pModule = init_pymodule((char *)"emb");
    auto guard_pModule = sg::make_scope_guard([&pModule]() noexcept {
        Py_XDECREF(pModule);  // 释放pModule
    });

    // http接口逻辑
    crow::SimpleApp app;
    CROW_ROUTE(app, "/api")
    ([](const crow::request& req) {
        // 结束python调用,转换结果
        crow::json::wvalue x({{"status", "ok"}});
        x["result"] = numargs;
        return crow::response(x);
    });

    CROW_ROUTE(app, "/submit").methods("POST"_method)([](const crow::request& req) {
        crow::multipart::message msg(req);
        std::string code_str = msg.get_part_by_name("script").body;
        if (code_str.empty()){
            return crow::response(crow::status::BAD_REQUEST);  // same as crow::response(400)
        }
        auto code = code_str.c_str();
        // 开始执行python调用
        // // PyGILState_STATE gstate;
        auto _save = PyEval_SaveThread();
        auto gstate = PyGILState_Ensure();
        auto guard_gstate = sg::make_scope_guard([&gstate, &_save]() noexcept {
            PyGILState_Release(gstate);
            PyEval_RestoreThread(_save);
            CROW_LOG_INFO << "PyGILState_Release ok";
        });
        CROW_LOG_INFO << "PyGILState_Ensure ok";
        /* Perform Python actions here. */
        CROW_LOG_INFO << std::format("submit code {}", code);
        int res = PyRun_SimpleString(code);
        if (res == 0){
            CROW_LOG_INFO << "PyRun_SimpleString ok";
            crow::json::wvalue x({{"status", "ok"}});
            x["result"] = numargs;
            return crow::response(x);
        }else{
            CROW_LOG_ERROR << "Python code get error";
            return crow::response(crow::status::INTERNAL_SERVER_ERROR);
        }
    });
    app.loglevel(crow::LogLevel::Info).port(18080).multithreaded().run();
    return finalize_py();
}
```
