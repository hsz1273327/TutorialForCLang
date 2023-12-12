#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "helloworld.h"
/* Return the number of arguments of the application command line */
static PyObject* emb_numargs(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}

static PyMethodDef EmbMethods[] = {{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."}, {NULL, NULL, 0, NULL}};

static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};

static PyObject* PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}
