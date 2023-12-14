#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "embc.h"

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
PyMODINIT_FUNC PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}
