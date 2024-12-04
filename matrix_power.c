#include <stdlib.h>
#include <Python.h>
#include "./safe_allocations.h"

void Cmatrix_power(size_t size, double matrix[size][size], int pow) {
    if (pow == 0) {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                matrix[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
        return;
    }

    double(*product)[size] = safe_malloc(size * size *sizeof(double));
    double(*temp)[size] = safe_malloc(size * size * sizeof(double));

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            temp[i][j] = 0;
        }
    }

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            product[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int step = 0; step < pow; ++step) {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                for (size_t k = 0; k < size; ++k) {
                    temp[i][j] += product[i][k] * matrix[k][j];
                }
            }
        }

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                product[i][j] = temp[i][j];
                temp[i][j] = 0;
            }
        }

    }

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            matrix[i][j] = product[i][j];
        }
    }

    free(product);
    free(temp);
}


static PyObject* foreign_matrix_power(PyObject* self, PyObject* args) {
    PyObject* input_matrix;
    int power;

    if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &input_matrix, &power)) {
        return NULL;
    }

    if (power < 0) {
        PyErr_SetString(PyExc_ValueError, "Power must be a positive integer");
        return NULL;
    }

    size_t size = PyList_Size(input_matrix);
    if (size == 0) {
        PyErr_SetString(PyExc_ValueError, "Matrix cannot be empty");
        return NULL;
    }

    double(*matrix)[size] = malloc(size * size * sizeof(double));
    if (!matrix) {
        PyErr_NoMemory();
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        PyObject* line = PyList_GetItem(input_matrix, i);
        if (!PyList_Check(line) || PyList_Size(line) != size) {
            free(matrix);
            PyErr_SetString(PyExc_ValueError, "Matrix must be square");
            return NULL;
        }

        for (size_t j = 0; j < size; j++) {
            PyObject* item = PyList_GetItem(line, j);
            matrix[i][j] = PyFloat_AsDouble(item);
            if (PyErr_Occurred()) {
                free(matrix);
                PyErr_SetString(PyExc_ValueError,
                                "Matrix contains non-numeric values");
                return NULL;
            }
        }
    }

    Cmatrix_power(size, matrix, power);

    PyObject* output_matrix = PyList_New(size);
    for (size_t i = 0; i < size; i++) {
        PyObject* line = PyList_New(size);
        for (size_t j = 0; j < size; j++) {
            PyList_SetItem(line, j, PyFloat_FromDouble(matrix[i][j]));
        }
        PyList_SetItem(output_matrix, i, line);
    }

    free(matrix);
    return output_matrix;
}

static PyMethodDef methods[] = {
    {"foreign_matrix_power", foreign_matrix_power, METH_VARARGS,
     "Raise a matrix to a positive power."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef matrix_power = {PyModuleDef_HEAD_INIT, "matrix_power",
                                          "Powering the matrix", -1, methods};

PyMODINIT_FUNC PyInit_matrix_power() { return PyModule_Create(&matrix_power); }