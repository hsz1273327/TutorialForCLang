# C中调用Python模块

我们知道Python本质上是一个C写的虚拟机,它本身就提供了与C交互的接口.我们先来一个[helloworld例子](https://github.com/hsz1273327/TutorialForCLang/tree/master/%E4%B8%8EPython%E4%BA%A4%E4%BA%92/C%E4%B8%AD%E8%B0%83%E7%94%A8Python%E6%A8%A1%E5%9D%97/helloworld)展示如何在C中执行python代码.

+ `helloworld/main.c`

    ```C
    #define PY_SSIZE_T_CLEAN
    #include <Python.h>


    int main(int argc, char* argv[]) {
        wchar_t* program = Py_DecodeLocale(argv[0], NULL);
        if (program == NULL) {
            fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
            exit(1);
            }
        // 初始化python环境
        PyStatus status;
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        // 设置python程序名
        status = PyConfig_SetString(&config, &config.program_name, program);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: InitPythonConfig set program_name get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        // 使用设置初始化python解释器
        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: Py_InitializeFromConfig get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        PyConfig_Clear(&config);

        PyRun_SimpleString("from time import time,ctime\n"
            "print('Today is', ctime(time()))\n");
        if (Py_FinalizeEx() < 0) {
            exit(120);
            }
        PyMem_RawFree(program);
        return 0;
    }
    ```

+ 编译,需要注意一定要设置你环境中的`Python.h`查找路径和对应的`libpython3.10`

    ```bash
    gcc -I /Users/mac/micromamba/envs/py3.10/include/python3.10 -L/Users/mac/micromamba/envs/py3.10/lib -lpython3.10 -o helloworld main.c
    ```

+ 执行

    ```txt
    >>> ./helloworld
    Today is Tue Dec  5 10:39:59 2023
    ```

这个例子中我们可以看到在C中调用Python的基本框架

+ [`wchar_t *program = Py_DecodeLocale(argv[0], NULL)`](https://docs.python.org/zh-cn/3/c-api/sys.html?highlight=py_decodelocale#c.Py_DecodeLocale)用于将可执行文件的文件名作为python程序的应用名进行类型转化(`char*` -> `WCHAR*`).整个`Python.h`中但凡用到字符串的都是使用的类型`WCHAR*`,`Python.h`中也提供了`Py_DecodeLocale`,`PyUnicode_FromWideChar`,`PyUnicode_AsWideCharString`等专门用于将`char *`转成`WCHAR *`的[工具](https://docs.python.org/zh-cn/3/c-api/unicode.html#wchar-t-support).整个程序结束后调用[`PyMem_RawFree(program)`](https://docs.python.org/zh-cn/3/c-api/memory.html#c.PyMem_RawFree)回收释放这个应用名对应的内存.

+ 通过下面的代码段为程序的python进行初始化.

    ```C
    // 初始化python环境
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        fprintf(stderr, "Fatal error: InitPythonConfig set program_name get error\n");
        PyConfig_Clear(&config);
        exit(1);
    }
    // 使用设置初始化python解释器
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        fprintf(stderr, "Fatal error: Py_InitializeFromConfig get error\n");
        PyConfig_Clear(&config);
        exit(1);
    }
    PyConfig_Clear(&config);
    ```

    [PyConfig_InitPythonConfig](https://docs.python.org/zh-cn/3/c-api/init_config.html?highlight=pyconfig_initpythonconfig#c.PyConfig_InitPythonConfig)用于初始化一个python设置,之后通过[PyConfig_SetString](https://docs.python.org/zh-cn/3/c-api/init_config.html?highlight=pyconfig_initpythonconfig#c.PyConfig_SetString)我们将命令行命令设置为这个python应用的名字.最后执行[`Py_InitializeFromConfig`](https://docs.python.org/zh-cn/3/c-api/init_config.html?highlight=pyconfig_initpythonconfig#c.Py_InitializeFromConfig)使用配置文件初始化Python解释器.需要注意,C中调用python对象一定要回收--无论配置设置是否成功都需要调用[PyConfig_Clear](https://docs.python.org/zh-cn/3/c-api/init_config.html?highlight=pyconfig_initpythonconfig#c.PyConfig_Clear)回收配置对象;同时我们的python代码执行完成后需要用[`if (Py_FinalizeEx() < 0) {exit(120);}`](https://docs.python.org/zh-cn/3/c-api/init.html?highlight=py_initialize#c.Py_FinalizeEx)关闭回收解释器;

+ `PyRun_SimpleString("from time import time,ctime;print('Today is', ctime(time()))\n");`则是实际执行的python代码.[PyRun_SimpleString](https://docs.python.org/zh-cn/3/c-api/veryhigh.html?highlight=pyrun_simplestring#c.PyRun_SimpleString)算是Python CAPI的一个高级接口,它的作用是将传入的字符串导入python解释器中执行.类似的还有[`PyRun_SimpleFile`](https://docs.python.org/zh-cn/3/c-api/veryhigh.html?highlight=pyrun_simplestring#c.PyRun_SimpleFile)用于从文件中读取脚本执行.注意,在Windows上，`fp`应当以二进制模式打开(即`fopen(filename, "rb")`);在其他平台则使用`fopen(filename,"r")`.

## 调用模块

为了加载python模块,我们需要先确保可以加载

1. 先有个python模块.我们使用下面这个`hello.py`作为例子

    ```python
    def apply():
        print("hello world!")
    ```

2. 让我们的模块可以被C程序中的python解释器找到.
    python解释器使用`sys.path`查找模块.如果我们什么都不设置,python解释器默认会将你的编译时指定的python动态库/静态库所在的环境中的的库位置放进`sys.path`,即`xxx`,`xxx/site-packages`,和`xxx/lib-dynload`.除此之外还会额外加个`~/.local/lib/xxx/site-packages`.这可以至少保证你可以用到标准库.

    但如果你需要将特定路径加入`sys.path`则需要
    1. 默认设置不要丢

        ```C
        ...
        status = PyConfig_Read(&config);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: PyConfig_Read get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        ...
        ```

    2. 允许设置`sys.path`

        ```C
        ...
        config.module_search_paths_set = 1;
        ...
        ```

    3. 在`sys.path`中添加路径

        ```C
        // 添加python的sys.path项目用于查找模块
        status = PyWideStringList_Append(&config.module_search_paths,
            current_dir_name);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: InitPythonConfig set module_search_paths get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        ```

        为了比较方便的处理路径问题,我们使用了C++17的`std::filesystem::path`,因此在编译时需要用`g++`并且加上编译标识`-std=c++17`

在可以加载之后我们就要调用这个模块中的函数接口了.

+ 导入模型

    ```C
    PyObject* pName;
    PyObject * pModule;

    pName = PyUnicode_DecodeFSDefault(Module_Name); //将模块名类型转为python对象字符串
    /* Error checking of pName left out */
    pModule = PyImport_Import(pName); //导入模块
    Py_DECREF(pName); //用完对象pName后释放对象pName的gc引用计数

    if (pModule != NULL) {
            ... //使用模块pModule
    } else {
        PyErr_Print();// 捕获错误,并打印
        fprintf(stderr, "Failed to load Module\"%s\"\n", Module_Name);
        return 1;
    }
    Py_XDECREF(pModule); //回收模块对象的gc引用
    ...
    ```

+ 在模块中查找方法并调用

```C
...
PyObject * pFunc;
PyObject* pValue;
...
pFunc = PyObject_GetAttrString(pModule, Func_Name); //在模块中找到函数名为func_name的函数,将这个函数对象提出来
if (pFunc && PyCallable_Check(pFunc)) {
    // pFunc存在且为可调用对象,则执行调用
    pValue = PyObject_CallObject(pFunc, NULL); //调用pyhton的可调用对象,在这里就是一个函数
    Py_DECREF(pValue); //由于python函数范围值为None,pValue就没啥用,直接回收
} else {
    if (PyErr_Occurred()){
        // 捕获错误,并打印
        PyErr_Print();
    } 
    fprintf(stderr, "Cannot find function \"%s\"\n", Func_Name);
}
Py_XDECREF(pFunc);
```

python中万物皆对象,而python中对象在C中类型为`PyObject *`,模块名是对象,模块是对象,函数是对象,返回值也是对象.我们每创建一个对象,就需要在用完之后为它回收gc中的计数器.如果一定会被创建出来的对象就用[Py_DECREF](https://docs.python.org/zh-cn/3/c-api/refcounting.html?highlight=py_decref#c.Py_DecRef)回收;如果不一定能创建出来的,就用[Py_XDECREF](https://docs.python.org/zh-cn/3/c-api/refcounting.html?highlight=py_xdecref#c.Py_XDECREF)

而对于查找模块中的函数,我们需要先获取到对象(`PyObject_GetAttrString(pModule, Func_Name)`)再确认这个对象是可调用对象(`PyCallable_Check(pFunc)`).使用[PyErr_Occurred](https://docs.python.org/zh-cn/3/c-api/exceptions.html?highlight=pyerr_occurred#c.PyErr_Occurred)可以捕获异常标识,如果返回值不为空则可以使用[PyErr_ExceptionMatches](https://docs.python.org/zh-cn/3/c-api/exceptions.html?highlight=pyerr_occurred#c.PyErr_ExceptionMatches)捕获匹配异常,或者使用[PyErr_Print](https://docs.python.org/zh-cn/3/c-api/exceptions.html?highlight=pyerr_occurred#c.PyErr_Print)打印输出异常信息.

在都没有问题后我们就可以使用[PyObject_CallObject](https://docs.python.org/zh-cn/3/c-api/call.html?highlight=pyobject_callobject#c.PyObject_CallObject)调用这个python的函数对象了.

最终我们的代码如下:

+ `load_module/main.cpp`

    ```C++
    #define PY_SSIZE_T_CLEAN
    #include <Python.h>
    #include <filesystem>
    #include <iostream>

    const char* Module_Name = "hello";
    const char* Func_Name = "apply";

    int main(int argc, char* argv[]) {
        wchar_t* program = Py_DecodeLocale(argv[0], NULL);
        if (program == NULL) {
            fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
            exit(1);
        }
        auto current_dir = std::filesystem::current_path();
        auto _current_dir_name = current_dir.string().c_str();
        wchar_t* current_dir_name = Py_DecodeLocale(_current_dir_name, NULL);
        if (current_dir_name == NULL) {
            fprintf(stderr, "Fatal error: cannot decode current_dir_name\n");
            exit(1);
        } else {
            printf("current_dir %s \n", _current_dir_name);
        }

        // 初始化python环境
        PyStatus status;
        PyConfig config;
        PyConfig_InitPythonConfig(&config);
        // 设置python程序名
        status = PyConfig_SetString(&config, &config.program_name, program);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: InitPythonConfig set program_name get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        /* 加载默认配置 */
        status = PyConfig_Read(&config);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: PyConfig_Read get error\n");
            PyConfig_Clear(&config);
            exit(1);
        }
        // 设置python的sys.path用于查找模块
        config.module_search_paths_set = 1;
        status = PyWideStringList_Append(&config.module_search_paths, current_dir_name);
        if (PyStatus_Exception(status)) {
            fprintf(stderr,
                    "Fatal error: InitPythonConfig set module_search_paths get "
                    "error\n");
            PyConfig_Clear(&config);
            exit(1);
        }

        status = Py_InitializeFromConfig(&config);
        if (PyStatus_Exception(status)) {
            PyConfig_Clear(&config);
            if (PyStatus_IsExit(status)) {
                return status.exitcode;
            }
            // 抛出错误
            Py_ExitStatusException(status);
        }
        PyConfig_Clear(&config);

        // 开始导入模块
        PyObject *pName, *pModule, *pFunc;
        PyObject* pValue;

        pName = PyUnicode_DecodeFSDefault(Module_Name);  // 将模块名类型转为python对象字符串

        pModule = PyImport_Import(pName);  // 导入模块

        Py_DECREF(pName);  // 释放对象pName

        if (pModule != NULL) {
            // 在模块中找到函数名为func_name的函数,将这个函数对象提出来
            pFunc = PyObject_GetAttrString(pModule, Func_Name);
            /* pFunc is a new reference */
            if (pFunc && PyCallable_Check(pFunc)) {
                // pFunc存在且为可调用对象,则执行调用
                pValue = PyObject_CallObject(pFunc, NULL);
                Py_DECREF(pValue);

            } else {
                if (PyErr_Occurred())  // 捕获错误,并打印
                    PyErr_Print();
                fprintf(stderr, "Cannot find function \"%s\"\n", Func_Name);
            }
            Py_XDECREF(pFunc);
        } else {
            PyErr_Print();  // 捕获错误,并打印
            fprintf(stderr, "Failed to load Module\"%s\"\n", Module_Name);
            return 1;
        }
        Py_XDECREF(pModule);
        if (Py_FinalizeEx() < 0) {
            return 120;
        }
        PyMem_RawFree(current_dir_name);
        PyMem_RawFree(program);
        return 0;
    }
    ```

### 配置python解释器

我们来介绍如何配置python解释器,顺便规整上面的代码.这边的代码还在上面的例子中,只是源文件为`mainwithconf.cpp`.

#### 配置模块搜索路径

还是那个问题,要调用python模块我们需要先可以找到模块,python解释器根据其内部的全局变量`sys.path`寻找模块.python的解释器启动在命令行中看起来就是一个`python`就行了,但其实它受到好几项设置的影响,尤其是和搜索模块相关的,可以总结如下:

+ 路径配置输入,即会在调用`PyConfig_Read`之后影响`路径配置输出字段`的取值.

    可以用环境变量和`PyConfig`设置的包括:

    | 环境变量                                | `sys`中的设置    | `PyConfig`中的设置        | 默认值                                                                   | 含义                                                                     |
    | --------------------------------------- | ---------------- | ------------------------- | ------------------------------------------------------------------------ | ------------------------------------------------------------------------ |
    | `PYTHONHOME`                            | ---              | `PyConfig.home`           | 调用的`libpython`链接库查找路径的前一级,一般安装python后会在`/usr/local` | Python相关资源的所在目录                                                 |
    | `PYTHONPLATLIBDIR`                      | `sys.platlibdir` | `PyConfig.platlibdir`     | 非windows平台为`lib`;Windows上则为`DLLs`                                 | 平台专用库目录,用于构建标准库的路径和已安装扩展模块的路径                |
    | `PYTHONEXECUTABLE`(仅在 macOS 上起作用) | ---              | `PyConfig.program_name`   | `NULL`                                                                   | 用于初始化`executable`和在`Python`初始化期间早期错误消息中使用的程序名称 |
    | `PYTHONPATH`                            | `sys.path`       | `PyConfig.pythonpath_env` | `NULL`                                                                   | 增加额外的搜索路径                                                       |

    除了这些外还会`对路径配置输出字段`造成影响的包括
    + 当前工作目录,用于获取绝对路径
    + `PATH环境变量`,用于获取程序的完整路径(即默认的`PyConfig.program_name`)
    + `__PYVENV_LAUNCHER__`环境变量
    + (仅限 Windows only)注册表`HKEY_CURRENT_USER`和`HKEY_LOCAL_MACHINE`的 `"SoftwarePythonPythonCoreX.YPythonPath"` 项下的应用程序目录(其中 X.Y 为 Python 版本).

    另外`PyConfig.pathconfig_warnings`设置可以用于控制是否允许计算路径配置以将警告记录到`stderr`中(`1`为允许`0`为不允许).这个的默认值根据配置的加载模式不同不同,在本文的用法中默认为`1`

+ 路径配置输出字段,调用`PyConfig_Read`之后由`路径配置输入`共同影响生成的配置,他们依然可以用`PyConfig_SetString`,`PyWideStringList_Append`等方式进行修正

    | `sys`中的设置          | `PyConfig`中的设置                                                 | 含义                                                                                                       |
    | ---------------------- | ------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------- |
    | `sys.exec_prefix`      | `PyConfig.exec_prefix`                                             | 指定使用的python的位置,可以用于指定一个虚拟环境或和`sys.base_exec_prefix`保持一致                          |
    | `sys.prefix`           | `PyConfig.prefix`                                                  | 一个指定用于安装与平台无关的 Python 文件的站点专属目录前缀,可用于指定虚拟环境或和`sys.base_prefix`保持一致 |
    | `sys.executable`       | `PyConfig.executable`                                              | Python 解释器的可执行二进制文件的绝对路径,通常设为`NULL`                                                   |
    | `sys.base_exec_prefix` | `PyConfig.base_exec_prefix`                                        | 由`PyConfig.home`得到,指定使用的libpython的位置前缀                                                        |
    | `sys.base_prefix`      | `PyConfig.base_prefix`                                             | 由`PyConfig.home`得到,一个指定用于安装与平台无关的 Python 文件的目录前缀                                   |
    | `sys._base_executable` | `PyConfig.base_executable`                                         | 指向基础的Python环境的设置.由`__PYVENV_LAUNCHER__`环境变量设置.如为`NULL`则从`PyConfig.executable`设置.    |
    | `sys.path`             | `PyConfig.module_search_paths`和`PyConfig.module_search_paths_set` | 搜索模块的路径.                                                                                            |

在调用Python模块的C项目中我们通常会是这样的架构

```txt
\---|
    |---可执行文件
    |---env # 虚拟环境
    |---python模块 # 项目中的python业务模块
```

又或者在可执行文件的启动配置中允许指定模块位置和虚拟环境位置.

我们自己维护的python模块放在固定位置,第三方模块则放在虚拟环境中维护.这样既做到了环境隔离也更加便于维护自己的项目.当然我们也应该允许用户直接使用编译指定的libpython对应的环境.因此我们可以将解释器初始化这块抽取出来,构造一个初始化python解释器的函数:

```Cpp
...
#include <filesystem>

int init_py(char* programname, char* envpath, char* pymodulepath, bool debugmod) {
    wchar_t* program;
    wchar_t* env_dir_name;
    wchar_t* pymodule_dir_name;
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode programname\n");
        return 1;
    }

    // 初始化python设置
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        fprintf(stderr, "Fatal error: InitPythonConfig set program_name get error\n");
        PyConfig_Clear(&config);
        return 1;
    }
    // 加载默认配置
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        fprintf(stderr, "Fatal error: PyConfig_Read get error\n");
        PyConfig_Clear(&config);
        return 1;
    }
    // 设置python的sys.path用于查找模块
    std::filesystem::path pymodule_dir;
    if (pymodulepath == NULL) {
        pymodule_dir = std::filesystem::current_path();
    } else {
        pymodule_dir = pymodulepath;
        if (pymodule_dir.is_relative()) {
            pymodule_dir = std::filesystem::absolute(pymodule_dir);
        }
    }
    auto _pymodule_dir_name = pymodule_dir.string().c_str();
    pymodule_dir_name = Py_DecodeLocale(_pymodule_dir_name, NULL);
    if (pymodule_dir_name == NULL) {
        fprintf(stderr, "Fatal error: cannot decode pymodule_dir_name\n");
        return 1;
    } else {
        printf("pymodule_dir %s \n", _pymodule_dir_name);
    }
    config.module_search_paths_set = 1;
    status = PyWideStringList_Append(&config.module_search_paths, pymodule_dir_name);
    if (PyStatus_Exception(status)) {
        fprintf(stderr,
                "Fatal error: InitPythonConfig set module_search_paths get "
                "error\n");
        PyConfig_Clear(&config);
        return 1;
    }

    // 设置虚拟环境
    if (envpath != NULL) {
        std::filesystem::path env_dir = envpath;
        if (env_dir.is_relative()) {
            env_dir = std::filesystem::absolute(env_dir);
        }
        auto _env_dir_name = env_dir.string().c_str();
        env_dir_name = Py_DecodeLocale(_env_dir_name, NULL);
        if (env_dir_name == NULL) {
            fprintf(stderr, "Fatal error: cannot decode _env_dir_name \n");
            return 1;
        } else {
            printf("use virtual environments %s \n", _env_dir_name);
        }
        status = PyConfig_SetString(&config, &config.prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: InitPythonConfig set prefix get error\n");
            PyConfig_Clear(&config);
            return 1;
        }
        status = PyConfig_SetString(&config, &config.exec_prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            fprintf(stderr, "Fatal error: InitPythonConfig set exec_prefix get error\n");
            PyConfig_Clear(&config);
            return 1;
        }
    }
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        if (PyStatus_IsExit(status)) {
            return status.exitcode;
        }
        // 抛出错误
        Py_ExitStatusException(status);
    }
    PyConfig_Clear(&config);
    PyMem_RawFree(pymodule_dir_name);
    if (envpath != NULL) {
        PyMem_RawFree(env_dir_name);
    }
    PyMem_RawFree(program);
    if (debugmod) {
        PyRun_SimpleString("import sys;print(sys.path);print(sys.prefix)");
    }
    return 0;
}
```

这个函数可以通过设置参数`envpath`设置虚拟环境位置,为`NULL`时则不使用虚拟环境;参数`pymodulepath`可以用于设置项目python模块的查找路径,为`NULL`时为可执行文件所在文件夹;参数`debugmod`则用于检查是否按预期设置成功.

#### 调用python模块

python应用通常会大量调用第三方模块,我们可以正常的给用于编译的python环境或它创建的虚拟环境安装第三方模块,这样就可以在我们自己的python代码中用这些第三方模块了,我们改写`hello.py`让它调用我们安装的第三方库numpy.

```python
def callnumpy():
    import numpy as np
    print(f"numpy version: {np.version.__version__}")

```

需要注意,numpy无法同时在多个解释器中加载,这就限制了我们的应用不能起多个python解释器.当然这在我们的例子中完全没有影响,因为我们仅使用了`Py_InitializeFromConfig`启动了一个解释器,并没有搞多解释器.

但Python的CAPI是支持的,可以使用[`Py_NewInterpreter(void)`](https://docs.python.org/zh-cn/3/c-api/init.html#c.Py_NewInterpreter)和[`Py_EndInterpreter(PyThreadState *tstate)`](https://docs.python.org/zh-cn/3/c-api/init.html#c.Py_EndInterpreter)实现.我的建议也是任何情况下能单解释器就单解释器.

我们把调用python对象的部分抽出来变成一个函数:

```Cpp

int callpy() {
    const char* Module_Name = "hello";
    const char* Func_Name = "apply";
    const char* FuncCallNumpy_Name = "callnumpy";
    PyObject *pName, *pModule, *pFunc, *pFuncCallNumpy;
    PyObject* pValue;

    pName = PyUnicode_DecodeFSDefault(Module_Name);  // 将模块名类型转为python对象字符串
    pModule = PyImport_Import(pName);                // 导入模块
    Py_DECREF(pName);                                // 释放对象pName
    if (pModule != NULL) {
        // 调用numpy的函数
        pFuncCallNumpy = PyObject_GetAttrString(pModule, FuncCallNumpy_Name);
        if (pFuncCallNumpy && PyCallable_Check(pFuncCallNumpy)) {
            // pFunc存在且为可调用对象,则执行调用
            pValue = PyObject_CallObject(pFuncCallNumpy, NULL);
            Py_DECREF(pValue);
        } else {
            if (PyErr_Occurred())  // 捕获错误,并打印
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", FuncCallNumpy_Name);
        }
        Py_XDECREF(pFuncCallNumpy);

    } else {
        PyErr_Print();  // 捕获错误,并打印
        fprintf(stderr, "Failed to load Module\"%s\"\n", Module_Name);
        return 1;
    }
    Py_XDECREF(pModule);
    return 0;
}
```

#### 回收python解释器

这个没啥好说的,抽出来纯粹为了更优雅些

```Cpp
int finalize_py(){
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}
```

#### 回到例子

上面的一整段代码逻辑我们就可以拆分为如下几个部分:

1. python解释器的初始化部分

2. 调用python模块的部分

3. 回收python解释器的部分

我们再将这些串起来构成`main`函数

```Cpp

int main(int argc, char* argv[]) {
    int status;
    status = init_py(argv[0], "env/", NULL, true);
    if (status != 0) {
        return status;
    }

    // 开始执行python调用
    status = callpy();
    if (status != 0) {
        return status;
    }
    // 回收python解释器
    status = finalize_py();
    if (status != 0) {
        return status;
    }
    return 0;
}
```

## 数据交换

光调用没用,我们更加需要的是数据交换.数据交换具体来说可以看做是这样两种过程

1. 取出需要的python对象,转化成C类型后使用
2. 调用python的可调用对象,获得它的返回值,转化成C类型后使用

下面我们再对这个例子进行修改来演示C和python间的数据交换

### Python对象的所有权

我们知道Python是有垃圾回收机制的,使用的是引用计数方案,即其中所有对象都会维护一个计数器用于标记是否要被gc回收.而C中是没有的,我们在C程序中创建的Python对象并不会自动增减计数器,因此就有了上面例子中的手动调用`void Py_DECREF(PyObject *o)`或`void Py_XDECREF(PyObject *o)`释放引用的操作.

Python对象的所有权相关的描述在[官网有介绍](https://docs.python.org/zh-cn/3/extending/extending.html#reference-counts).我们大致可以整理出如下几个概念:

+ `引用计数`,对象记录的被引用的次数,当为触发gc时如果对象的引用计数为0,对象会被回收
+ `强引用`, 持有引用的代码所拥有的对象的引用.在创建引用时可通过调用`Py_INCREF()`来获取强引用而在删除引用时可通过`Py_DECREF()`来释放它.
+ `借入引用`, 使用借入引用的代码并不持有这个引用,也就是说这个引用**不会触发**`引用计数`的增加,推荐在`借入引用`上调用`Py_INCREF()`以将其原地转换为强引用.
+ `引用的所有权`,`强引用`的所有权归创造这个引用的代码(即调用返回Pyhton对象引用`PyObject *`的函数的代码)所有,`借入引用`的所有权归被借的对象(即返回Pyhton对象引用`PyObject *`的函数)管理.
+ `偷走所有权`,指以Pyhton对象引用`PyObject *`为参数函数会拿走调用方代码对传入参数的引用所有权.

多数Python的CAPI函数中以`PyObject *`为类型的参数都是借入引用,也就是说你的代码调用这些函数时并不会失去传入对象的引用所有权,你依然需要手工回收他们,当然也有特例,就是`void PyList_SET_ITEM(PyObject *list, Py_ssize_t i, PyObject *o)`和`int PyTuple_SetItem(PyObject *p, Py_ssize_t pos, PyObject *o)`中的`o`,他们会偷走`o`的所有权,这也就意味着你不需要处理这两个函数中`o`参数的引用计数回收的工作了.

而多数Python的CAPI函数中以`PyObject *`为类型的返回值都是强引用,也就是说我们需要手动控制这些对象引用的回收.当然也有例外,容器的`GetItem`接口即`PyTuple_GetItem()`,`PyList_GetItem()`, `PyDict_GetItem()`和`PyDict_GetItemString()`这四个接口都是返回的`借入引用`,也就是说我们通常不需要手工释放其引用计数,当然如果有必要,也可以调用`Py_INCREF()`来获取自己的引用所有权,但记得这样就得自己用`Py_DECREF()`释放了

我们需要额外小心容器类型和有属性的对象的操作,因为不同容器内的元素有的是强引用(`偷走所有权`)有的是`借入引用`,需要查询接口文档才可以确认.

当我们处理容器或有属性的对象中的元素时,如果涉及到多个元素间的使用或比较复杂的逻辑时,建议使用`Py_INCREF()`获得的元素所有权后再使用,这可以避免很多问题.官方给出过一个这样的例子:

```C
void
bug(PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);

    PyList_SetItem(list, 1, PyLong_FromLong(0L));
    PyObject_Print(item, stdout, 0); /* BUG! */
}
```

上面这个代码初看没啥问题,但实际不能这样写,会有风险.应该改成:

```C
void
no_bug(PyObject *list)
{
    PyObject *item = PyList_GetItem(list, 0);

    Py_INCREF(item); //增加的
    PyList_SetItem(list, 1, PyLong_FromLong(0L));
    PyObject_Print(item, stdout, 0);
    Py_DECREF(item); //增加的
}
```

为什么呢?因为`PyList_SetItem(list, 1, PyLong_FromLong(0L));`这一句会触发`list[1]`元素的替换,这种替换在原始元素对象没有别的引用时会触发删除操作调用`__del__`方法.如果刚好这个对象的的`__del__`方法中有对`list[0]`元素的删除操作,由于`PyList_GetItem`返回的是`借入引用`,只要别处没有这个元素的引用了,那在执行完`PyList_SetItem(list, 1, PyLong_FromLong(0L));`后`item`就没了.

也就是说处理借入引用最稳妥的办法是借入后就申请所有权,用完再释放所有权.

### 数据类型转换

Python中万物都是对象,在C这个层面看就是万物都是`PyObject*`,这也就意味着无论是无论是获取数据还是作为参数,我们都需要在`PyObject*`和python各种对象之间进行转换.这个太多了,可以查看[官方文档](https://docs.python.org/zh-cn/3/c-api/concrete.html).这里介绍一些比较常用的类型对应的使用方法,我尽量挑使用场景比较多的列出来.

#### None类型

None类型是一个单例,就是`Py_None`,也就是说在C/C++中并不需要做转换,判断也只需要`x == Py_None`即可

None常作为参数或返回值用于数据交换

#### 布尔类型

C中实际没有布尔值,而是0和其他作为false和true.

+ 使用`int PyBool_Check(PyObject *o)`来判断对象是否是python中的布尔类型.
+ 使用`PyObject *PyBool_FromLong(long v)`可以从C中用long类型的数据构造一个python中的布尔型对象,`0`会转为`False`,其他会转为`True`
+ 单例`Py_False`对应python中的`False`,`Py_True`对应pyhton中的`True`.在C中可以直接使用`==`进行判断

布尔类型常作为参数或返回值用于数据交换

#### 数值类型

数值类型python和C中基本都有对应

数值类型都常作为参数或返回值用于数据交换

##### 整型数

python中的整型数是不限长度的,这就为转换提供了难度.

+ 使用`int PyLong_Check(PyObject *p)`可以用于检测python对象是不是整型数

+ 使用如下函数可以从C类型的对象构造Python的整型数对象

    | 函数                                                                  | C类型                | 说明                                                                  |
    | --------------------------------------------------------------------- | -------------------- | --------------------------------------------------------------------- |
    | `PyObject *PyLong_FromLong(long v)`                                   | `long`               | ---                                                                   |
    | `PyObject *PyLong_FromUnsignedLong(unsigned long v)`                  | `unsigned long`      | ---                                                                   |
    | `PyObject *PyLong_FromLongLong(long long v)`                          | `long long`          | ---                                                                   |
    | `PyObject *PyLong_FromUnsignedLongLong(unsigned long long v)`         | `unsigned long long` | ---                                                                   |
    | `PyObject *PyLong_FromDouble(double v)`                               | `double`             | 会仅保留整数部分                                                      |
    | `PyObject *PyLong_FromString(const char *str, char **pend, int base)` | `char *str`          | 用于构造非10进制整型数,base为0则还是十进制也就是字面量,不为0则必须在2 |
    ~36之间;pend需要传入一个指针或NULL,如果不是NULL则会将出错时第一个字符的指针填入其中.

+ 使用如下函数将python的整型转换为对应的C类型

    | 函数                                                                   | C类型                | 说明                                                                                                                                         |
    | ---------------------------------------------------------------------- | -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
    | `long PyLong_AsLong(PyObject *obj)`                                    | `long`               | 超出范围会返回`-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError`                                                                        |
    | `long PyLong_AsLongAndOverflow(PyObject *obj, int *overflow)`          | `long`               | 超出`LONG_MAX`会给传入的参数`overflow`赋值`1`;小于`LONG_MIN`则会赋值`-1`;出错则返回`-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError`   |
    | `long long PyLong_AsLongLong(PyObject *obj)`                           | `long long`          | 超出范围会返回`-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError`                                                                        |
    | `long long PyLong_AsLongLongAndOverflow(PyObject *obj, int *overflow)` | `long long`          | 超出`LLONG_MAX`会给传入的参数`overflow`赋值`1`;小于`LLONG_MIN`则会赋值`-1`;出错则返回`-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError` |
    | `unsigned long PyLong_AsUnsignedLong(PyObject *pylong)`                | `unsigned long`      | 超出范围会返回`(unsigned long)-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError`                                                         |
    | `unsigned long long PyLong_AsUnsignedLongLong(PyObject *pylong)`       | `unsigned long long` | 超出范围会返回`(unsigned long long)-1`且需要使用`PyErr_Occurred()`捕获异常`OverflowError`                                                    |
    | `unsigned long PyLong_AsUnsignedLongMask(PyObject *obj)`               | `unsigned long`      | 超出范围不会抛出异常,会返回`ULONG_MAX + 1 求模的余数`,其他异常会返回`(unsigned long)-1`                                                      |
    | `unsigned long long PyLong_AsUnsignedLongLongMask(PyObject *obj)`      | `unsigned long long` | 超出范围不会抛出异常,会返回`ULLONG_MAX + 1 求模的余数`,其他异常会返回`(unsigned long long)-1`                                                |
    | `double PyLong_AsDouble(PyObject *pylong)`                             | `double`             | 超出了`double`的取值范围则需要使用`PyErr_Occurred()`捕获异常`OverflowError`,且返回`-1.0`                                                     |

##### 浮点数

+ 使用`int PyFloat_Check(PyObject *p)`可以用于检测python对象是不是float类型

+ 使用如下函数可以从C类型的对象构造Python的整型数对象

    | 函数                                     | C类型    | 说明         |
    | ---------------------------------------- | -------- | ------------ |
    | `PyObject *PyFloat_FromDouble(double v)` | `double` | 失败返回NULL |

+ 使用如下函数将python的整型转换为对应的C类型

    | 函数                                         | C类型    | 说明                                                      |
    | -------------------------------------------- | -------- | --------------------------------------------------------- |
    | `double PyFloat_AsDouble(PyObject *pyfloat)` | `double` | 失败时将返回`-1.0`,并且需要调用PyErr_Occurred()来检测错误 |

#### 字符串类型

字符串是个很麻烦的概念,它是文本的形式,但由于编码的存在即便是在Python中Python2和Python3中类型都不同.C是一门非常古老的编程语言,在创建它的时候字符串就是字节串,每个字节都是ascii码,但随着计算机的发展,仅能表现英语的ascii码显然无法涵盖所有字符,当时的所谓字符串也就名不副实了.为了解决非英语的字符的使用问题,unicode应运而生,通过编码解码实现了这一需求,成为了真正的字符串,而原本的字符串成了实质上的字节串.
python中的字符串和C中对应的东西在称呼上是这样对应的

| 名称    | C中类型     | Python3中类型 |
| ------- | ----------- | ------------- |
| 字符串  | `char *`    | `bytes`       |
| unicode | `wchar_t *` | `str`         |

我们通常说的字符串又往往指的是带编码的unicode,而不带编码的通常我们会叫它字节串或字节流.但在现在这个场景下,我们以表中的命名为准.

字符串类型常作为参数或返回值用于数据交换

##### 字符串

+ 使用`int PyBytes_Check(PyObject *o)`可以用于检测python对象是不是bytes类型

+ 使用如下函数可以从C类型的对象构造Python的整型数对象

    | 函数                                                    | C类型    | 说明                                                                                                                     |
    | ------------------------------------------------------- | -------- | ------------------------------------------------------------------------------------------------------------------------ |
    | `PyObject *PyBytes_FromString(const char *v)`           | `char *` | 失败返回NULL,形参`v`不可为NULL                                                                                           |
    | `PyObject *PyBytes_FromFormat(const char *format, ...)` | `char *` | 接受一个C`printf()`风格的format字符串和可变数量的参数,计算结果 Python 字节串对象的大小并返回参数值经格式化后的字节串对象 |

+ 使用如下函数将python的整型转换为对应的C类型

    | 函数                                  | C类型    | 说明                                                                                                           |
    | ------------------------------------- | -------- | -------------------------------------------------------------------------------------------------------------- |
    | `char *PyBytes_AsString(PyObject *o)` | `char *` | 返回结果的缓冲区的最后一个字节总是为空,失败时将返回`NULL`并引发`TypeError`需要调用`PyErr_Occurred()`来检测错误 |

##### unicode

+ 使用`int PyUnicode_Check(PyObject *o)`可以用于检测python对象是不是str类型

+ 使用如下函数可以从C类型的对象构造Python的整型数对象

    | 函数                                                                     | C类型       | 说明                                                                                                                               |
    | ------------------------------------------------------------------------ | ----------- | ---------------------------------------------------------------------------------------------------------------------------------- |
    | `PyObject *PyUnicode_FromString(const char *str)`                        | `char *`    | UTF-8编码                                                                                                                          |
    | `PyObject *PyUnicode_DecodeFSDefault(const char *str)`                   | `char *`    | 用系统设置的解码器解码参数字符串为python中的str                                                                                    |
    | `PyObject *PyUnicode_FromFormat(const char *format, ...)`                | `char *`    | UTF-8编码,接受一个C`printf()`风格的format字符串和可变数量的参数,计算结果 Python 字节串对象的大小并返回参数值经格式化后的字节串对象 |
    | `PyObject *PyUnicode_FromWideChar(const wchar_t *wstr, Py_ssize_t size)` | `wchar_t *` | 将`-1`作为大小表示本函数会使用`wcslen()`自己计算长度                                                                               |

+ 使用如下函数将python的整型转换为对应的C类型

    | 函数                                                                       | C类型       | 说明                                                                                                                                                                                                                                                                                                  |
    | -------------------------------------------------------------------------- | ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
    | `const char *PyUnicode_AsUTF8(PyObject *unicode)`                          | `char *`    | 将python中的str以utf-8解码后返回为对应的C中的字符串                                                                                                                                                                                                                                                   |
    | `wchar_t *PyUnicode_AsWideCharString(PyObject *unicode, Py_ssize_t *size)` | `wchar_t *` | 如果参数`size` 不为`NULL`则会将宽字符的数量(不包括结尾空字符)写入到`*size`中;如果`size`为`NULL`并且`wchar_t*`字符串包含空字符则将引发`ValueError`.成功时返回由`PyMem_New`分配的缓冲区(使用`PyMem_Free()`来释放它.发生错误时则返回`NULL`并且`*size`将是未定义的.如果内存分配失败则会引发`MemoryError`. |

#### 容器类型

容器类型单独创建没什么意义,关键是将其中的元素放进去拿出来.

不同容器的操作引用逻辑会有不同,需要额外注意

<!-- 容器类型单独创建没什么意义,关键是将其中的元素放进去拿出来,我们假定容器中的元素都是python中的`str`类型,需要转到C中的`char *`.下面开始具体介绍 -->

##### 元组类型

元组非常重要,因为可调用对象如果是多值返回,返回的就是一个元组,如果调用函数,元组也常被用于传递位置参数

+ 使用`int PyTuple_Check(PyObject *p)`可以用于检测python对象是不是tuple类型

+ 构造: 有两种构造方法
  
    1. 使用`PyObject *PyTuple_Pack(Py_ssize_t n, ...)`,其中`n`为元组长度,可变参数为元素,注意可变参数部分必须是`PyObject *`类型.

        ```C
        PyObject * x,y,t;
        x = PyUnicode_DecodeFSDefault("12345");
        y = PyUnicode_DecodeFSDefault("abcde");
        t = PyTuple_Pack(2, x, y);
        ...
        Py_DECREF(x);
        Py_DECREF(y);
        Py_DECREF(t);
        ```

    2. 使用`PyObject *PyTuple_New(Py_ssize_t len)`构建一个固定长度的元组,然后使用`int PyTuple_SetItem(PyObject *p, Py_ssize_t pos, PyObject *o)`设置对应位置上的元素.**需要注意**`PyTuple_SetItem`接口会`偷走传入对象引用的所有权`

        ```C
        PyObject * x,y,t;
        t = PyTuple_New(2);
        x = PyUnicode_DecodeFSDefault("12345");
        y = PyUnicode_DecodeFSDefault("abcde");
        PyTuple_SetItem(t,1,x);
        PyTuple_SetItem(t,2,y);
        ...
        Py_DECREF(t);
        ```

+ 提取元素: 通常是如下步骤:

    1. 使用`Py_ssize_t PyTuple_Size(PyObject *p)`获得元组长度
    2. for循环元组长度,使用`PyObject *PyTuple_GetItem(PyObject *p, Py_ssize_t pos)`获取对应位置的元素,**注意**这个接口获取的是对象的`借入引用`
    3. 分别使用其他类型的检测和提取功能获取C部分

    ```C
    #include <vector>
    ...
    Py_ssize_t length;
    Py_ssize_t i;
    PyObject * item;
    std::vector<char*> result={};
    length = PyTuple_Size(t);

    for (i=0;i<length;i++){
        item = PyTuple_GetItem(t, i);
        PyUnicode_AsUTF8(item);
        result.push_back(PyUnicode_AsUTF8(item));
    }
    ...
    ```

##### 字典类型

字典非常重要,常会用在调用可调用对象时作为关键字参数传递,我们应当尽量不要传递字典作为返回值或全局变量的值.对于结构化数据的传递更加推荐使用`json`等文本序列化协议.

+ 使用`int PyDict_Check(PyObject *p`可以用于检测python对象是不是Dict类型

+ 构造: 通常构造的步骤如下

    1. 使用`PyObject *PyDict_New()`构造一个指定长度的空dict
    2. 使用`int PyDict_SetItem(PyObject *p, PyObject *key, PyObject *val)`或`int PyDict_SetItemString(PyObject *p, const char *key, PyObject *val)`将对象键值对一个一个加到字典中.**需要注意**`PyDict_SetItem`接口不会偷走传入对象引用的所有权,因此你还是得手动释放传入对象在这段代码中的所有权

    ```C
    PyObject * x,y,d;
    d = PyDict_New();
    x = PyUnicode_DecodeFSDefault("12345");
    PyDict_SetItemString(d,"key1" x);
    y = PyUnicode_DecodeFSDefault("abcde");
    PyDict_SetItemString(d,"key2" y);
    ...
    Py_DECREF(x);
    Py_DECREF(y);
    Py_DECREF(d);
    ```

+ 提取元素: 通常是如下步骤

    1. 使用`PyObject *PyDict_Keys(PyObject *p)`获得Dict中的所有key,注意返回的值是用Python的List对象,
    2. 使用`Py_ssize_t PyList_Size(PyObject *list)`获得list长度
    3. for循环这个key的list长度,使用`PyObject *PyList_GetItem(PyObject *list, Py_ssize_t index)`获取key对象,注意这个接口`PyList_GetItem`获得的是对象的`借入引用`
    4. 使用`PyObject *PyDict_GetItem(PyObject *p, PyObject *key)`获取key对应的取值,分别使用其他类型的检测和提取功能获取C部分.注意这个接口`PyDict_GetItem`获得的是也是对象的`借入引用`

    ```C
    #include <map>
    ...
    Py_ssize_t length;
    Py_ssize_t i;
    PyObject * keys;
    PyObject * key;
    PyObject * value;
    std::map<char*, char*> result={};

    keys = PyDict_Keys(PyObject *d)
    length = PyList_Size(keys);
    for (i=0;i<length;i++){
        key = PyList_GetItem(keys, i);
        char* map_key= PyUnicode_AsUTF8(key);
        value = PyDict_GetItem(d, key);
        map_value = PyUnicode_AsUTF8(value);
        result[map_key] = map_value;
    }
    ...
    Py_DECREF(keys);
    ...
    ```

如果对象并不是字典而是一些其他满足Map协议的对象,可以使用`int PyMapping_Check(PyObject *o)`替代`PyDict_Check`的功能;用`PyObject *PyObject_GetItem(PyObject *o, PyObject *key)`即python中的`o[key]`操作来获取key的值;用`int PyObject_SetItem(PyObject *o, PyObject *key, PyObject *v)`(注意该函数 不会偷取`v`的引用计数)即python中的`o[key] = v`来做key的赋值操作,用`int PyObject_DelItem(PyObject *o, PyObject *key)`即python中的`del o[key]`来删除key

##### 列表类型

我们应该尽量控制接口不要用列表作为参数或返回值

+ 使用`int PyList_Check(PyObject *p)`可以用于检测python对象是不是list类型

+ 构造: 通常构造的步骤如下

    1. 使用`PyObject *PyList_New(Py_ssize_t len)`构造一个指定长度的空list,当`len`大于零时被返回的列表对象项目被设成`NULL`,通常`len`会设成`0`
    2. 使用`int PyList_Append(PyObject *list, PyObject *item)`或`void PyList_SET_ITEM(PyObject *list, Py_ssize_t i, PyObject *o)`将对象一个一个加到list中.需要注意`PyList_SET_ITEM`会`偷走传入对象o的所有权`,而`PyList_Append`不会.

    ```C
    PyObject * x,y,l;
    l = PyList_New(0);
    x = PyUnicode_DecodeFSDefault("12345");
    PyList_Append(l, x)
    Py_DECREF(x);
    y = PyUnicode_DecodeFSDefault("abcde");
    PyList_Append(l, y)
    Py_DECREF(y);
    ...
    Py_DECREF(x);
    Py_DECREF(y);
    Py_DECREF(l);
    ```

+ 提取元素: 通常是如下步骤

    1. 使用`Py_ssize_t PyList_Size(PyObject *list)`获得list长度
    2. for循环list长度,使用`PyObject *PyList_GetItem(PyObject *list, Py_ssize_t index)`获取对应位置的元素,注意`PyList_GetItem`返回的是`借入引用`.
    3. 分别使用其他类型的检测和提取功能获取C部分

    ```C
    #include <vector>
    ...
    Py_ssize_t length;
    Py_ssize_t i;
    PyObject * item;
    std::vector<char*> result={};
    length = PyList_Size(l);
    for (i=0;i<length;i++){
        item = PyList_GetItem(l, i);
        PyUnicode_AsUTF8(item)
        result.push_back(PyUnicode_AsUTF8(item));
    }
    ...
    ```

如果是其他非列表的序列类型,可以使用`int PySequence_Check(PyObject *o)`替代`PyList_Check`来进行检测,;用`PyObject *PyObject_GetItem(PyObject *o, PyObject *key)`即python中的`o[key]`操作来获取key的值;用`int PyObject_SetItem(PyObject *o, PyObject *key, PyObject *v)`(注意该函数不会偷取`v`的引用计数)即python中的`o[key] = v`来做key的赋值操作;用`int PySequence_Contains(PyObject *o, PyObject *value)`判断元素是否在序列中(在返回`1`,否则返回`0`.出错时返回`-1`);`Py_ssize_t PySequence_Index(PyObject *o, PyObject *value)`获取指定元素在序列中的index(出错返回`-1`).

### 访问对象中字段

数据交换中第一种方式是从一个python对象中获取字段.这个对象可以是模块对象,类对象,类的实例对象,具名元组对象等.这些对象都可以用如下接口访问其中的字段.

#### 判断对象中是否存在字段

有如下两个接口可以用于判断对象中是否有对应的字段,他们是

+ `int PyObject_HasAttr(PyObject *o, PyObject *attr_name)`
+ `int PyObject_HasAttrString(PyObject *o, const char *attr_name)`

这两个函数功能都相当于python中的`hasattr(o, attr_name)`二者区别仅在于一个使用Python的`str`类型作为参数查找,一个是使用C的`char*`类型的UTF-8编码字符串作为参数.

#### 通过字段取值

通过字段取值可以使用如下两个接口:

+ `PyObject *PyObject_GetAttr(PyObject *o, PyObject *attr_name)`
+ `PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)`

这两个函数功能都相当于python中的`getattr(o, attr_name)`.二者区别仅在于一个使用Python的`str`类型作为参数查找,一个是使用C的`char*`类型的UTF-8编码字符串作为参数.成功返回属性值,失败则返回`NULL`.需要注意这两个接口返回的都是`强引用`

#### 设置字段的值

设置字段的值可以使用如下两个接口:

+ `int PyObject_SetAttr(PyObject *o, PyObject *attr_name, PyObject *v)`
+ `int PyObject_SetAttrString(PyObject *o, const char *attr_name, PyObject *v)`

这两个函数功能都相当于python中的`setattr(o, attr_name)`.二者区别仅在于一个使用Python的`str`类型作为参数`attr_name`查找,一个是使用C的`char*`类型的UTF-8编码字符串作为参数.成功返回0,失败则返回`-1`.

注意如果`v`为`NULL`则该字段会被移除.接口不会头灶传入的`v`的所有权.

### 调用可调用对象

Python中可调用对象大致可以分

+ 函数
+ 类,调用即实例化
+ 静态方法,类方法
+ 实例方法

他们可以使用`PyObject *PyObject_Call(PyObject *callable, PyObject *args, PyObject *kwargs)`来调用,其中`callable`是可调用对象;`args`必须为一个python的`tuple`对象,表示位置参数;`kwargs`可以为`NULL`或pyhton的`dict`对象,表示关键字参数.这个接口相当于在python中调用`callable(*args,**kwargs)`,十分通用.

用法大致流程如下:

1. 检查是否是可执行对象,不是就报错

    ```C

    if (PyCallable_Check(pFunc)){
        ....
    }else{
        fprintf(stderr, "pFunc not callable");
        ...
    }
    ```

2. 如果是可执行对象则先构造参数

    ```C
    // 构造参数args
    auto x = PyLong_FromLong(longx);
    auto y = PyFloat_FromDouble(0.25);
    auto args = PyTuple_Pack(2, x, y);

    // 构造kwargs
    auto kwargs = PyDict_New();
    auto z = PyList_New(0);
    std::vector<char*> v = {"item1", "item2", "item3"};
    for (auto i : v) {
        auto item = PyUnicode_DecodeFSDefault(i);
        PyList_Append(z, item);
        Py_DECREF(item);  //---注意
    }                     // access by value, the type of i is int
    PyDict_SetItemString(kwargs, "z", z);

    ...

    // 参数gc计数器回收
    Py_DECREF(args);
    Py_DECREF(kwargs);
    Py_DECREF(x);
    Py_DECREF(y);
    Py_DECREF(z);
    ```

    需要注意:
    1. 第一个参数固定为python的`tuple`类型,相当于`*args`,不可以为`NULL`,如果没参数就传入一个`PyTuple_New(0);`
    2. 第二个参数固定为pyhton的`dict`类型,相当于`*kwargs`,可以为`NULL`
    3. 记得调用完成后回收这些参数和构造参数所用到对象的引用所有权.

3. 执行函数调用

    ```C
    ...
    // 调用函数对象
    auto result = PyObject_Call(pFunc, args, kwargs);
    ...
    ```

4. 处理调用返回对象和调用中触发的异常

    ```Cpp
    ...
    if (result != NULL) {
        auto result_double = PyFloat_AsDouble(result);
        printf("Result of call: %f\n", result_double);
    } else {
        PyErr_Print(); // 处理异常
        fprintf(stderr, "Call %s failed\n", Claz_Name);
    }
    Py_XDECREF(result); //回收返回值的引用所有权
    ...
    ```

至此C中对Python模块的简单调用算是告一段落了,有了以上知识就已经可以用C简单地调用Python模块了.

### 利用C++进一步优化

上面的例子中虽然我们已经使用了大量的C++特性,但基本框架依然是C的风格,我们可以进一步使用C++库和C++中的异常让我们的代码更加简洁优雅.这个例子可以参考[mainwithmore.cpp]()

## 外部线程和服务化

继续我们的介绍.C中简单调用Python模块适用于那种单一进程单一线程的简单程序,必须是在单线程中才不会有问题.如果涉及到多线程,那么由于gc的存在,调用会出问题.

我们很多时候就是希望用python快速迭代做算法实现,用C做高性能服务器,要满足这样的需求,就得参考这部分的内容了.

这部分的例子在[](),我们使用[Crow](https://github.com/CrowCpp/Crow)这个库来实现一个简单的http服务.这个库允许我们仅使用一个头文件就像flask一样轻松构造一个简单的http服务.而仅仅只需要装个依赖`asio`(macos下使用`brew install asio`安装)

我们先将项目结构定下:

```txt
\---|
    |---env/  # python的虚拟环境
    |---m.py  # 待加载python模块
    |---crow_all.h  # crow的头文件,这边简单起见使用的是单文件方式
    |---main.cpp    # 服务源文件

```

然后通过如下命令编译,注意这是在我的mac上进行的,请作为参考自行修改编译命令

```bash
g++ -I /Users/mac/micromamba/envs/py3.10/include/python3.10 \
    -I /usr/local/include \
    -L/Users/mac/micromamba/envs/py3.10/lib \
    -lpython3.10 \
    -o numpyserv \
    -std=c++17 \
    -Wwrite-strings \
    main.cpp
```

我们的例子实现的是一个返回指定`d`值的元素取值为`0~1`间数据的随机向量,这个`d`的取值范围为`1~20`之间的整数.我们规定参数的校验和转换工作在Cpp端处理.python端仅需要实现功能即可.

+ `m.py`加载的python模块,固定调用`apply(n: int) -> list[float]`这个接口.

    ```python
    import numpy as np


    def apply(n: int) -> list[float]:
        arr = np.random.random(n).tolist()
        return arr

    ```

+ `main.cpp`入口,这个入口需要管python的生命周期和请求的调用,

    ```C++
    #define PY_SSIZE_T_CLEAN
    #include <Python.h>
    #include "crow_all.h"



    int main() {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/random_array")
        ([](const crow::request& req) {
            auto d = req.url_params.get("d");
            if (!d) {
                return crow::response(crow::status::BAD_REQUEST);  // same as crow::response(400)
            }
            crow::json::wvalue x({{"message", "Hello, World!"}});
            x["d"] = d;
            return crow::response(x);
        });
        app.port(18080).multithreaded().run();
    }
    ```

### 外部线程和gil