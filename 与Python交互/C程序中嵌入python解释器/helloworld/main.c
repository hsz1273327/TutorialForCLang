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
        fprintf(stderr,
                "Fatal error: InitPythonConfig set program_name get error\n");
        PyConfig_Clear(&config);
        exit(1);
    }

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        fprintf(stderr, "Fatal error: Py_InitializeFromConfig get error\n");
        PyConfig_Clear(&config);
        exit(1);
    }
    PyConfig_Clear(&config);

    PyRun_SimpleString(
        "from time import time,ctime\n"
        "print('Today is', ctime(time()))\n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    return 0;
}