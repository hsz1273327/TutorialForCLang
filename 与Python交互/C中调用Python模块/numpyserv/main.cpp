#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <exception>
#include <filesystem>
#include <format>
#include <vector>
#include "crow_all.h"
#include "scope_guard.hpp"

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

int finalize_py(PyObject* P_Module_m, PyObject* P_Func_m) {
    printf("finalize_py\n");
    Py_XDECREF(P_Module_m);  // 释放对象pName的gc计数器
    printf("Py_XDECREF(P_Module_m) ok\n");
    Py_XDECREF(P_Func_m);  // 释放对象pName的gc计数器
    printf("Py_XDECREF(P_Func_m) ok\n");
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
    if (pModule == NULL) {
        throw AppException(std::format("module {} not found", Module_Name).c_str());
    }
    return pModule;
}

PyObject* init_py_api(PyObject* P_Module_m, char* Func_Name) {
    auto pFunc = PyObject_GetAttrString(P_Module_m, Func_Name);
    if (pFunc == NULL) {
        throw AppException(std::format("Api {} not found", Func_Name).c_str());
    }
    if (!PyCallable_Check(pFunc)) {
        if (PyErr_Occurred())  // 捕获错误,并打印
            PyErr_Print();
        throw AppException(std::format("Api {} not Callable", Func_Name).c_str());
    }
    return pFunc;
}

int main(int argc, char* argv[]) {
    // 初始化python解释器
    try {
        init_py(argv[0], (char *)"env/", NULL, false);
    } catch (const AppException& ex) {
        fprintf(stderr, ex.what());
        return 1;
    }
    PyObject* P_Module_m;
    PyObject* P_Func_m;
    try {
        // 加载模型
        P_Module_m = init_pymodule((char *)"numpyprocess");
        // 加载接口
        P_Func_m = init_py_api(P_Module_m, (char *)"apply");
    } catch (const AppException& ex) {
        fprintf(stderr, ex.what());
        return 1;
    }
    // http接口逻辑
    crow::SimpleApp app;

    CROW_ROUTE(app, "/random_array")
    ([&P_Func_m](const crow::request& req) {
        auto d = req.url_params.get("d");
        if (!d) {
            return crow::response(crow::status::BAD_REQUEST);  // same as crow::response(400)
        }
        auto dl = atol(d);
        if (dl < 1) {
            return crow::response(crow::status::BAD_REQUEST);  // same as crow::response(400)
        }
        CROW_LOG_INFO << std::format("Call api get d {}", d);
        // 开始执行python调用
        // 构造参数args
        auto n = PyLong_FromLong(dl);
        auto args = PyTuple_Pack(1, n);
        auto guard_pFunc_args = sg::make_scope_guard([&args, &n]() noexcept {
            // 回收args参数
            Py_DECREF(args);
            Py_DECREF(n);
            printf("Py_DECREF FUNC (args) ok\n");
        });
        CROW_LOG_INFO << "Call api params make ok";
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
        auto result = PyObject_Call(P_Func_m, args, NULL);
        CROW_LOG_INFO << "PyObject_Call ok";
        auto guard_pFunc_result = sg::make_scope_guard([&result]() noexcept {
            // 回收kwargs参数
            Py_XDECREF(result);
            CROW_LOG_INFO << "Py_DECREF FUNC (result) ok";
        });
        /* evaluate result or handle exception */
        if (result != NULL) {
            auto is_list = PyList_Check(result);
            if (!is_list) {
                // throw AppException(std::format("Call {} get result not list", Func_Name).c_str());
                CROW_LOG_ERROR << "Call api get result not list";
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
            auto size = PyList_Size(result);
            std::vector<double> arr = {};
            Py_ssize_t i;
            for (i = 0; i < size; i++) {
                auto item = PyList_GetItem(result, i);
                arr.push_back(PyFloat_AsDouble(item));
            }
            // 结束python调用,转换结果
            crow::json::wvalue x({{"status", "ok"}});
            x["result"] = arr;
            return crow::response(x);

        } else {
            PyErr_Print();
            CROW_LOG_ERROR << "Call api failed";
            return crow::response(crow::status::INTERNAL_SERVER_ERROR);
        }
    });
    app.loglevel(crow::LogLevel::Info).port(18080).multithreaded().run();
    return finalize_py(P_Module_m, P_Func_m);
}
