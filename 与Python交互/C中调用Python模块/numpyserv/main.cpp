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