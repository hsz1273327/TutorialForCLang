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
            fprintf(stderr,
                    "Fatal error: InitPythonConfig set program_name get error\n");
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
        status =
            PyWideStringList_Append(&config.module_search_paths, current_dir_name);
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

        pName = PyUnicode_DecodeFSDefault(
            Module_Name);  // 将模块名类型转为python对象字符串

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

        PyMem_RawFree(program);
        return 0;
    }
    ```

### 更进一步的初始化python解释器


## 数据交换