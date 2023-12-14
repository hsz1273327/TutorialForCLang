#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <exception>
#include <filesystem>
#include <map>
#include "scope_guard.hpp"

// 模块的部分
// C中全局变量
int numargs = 0;

// 包装为模块
// python模块中获取numargs的值
static PyObject* emb_numargs(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}
// python模块中设置numargs的值
static PyObject* emb_setnumargs(PyObject* self, PyObject* args) {
    int i;
    auto ok = PyArg_ParseTuple(args, "i:numargs", &i);
    if (!ok) {
        return NULL;
    }
    numargs = i;
    Py_RETURN_NONE;
}
// 注册python模块中的方法
static PyMethodDef EmbMethods[] = {{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
                                   {"setnumargs", emb_setnumargs, METH_VARARGS, "Set the number of numargs received by the process."},
                                   {NULL, NULL, 0, NULL}};
// 构造python模块
static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};
// 初始化模块的函数
PyObject* PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}

// 应用部分
class AppException : public std::runtime_error {
   public:
    AppException(const char* err) : std::runtime_error(err) {}
};

/**
 * @fn BINARY_VECTOR_P VEC_init(float,float)
 * @brief 相对通用的初始化python解释器函数
 * @param[programname]  应用名.
 * @param[envpath]  虚拟环境路径.为NULL则不使用虚拟环境;为相对路径则从应用所在文件夹开始查找
 * @param[pymodulepath]  python模块的额外查找路径.为NULL则将当前应用所在文件夹加入;为相对路径则从应用所在文件夹开始查找
 * @param[pyhomepath]  设置python_home.为相对路径则从应用所在文件夹开始查找.如果`isolated`为true则不能为NULL
 * @param[tabs] 需要预先加载的模块名模块初始化函数信息
 * @param[isolated]  是否使用隔离配置初始化python解释器
 * @param[debugmod]  是否打印debug用的文本
 * @return void
 * @exception <AppException> { 应用级别异常 }
 */
void init_py(char* programname,
             char* envpath,
             char* pymodulepath,
             char* pyhomepath,
             const std::map<std::string, PyObject* (*)(void)>* tabs,
             bool isolated,
             bool debugmod) {
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

    // 初始化
    if (isolated) {
        // 预设置
        PyStatus statusp;
        PyPreConfig preconfig;
        PyPreConfig_InitIsolatedConfig(&preconfig);
        preconfig.utf8_mode = 1;
        statusp = Py_PreInitialize(&preconfig);
        if (PyStatus_Exception(statusp)) {
            Py_ExitStatusException(statusp);
        }
    }
    // 基本设置
    //  隔离模式
    PyStatus status;
    PyConfig config;
    if (isolated) {
        PyConfig_InitIsolatedConfig(&config);
    } else {
        PyConfig_InitPythonConfig(&config);
    }
    auto guard_config = sg::make_scope_guard([&config]() noexcept {
        PyConfig_Clear(&config);
        printf("python init config clear\n");
    });
    // 设置python程序名
    status = PyConfig_SetString(&config, &config.program_name, program);
    if (PyStatus_Exception(status)) {
        throw AppException("Fatal error: InitPythonConfig set program_name get error");
    }

    // 设置python_home
    wchar_t* pyhome;
    auto guard_pyhome = sg::make_scope_guard([&pyhome, &pyhomepath]() noexcept {
        if (pyhomepath != NULL) {
            PyMem_RawFree(pyhome);
        }
    });
    if (pyhomepath != NULL) {
        std::filesystem::path pyhome_dir = pyhomepath;
        if (pyhome_dir.is_relative()) {
            pyhome_dir = std::filesystem::absolute(pyhome_dir);
        }
        const char* _pyhome_dir_name = nullptr;
        {
            auto _pyhome_dir_name_str = pyhome_dir.string();
            _pyhome_dir_name = _pyhome_dir_name_str.c_str();
        }

        pyhome = Py_DecodeLocale(_pyhome_dir_name, NULL);
        if (pyhome == NULL) {
            throw AppException("Fatal error: cannot decode pyhome");
        } else {
            if (debugmod) {
                printf("use python_home %s \n", pyhome);
            }
        }
        status = PyConfig_SetString(&config, &config.home, pyhome);
        if (PyStatus_Exception(status)) {
            throw AppException("Fatal error: InitPythonConfig set home get error");
        }
    } else {
        if (isolated) {
            throw AppException("Fatal error: isolated config must set pyhomepath");
        }
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

    // 提前初始化模块`emb`
    if (tabs != NULL) {
        for (const auto& [key, value] : *tabs) {
            PyImport_AppendInittab(key.c_str(), value);
        }
    }

    // 初始化python解释器
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

int finalize_py() {
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    printf("finalize_py ok\n");
    return 0;
}

int main(int argc, char* argv[]) {
    // 初始化python解释器
    try {
        numargs = 10;
        std::map<std::string, PyObject* (*)(void)> tabs{{"emb", PyInit_emb}};
        // init_py(argv[0], (char*)"env", NULL, (char*)"/Users/mac/micromamba/envs/py3.10", NULL, true, true);
        init_py(argv[0], (char*)"env", NULL, (char*)"/Users/mac/micromamba/envs/py3.10", &tabs, true, true);
        PyRun_SimpleString("import emb;print('Number of arguments', emb.numargs())");
        PyRun_SimpleString("emb.setnumargs(20);print('Number of arguments', emb.numargs())");
        printf("get numargs now is %d\n", numargs);
    } catch (const AppException& ex) {
        fprintf(stderr, "%s", ex.what());
        return 1;
    }
    return finalize_py();
}
