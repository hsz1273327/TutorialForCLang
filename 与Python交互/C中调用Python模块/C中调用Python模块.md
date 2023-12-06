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

### 数据类型转换

Python中万物都是对象,在C这个层面看就是万物都是`PyObject*`,这也就意味着无论是无论是获取数据还是作为参数,我们都需要在`PyObject*`和python各种对象之间进行转换.这个太多了,可以查看[官方文档](https://docs.python.org/zh-cn/3/c-api/concrete.html).这里介绍几个比较常用的接口

#### None类型

#### 布尔类型

#### 数值类型

#### 字符串类型

#### 字典类型

#### 列表类型

#### 可迭代对象类型

#### 缓冲协议类型

#### 类型类型

### 获取对象中字段对应的值



### 调用可调用对象


#### 函数对象的调用

#### 类实例化调用

#### 方法调用

## 外部线程和服务化

### 外部线程和gil