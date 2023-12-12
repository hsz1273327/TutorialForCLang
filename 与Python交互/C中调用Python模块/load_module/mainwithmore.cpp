#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>
#include <format>
#include <exception>
#include <vector>
#include "scope_guard.hpp"

class AppException : public std::runtime_error{
public:
    AppException(const char* err) : std::runtime_error(err) {}       
};

void init_py(char* programname, char* envpath, char* pymodulepath, bool debugmod) {
    wchar_t* program;
    wchar_t* env_dir_name;
    wchar_t* pymodule_dir_name;
    program = Py_DecodeLocale(programname, NULL);
    if (program == NULL) {
        throw AppException("Fatal error: cannot decode programname");
    }

    // 初始化python设置
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    auto guard = sg::make_scope_guard([&config]() noexcept {
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
    if (envpath != NULL) {
        std::filesystem::path env_dir = envpath;
        if (env_dir.is_relative()) {
            env_dir = std::filesystem::absolute(env_dir);
        }
        auto _env_dir_name = env_dir.string().c_str();
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
    PyMem_RawFree(pymodule_dir_name);
    if (envpath != NULL) {
        PyMem_RawFree(env_dir_name);
    }
    PyMem_RawFree(program);
    if (debugmod) {
        PyRun_SimpleString("import sys;print(sys.path);print(sys.prefix)");
    }
}

PyObject* init_pymodule(char* Module_Name) {
    auto pName = PyUnicode_DecodeFSDefault("hello");  // 将模块名类型转为python对象字符串
    auto guard = sg::make_scope_guard([&pName]() noexcept {
        Py_DECREF(pName);  // 释放对象pName的gc计数器
    });
    auto pModule = PyImport_Import(pName);  // 导入模块
    return pModule;
}

void call_func_example(PyObject* pModule, long longx) {
    // 在模块中找到函数名为`call_with_params_and_return`的函数,将这个函数对象提出来
    const char* Func_Name = "call_with_params_and_return";
    auto pFunc = PyObject_GetAttrString(pModule, Func_Name);
    auto guard_pFunc = sg::make_scope_guard([&pFunc]() noexcept {
        Py_XDECREF(pFunc);  // 释放对象pName的gc计数器
        printf("Py_XDECREF(pFunc) ok\n");
    });
    /* pFunc is a new reference */
    if (pFunc && PyCallable_Check(pFunc)) {
        // pFunc存在且为可调用对象,则执行调用
        // 构造参数args
        auto x = PyLong_FromLong(longx);
        auto y = PyFloat_FromDouble(0.25);
        auto args = PyTuple_Pack(2, x, y);
        auto guard_pFunc_args = sg::make_scope_guard([&args, &x, &y]() noexcept {
            // 回收args参数
            Py_DECREF(args);
            Py_DECREF(x);
            Py_DECREF(y);
            printf("Py_DECREF FUNC (args) ok\n");
        });
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
        auto guard_pFunc_kwargs = sg::make_scope_guard([&kwargs, &z]() noexcept {
            // 回收kwargs参数
            Py_DECREF(kwargs);
            Py_DECREF(z);
            printf("Py_DECREF FUNC (kwargs) ok\n");
        });
        // 调用函数对象
        auto result = PyObject_Call(pFunc, args, kwargs);
        auto guard_pFunc_result = sg::make_scope_guard([&result]() noexcept {
            // 回收kwargs参数
            Py_XDECREF(result);
            printf("Py_DECREF FUNC (result) ok\n");
        });

        // 提取结果数据
        if (result != NULL) {
            auto is_tuple = PyTuple_Check(result);
            if (!is_tuple) {
                throw AppException(std::format("Call {} get result not tuple", Func_Name).c_str());
            }
            auto size = PyTuple_Size(result);
            if (size != 2) {
                throw AppException(std::format("Call {} get result len not 2", Func_Name).c_str());
            }
            auto result1 = PyTuple_GetItem(result, 0);
            auto result1_string = PyUnicode_AsUTF8(result1);
            printf("Result1 of call: %s\n", result1_string);
            auto result2 = PyTuple_GetItem(result, 1);
            auto result2_string = PyUnicode_AsUTF8(result2);
            printf("Result2 of call: %s\n", result2_string);
        } else {
            PyErr_Print();
            throw AppException(std::format("Call {} failed", Func_Name).c_str());
            
        }
    } else {
        if (PyErr_Occurred())  // 捕获错误,并打印
            PyErr_Print();
        fprintf(stderr, "Cannot find function \"%s\"\n", Func_Name);
    }
}

void call_class_example(PyObject* pModule) {
    // 在模块中找到类名为`PyVector`的类,将这个类对象提出来
    const char* Claz_Name = "PyVector";
    auto pClaz = PyObject_GetAttrString(pModule, Claz_Name);
    auto guard_pClaz = sg::make_scope_guard([&pClaz]() noexcept {
        Py_XDECREF(pClaz);  // 释放对象pName的gc计数器
        printf("Py_XDECREF(pClaz) ok\n");
    });
    /* pClaz is a new reference */
    if (pClaz && PyCallable_Check(pClaz)) {
        // pClaz存在且为可调用的类对象,则执行调用
        // 构造参数args
        auto x = PyLong_FromLong(1.5);
        auto y = PyFloat_FromDouble(0.5);
        auto args = PyTuple_Pack(2, x, y);
        auto guard_pClaz_args = sg::make_scope_guard([&args, &x, &y]() noexcept {
            // 回收args参数
            Py_DECREF(args);
            Py_DECREF(x);
            Py_DECREF(y);
            printf("Py_DECREF pClaz (args) ok\n");
        });

        // 调用函数对象
        auto pObj = PyObject_Call(pClaz, args, NULL);
        auto guard_pObj = sg::make_scope_guard([&pObj]() noexcept {
            Py_XDECREF(pObj);  // 释放对象pName的gc计数器
            printf("Py_XDECREF(pObj) ok\n");
        });
        // 提取结果数据
        if (pObj != NULL) {
            const char* Method_Name = "calculate_mod";
            auto pMethod = PyObject_GetAttrString(pObj, Method_Name);
            auto guard_pMethod = sg::make_scope_guard([&pMethod]() noexcept {
                Py_XDECREF(pMethod);  // 释放对象pName的gc计数器
                printf("Py_XDECREF(pMethod) ok\n");
            });

            if (pMethod && PyCallable_Check(pMethod)) {
                auto margs = PyTuple_New(0);
                auto guard_pMethod_args = sg::make_scope_guard([&margs]() noexcept {
                    // 回收args参数
                    Py_DECREF(margs);
                    printf("Py_DECREF pMethod (args) ok\n");
                });
                auto result = PyObject_Call(pMethod, margs, NULL);
                auto guard_pMethod_result = sg::make_scope_guard([&result]() noexcept {
                    // 回收result参数
                    Py_XDECREF(result);
                    printf("Py_DECREF pMethod (result) ok");
                });
                if (result != NULL) {
                    auto result_double = PyFloat_AsDouble(result);
                    printf("Result of call: %f\n", result_double);
                } else {
                    PyErr_Print();
                    throw AppException(std::format("Call {} . {} failed", Claz_Name, Method_Name).c_str());
                }
            } else {
                if (PyErr_Occurred()) {
                    PyErr_Print();  // 捕获错误,并打印
                }
                throw AppException(std::format("Cannot find method {} . {}", Claz_Name, Method_Name).c_str());
            }
        } else {
            PyErr_Print();
            throw AppException(std::format("Call {} failed", Claz_Name).c_str());
        }
    } else {
        if (PyErr_Occurred()) {
            PyErr_Print();  // 捕获错误,并打印
        }
        throw AppException(std::format("Cannot find class {}", Claz_Name).c_str());
    }
}

void callpy() {
    // const char* Class_Name = "PyVector";
    // const char* Method_Name = "calculate_mod";
    // const char* Attr_X_Name = "x";
    // const char* Attr_Y_Name = "Y";
    // const char* Property_Name = "mod";
    char* Module_Name = "hello";
    auto pModule = init_pymodule(Module_Name);  // 导入模块
    auto guard_pModule = sg::make_scope_guard([&pModule]() noexcept {
        Py_XDECREF(pModule);  // 释放pModule
    });
    if (pModule != NULL) {
        call_func_example(pModule, 123);
        // auto res_call_func_example = call_func_example(pModule, 123);
        // if (res_call_func_example) {
        //     printf("res_call_func_example > 0");
        //     return res_call_func_example;
        // }
        call_class_example(pModule);
        call_func_example(pModule, -123);
        // auto res_call_func_example_withexception = call_func_example(pModule, -123);
        // if (res_call_func_example_withexception) {
        //     printf("res_call_func_example > 0");
        //     return res_call_func_example;
        // }
    } else {
        PyErr_Print();  // 捕获错误,并打印
        throw AppException(std::format("Failed to load Module {}", Module_Name).c_str());
    }
}

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // int status;
    try {
        // 初始化
        init_py(argv[0], "env/", NULL, false);
        // 开始执行python调用
        callpy();
    } catch (const AppException& ex) {
        fprintf(stderr, ex.what());
        return 1;
    }

    // 回收python解释器
    return finalize_py();
}