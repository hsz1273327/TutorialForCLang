#define PY_SSIZE_T_CLEAN
#include <Python.h>
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

int callpy() {
    const char* Module_Name = "hello";
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


int finalize_py(){
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}

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