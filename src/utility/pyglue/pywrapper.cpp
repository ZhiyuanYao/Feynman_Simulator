/*      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *      
 *      Author: 
 *      Matias Fontanini
 * 
 */

#include "utility/utility.h"
#include "utility/complex.h"
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include "pywrapper.h"

using std::string;

namespace Python {
Object::Object()
{
}

Object::Object(PyObject* obj)
    : py_obj(make_pyshared(obj))
{
}

void Object::Print()
{
    PyObject_Print(py_obj.get(), stdout, 0);
}

std::string Object::PrettyString()
{
    pyunique_ptr str(PyObject_Repr(py_obj.get()));
    MakeSureNoError();
    return PyString_AsString(str.get());
}

Python::Object::pyshared_ptr Object::make_pyshared(PyObject* obj)
{
    return pyshared_ptr(obj, [](PyObject* obj) { Py_XDECREF(obj); });
}

void Object::reset_pyshared(PyObject* obj)
{
    py_obj.reset(obj, [](PyObject* obj) { Py_XDECREF(obj); });
}

void Object::EvalScript(const std::string& script)
{
    pyunique_ptr main(PyImport_AddModule("__main__"));
    PyObject* global = PyModule_GetDict(main.get()); //borrowed reference
    pyunique_ptr local(PyDict_New());
    PyObject* result = PyRun_String(script.c_str(), Py_eval_input,
                                    global, local.get());
    MakeSureNoError();
    reset_pyshared(result);
}

void Object::LoadModule(const string& script_path)
{
    char arr[] = "path";
    PyObject* path(PySys_GetObject(arr));
    string base_path("."), file_path;
    size_t last_slash(script_path.rfind("/"));
    if (last_slash != string::npos) {
        if (last_slash >= script_path.size() - 2)
            ABORT("Invalid script path");
        base_path = script_path.substr(0, last_slash);
        file_path = script_path.substr(last_slash + 1);
    }
    else
        file_path = script_path;
    if (file_path.rfind(".py") == file_path.size() - 3)
        file_path = file_path.substr(0, file_path.size() - 3);
    pyunique_ptr pwd(PyString_FromString(base_path.c_str()));

    PyList_Append(path, pwd.get());
    /* We don't need that string value anymore, so deref it */
    reset_pyshared(PyImport_ImportModule(file_path.c_str()));
    MakeSureNoError();
}

PyObject* Object::load_function(const std::string& name)
{
    PyObject* obj(PyObject_GetAttrString(py_obj.get(), name.c_str()));
    MakeSureNoError();
    return obj;
}

Object Object::CallFunction(const std::string& name)
{
    pyunique_ptr func(load_function(name));
    PyObject* ret(PyObject_CallObject(func.get(), 0));
    MakeSureNoError();
    return { ret };
}

Object Object::GetAttr(const std::string& name)
{
    PyObject* obj(PyObject_GetAttrString(py_obj.get(), name.c_str()));
    MakeSureNoError();
    return { obj };
}

bool Object::HasAttr(const std::string& name)
{
    try {
        GetAttr(name);
        return true;
    }
    catch (ERRORCODE e) {
        return false;
    }
}

void Initialize()
{
    Py_Initialize();
}

void Finalize()
{
    Py_Finalize();
}

void ClearError()
{
    PyErr_Clear();
}

void PrintError()
{
    PyErr_Print();
}

void MakeSureNoError()
{
    if (PyErr_Occurred()) {
        PrintError();
        ABORT("Python fatal error!");
    }
}

void PrintPyObject(PyObject* obj)
{
    PyObject_Print(obj, stdout, 0);
}

// Allocation methods

PyObject* CastToPyObject(const std::string& str)
{
    return PyString_FromString(str.c_str());
}

PyObject* CastToPyObject(const std::vector<char>& val, size_t sz)
{
    return PyByteArray_FromStringAndSize(val.data(), sz);
}

PyObject* CastToPyObject(const std::vector<char>& val)
{
    return CastToPyObject(val, val.size());
}

PyObject* CastToPyObject(const char* cstr)
{
    return PyString_FromString(cstr);
}

PyObject* CastToPyObject(bool value)
{
    return PyBool_FromLong(value);
}

PyObject* CastToPyObject(real num)
{
    return PyFloat_FromDouble(num);
}

PyObject* CastToPyObject(const Complex& num)
{
    return PyComplex_FromDoubles(num.Re, num.Im);
}

bool is_py_int(PyObject* obj)
{
    return PyInt_Check(obj);
}

bool is_py_float(PyObject* obj)
{
    return PyFloat_Check(obj);
}

bool Convert(PyObject* obj, std::string& val)
{
    if (!PyString_Check(obj))
        return false;
    val = PyString_AsString(obj);
    return true;
}

bool Convert(PyObject* obj, std::vector<char>& val)
{
    if (!PyByteArray_Check(obj))
        return false;
    if (val.size() < (size_t)PyByteArray_Size(obj))
        val.resize(PyByteArray_Size(obj));
    std::copy(PyByteArray_AsString(obj),
              PyByteArray_AsString(obj) + PyByteArray_Size(obj),
              val.begin());
    return true;
}

/*bool Convert(PyObject *obj, Py_ssize_t &val) {
    return GenericConvert<Py_ssize_t>(obj, is_py_int, PyInt_AsSsize_t, val);
}*/
bool Convert(PyObject* obj, bool& value)
{
    if (obj == Py_False)
        value = false;
    else if (obj == Py_True)
        value = true;
    else
        return false;
    return true;
}

bool Convert(PyObject* obj, real& val)
{
    return GenericConvert<real>(obj, is_py_float, PyFloat_AsDouble, val);
}

/*bool Convert(PyObject *obj, size_t &val) {
    return GenericConvert<size_t>(obj, is_py_int, PyInt_AsLong, val);
}*/

bool Convert(PyObject* obj, Complex& val)
{
    if (!PyComplex_Check(obj))
        return false;
    val = Complex(PyComplex_RealAsDouble(obj),
                  PyComplex_ImagAsDouble(obj));
    return true;
}
}
