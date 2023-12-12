/* $Id$ */
#ifndef HELLOWORLD_WRAP_H
#define HELLOWORLD_WRAP_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

// static PyObject* emb_numargs(PyObject* self, PyObject* args);
// static PyMethodDef EmbMethods[];
// static PyModuleDef EmbModule;
static PyObject* PyInit_emb(void);

#endif