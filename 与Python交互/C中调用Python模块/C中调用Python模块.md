# C中调用Python模块

我们知道Python本质上是一个C写的虚拟机,它本身就提供了与C交互的接口.我们先来一个[helloworld例子]()展示如何在C中执行python代码.

+ `helloworld/main.c`

    ```C
    #define PY_SSIZE_T_CLEAN
    #include <Python.h>

    int
    main(int argc, char *argv[])
    {
        wchar_t *program = Py_DecodeLocale(argv[0], NULL);
        if (program == NULL) {
            fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
            exit(1);
        }
        Py_SetProgramName(program);  /* optional but recommended */
        Py_Initialize();
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

+ [`wchar_t *program = Py_DecodeLocale(argv[0], NULL)`](https://docs.python.org/zh-cn/3/c-api/sys.html?highlight=py_decodelocale#c.Py_DecodeLocale)用于将可执行文件的文件名作为python程序的应用名,通过[`Py_SetProgramName(program)`](https://docs.python.org/zh-cn/3/c-api/init.html#c.Py_SetProgramName)设置这个python应用,同时向解释器告知 Python运行库的路径;整个程序结束后调用[`PyMem_RawFree(program)`](https://docs.python.org/zh-cn/3/c-api/memory.html#c.PyMem_RawFree)回收释放这个应用名对应的内存.

+ 调用[`Py_Initialize()`](https://docs.python.org/zh-cn/3/c-api/init.html?highlight=py_initialize#c.Py_Initialize)初始化Python解释器,同时在执行完成后用[`if (Py_FinalizeEx() < 0) {exit(120);}`](https://docs.python.org/zh-cn/3/c-api/init.html?highlight=py_initialize#c.Py_FinalizeEx)关闭解释器.

+ `PyRun_SimpleString("from time import time,ctime;print('Today is', ctime(time()))\n");`则是实际执行的python代码.[PyRun_SimpleString](https://docs.python.org/zh-cn/3/c-api/veryhigh.html?highlight=pyrun_simplestring#c.PyRun_SimpleString)算是Python CAPI的一个高级接口,它的作用是将传入的字符串导入python解释器中执行.类似的还有[`PyRun_SimpleFile`](https://docs.python.org/zh-cn/3/c-api/veryhigh.html?highlight=pyrun_simplestring#c.PyRun_SimpleFile)用于从文件中读取脚本执行.注意,在Windows上，`fp`应当以二进制模式打开(即`fopen(filename, "rb")`);在其他平台则使用`fopen(filename,"r")`.


