#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>
#include <vector>

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
        if (debugmod) {
            printf("pymodule_dir %s \n", _pymodule_dir_name);
        }
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
            if (debugmod) {
                printf("use virtual environments %s \n", _env_dir_name);
            }
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

int call_func_example(PyObject* pModule,long longx) {
    // 在模块中找到函数名为`call_with_params_and_return`的函数,将这个函数对象提出来
    const char* Func_Name = "call_with_params_and_return";
    auto pFunc = PyObject_GetAttrString(pModule, Func_Name);
    /* pFunc is a new reference */
    if (pFunc && PyCallable_Check(pFunc)) {
        // pFunc存在且为可调用对象,则执行调用
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

        // 调用函数对象
        auto result = PyObject_Call(pFunc, args, kwargs);
        // 参数gc计数器回收
        Py_DECREF(args);
        Py_DECREF(kwargs);
        Py_DECREF(x);
        Py_DECREF(y);
        Py_DECREF(z);
        // 提取结果数据
        if (result != NULL) {
            auto is_tuple = PyTuple_Check(result);
            if (!is_tuple) {
                fprintf(stderr, "Call %s get result not tuple\n", Func_Name);
                return 1;
            }
            auto size = PyTuple_Size(result);
            if (size != 2) {
                fprintf(stderr, "Call %s get result len not 2\n", Func_Name);
                return 1;
            }
            auto result1 = PyTuple_GetItem(result, 0);
            auto result1_string = PyUnicode_AsUTF8(result1);
            printf("Result1 of call: %s\n", result1_string);
            auto result2 = PyTuple_GetItem(result, 1);
            auto result2_string = PyUnicode_AsUTF8(result2);
            printf("Result2 of call: %s\n", result2_string);
        } else {
            PyErr_Print();
            fprintf(stderr, "Call %s failed\n", Func_Name);
        }
        // 返回值计数回收
        Py_XDECREF(result);
    } else {
        if (PyErr_Occurred())  // 捕获错误,并打印
            PyErr_Print();
        fprintf(stderr, "Cannot find function \"%s\"\n", Func_Name);
    }
    Py_XDECREF(pFunc);
    return 0;
}

int call_class_example(PyObject* pModule) {
    // 在模块中找到类名为`PyVector`的类,将这个类对象提出来
    const char* Claz_Name = "PyVector";
    auto pClaz = PyObject_GetAttrString(pModule, Claz_Name);
    /* pClaz is a new reference */
    if (pClaz && PyCallable_Check(pClaz)) {
        // pClaz存在且为可调用的类对象,则执行调用
        // 构造参数args
        auto x = PyLong_FromLong(1.5);
        auto y = PyFloat_FromDouble(0.5);
        auto args = PyTuple_Pack(2, x, y);
        // 调用函数对象
        auto pObj = PyObject_Call(pClaz, args, NULL);
        // 参数gc计数器回收
        Py_DECREF(args);
        Py_DECREF(x);
        Py_DECREF(y);
        // 提取结果数据
        if (pObj != NULL) {
            const char* Method_Name = "calculate_mod";
            auto pMethod = PyObject_GetAttrString(pObj, Method_Name);
            if (pMethod && PyCallable_Check(pMethod)) {
                auto margs = PyTuple_New(0);
                auto result = PyObject_Call(pMethod, margs, NULL);
                Py_DECREF(margs);
                if (result != NULL) {
                    auto result_double = PyFloat_AsDouble(result);
                    printf("Result of call: %f\n", result_double);
                } else {
                    PyErr_Print();
                    fprintf(stderr, "Call %s failed\n", Claz_Name);
                }
                // 返回值计数回收
                Py_XDECREF(result);
            } else {
                if (PyErr_Occurred())  // 捕获错误,并打印
                    PyErr_Print();
                fprintf(stderr, "Cannot find class \"%s\" . \"%s\"\n", Claz_Name, Method_Name);
            }
            Py_XDECREF(pMethod);
        } else {
            PyErr_Print();
            fprintf(stderr, "Call %s failed\n", Claz_Name);
        }
        // 返回值计数回收
        Py_XDECREF(pObj);
    } else {
        if (PyErr_Occurred())  // 捕获错误,并打印
            PyErr_Print();
        fprintf(stderr, "Cannot find class \"%s\"\n", Claz_Name);
    }
    Py_XDECREF(pClaz);
    return 0;
}

int callpy() {
    const char* Module_Name = "hello";
    auto pName = PyUnicode_DecodeFSDefault("hello");  // 将模块名类型转为python对象字符串
    auto pModule = PyImport_Import(pName);            // 导入模块
    Py_DECREF(pName);                                 // 释放对象pName的gc计数器
    if (pModule != NULL) {
        auto res_call_func_example = call_func_example(pModule,123);
        if (res_call_func_example) {
            printf("res_call_func_example > 0");
            Py_XDECREF(pModule);
            return res_call_func_example;
        }
        auto res_call_class_example = call_class_example(pModule);
        if (res_call_class_example) {
            printf("res_call_class_example > 0");
            Py_XDECREF(pModule);
            return res_call_class_example;
        }
        auto res_call_func_example_withexception = call_func_example(pModule,-123);
        if (res_call_func_example_withexception) {
            printf("res_call_func_example > 0");
            Py_XDECREF(pModule);
            return res_call_func_example;
        }
    } else {
        PyErr_Print();  // 捕获错误,并打印
        fprintf(stderr, "Failed to load Module\"%s\"\n", Module_Name);
        Py_XDECREF(pModule);
        return 1;
    }
    Py_XDECREF(pModule);
    return 0;
}

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int status;
    status = init_py(argv[0], "env/", NULL, false);
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