#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <format>
#include "crow_all.h"

void init_py(char* programname, char* envpath, char* pymodulepath, bool debugmod) {
    wchar_t* program;
    wchar_t* env_dir_name;
    wchar_t* pymodule_dir_name;
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        throw "Fatal error: cannot decode programname";
    }

    // 初始化python设置
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        throw "Fatal error: InitPythonConfig set program_name get error";
    }
    // 加载默认配置
    status = PyConfig_Read(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        throw "Fatal error: PyConfig_Read get error";
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
        throw "Fatal error: cannot decode pymodule_dir_name";
    } else {
        if (debugmod) {
            printf("pymodule_dir %s \n", _pymodule_dir_name);
        }
    }
    config.module_search_paths_set = 1;
    status = PyWideStringList_Append(&config.module_search_paths, pymodule_dir_name);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        throw "Fatal error: InitPythonConfig set module_search_paths get error";
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
            throw "Fatal error: cannot decode _env_dir_name";
        } else {
            if (debugmod) {
                printf("use virtual environments %s \n", _env_dir_name);
            }
        }
        status = PyConfig_SetString(&config, &config.prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            PyConfig_Clear(&config);
            throw "Fatal error: InitPythonConfig set prefix get error";
        }
        status = PyConfig_SetString(&config, &config.exec_prefix, env_dir_name);
        if (PyStatus_Exception(status)) {
            PyConfig_Clear(&config);
            throw "Fatal error: InitPythonConfig set exec_prefix get error";
        }
    }
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        if (PyStatus_IsExit(status)) {
            // return status.exitcode;
            throw "Fatal error: PyStatus_IsExit";
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
}

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}

PyObject* init_py_module(char* module_name) {
    auto pName = PyUnicode_DecodeFSDefault(module_name);  // 将模块名类型转为python对象字符串
    auto pModule = PyImport_Import(pName);                // 导入模块
    Py_DECREF(pName);                                     // 释放对象pName的gc计数器
    return pModule;
}

PyObject* init_py_api(char* module_name) {
    const char* Func_Name = "apply";
    auto pFunc = PyObject_GetAttrString(pModule, Func_Name);
    if (pFunc && PyCallable_Check(pFunc)) {
        return pFunc
    } else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }  // 捕获错误,并打印
        Py_XDECREF(pFunc);
        auto errmsg = std::format("Cannot find function {}", Func_Name).c_str();
        throw errmsg;
    }
    return NULL
}
std::vector<double> call_api_func(PyObject* pFunc, long longd) {
    // 线程锁
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    /* Perform Python actions here. */
    result = CallSomeFunction();
    /* evaluate result or handle exception */

    /* Release the thread. No Python API allowed beyond this point. */
    PyGILState_Release(gstate);



    // 构造参数args
    auto d = PyLong_FromLong(longd);
    auto args = PyTuple_Pack(1, d);

    // 调用函数对象
    auto result = PyObject_Call(pFunc, args, NULL);
    // 参数gc计数器回收
    Py_DECREF(args);
    Py_DECREF(d);
    // 提取结果数据
    if (result != NULL) {
        auto is_list = PyList_Check(result);
        if (!is_list) {
            auto errmsg = std::format("Call {} get result not list", Func_Name).c_str();
            Py_XDECREF(result);
            throw errmsg;
        }
        auto size = PyTuple_Size(result);

    } else {
        PyErr_Print();
        auto errmsg = std::format("Call {} failed", Func_Name).c_str();
        Py_XDECREF(pFunc);
        throw errmsg;
    }
    // 返回值计数回收
    Py_XDECREF(result);
}

// int finalize_py_module(PyObject * pModule){
//  if (pModule != NULL) {
//         auto res_call_func_example = call_func_example(pModule,123);
//         if (res_call_func_example) {
//             printf("res_call_func_example > 0");
//             Py_XDECREF(pModule);
//             return res_call_func_example;
//         }
//         auto res_call_class_example = call_class_example(pModule);
//         if (res_call_class_example) {
//             printf("res_call_class_example > 0");
//             Py_XDECREF(pModule);
//             return res_call_class_example;
//         }
//         auto res_call_func_example_withexception = call_func_example(pModule,-123);
//         if (res_call_func_example_withexception) {
//             printf("res_call_func_example > 0");
//             Py_XDECREF(pModule);
//             return res_call_func_example;
//         }
//     } else {
//         PyErr_Print();  // 捕获错误,并打印
//         fprintf(stderr, "Failed to load Module\"%s\"\n", Module_Name);
//         Py_XDECREF(pModule);
//         return 1;
//     }
//     Py_XDECREF(pModule);
// }

int main(int argc, char* argv[]) {
    // 初始化python解释器
    try {
        init_py(argv[0], "env/", NULL, false);
    } catch (char* msg) {
        fprintf(stderr, msg);
        return 1
    }
    // 加载模型
    auto P_Module_m = init_py_module("m");

    // 加载接口
    try{
        auto P_Func_m = init_py_api(P_Module_m)
    }catch(char* msg){
        fprintf(stderr, msg);
        return 1
    }
    // http接口逻辑
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
    // 回收模型
    Py_XDECREF(P_Module_m);
    // 回收python解释器
    return finalize_py();
}
