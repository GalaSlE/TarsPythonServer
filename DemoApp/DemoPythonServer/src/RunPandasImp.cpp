#include "RunPandasImp.h"
#include "servant/Application.h"
#include "python3.8/Python.h"


using namespace std;

//////////////////////////////////////////////////////
void RunPandasImp::initialize()
{
    //initialize servant here:
    //...

}

//////////////////////////////////////////////////////
void RunPandasImp::destroy()
{
}

class PythonThreadLocker
{
    PyGILState_STATE state;
public:
    PythonThreadLocker() : state(PyGILState_Ensure())
    {}
    ~PythonThreadLocker() {
        PyGILState_Release(state);
    }
};

/////////////////////////////////////////////////////////////////
int CallPythonFunction(const string &request, string &funcName, string &response)
{

    try
    {   
        TLOG_DEBUG(funcName.c_str() << "input params: " << request.c_str() << endl);
        PyObject *pModule, *pFunc;
        PyObject *pArgs, *pValue;
        pModule = PyImport_ImportModule("demo");

        if (pModule != nullptr) {
            pFunc = PyObject_GetAttrString(pModule,  "function_gateway");

            if (pFunc && PyCallable_Check(pFunc)) {
                pArgs = PyTuple_New(2);
                PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", request.c_str()));
                PyTuple_SetItem(pArgs, 1, Py_BuildValue("s", funcName.c_str()));
                pValue = PyObject_CallObject(pFunc, pArgs);

                Py_DECREF(pArgs);
                if (pValue != nullptr) {
                    const char *nResponse;
                    const char *nErrorMessage;
                    PyArg_ParseTuple(pValue, "sz", &nResponse, &nErrorMessage);
                    response = nResponse;
                    TLOG_DEBUG(funcName.c_str() << " Call response: " << nResponse << "input params: " << request.c_str() << endl);
                    if (nErrorMessage != nullptr) {
                        TLOG_INFO(funcName.c_str()  << request.c_str() << " get normal error message: " << nErrorMessage << endl);
                    };
                    Py_DECREF(pValue);
                }
                else {
                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);

                    PyObject *ptype, *pvalue, *ptraceback;
                    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                    PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

                    /* Get a full traceback */
                    PyObject *pyth_module, *pyth_func;
                    pyth_module = PyImport_ImportModule("traceback");

                    pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
                    if (pyth_func && PyCallable_Check(pyth_func)) {
                        PyObject *pyth_val, *pystr;

                        pyth_val = PyObject_CallFunctionObjArgs(pyth_func, ptype, pvalue, ptraceback, NULL);

                        pystr = PyObject_Str(pyth_val);
                        const char* full_backtrace = PyUnicode_AsUTF8(pystr);
                        TLOG_ERROR(funcName.c_str() << " Input: " << request.c_str() << " get unexpected error traceback: " << full_backtrace << endl);
                        PyErr_Restore(ptype, pvalue, ptraceback);
                        Py_DECREF(pyth_val);
                        Py_DECREF(pystr);
                    }
                    Py_DECREF(pyth_module);
                    Py_DECREF(pyth_func);

                    Py_DECREF(ptype);
                    Py_DECREF(pvalue);
                    Py_DECREF(ptraceback);
                    response =  R"({"code":1,"msg":"Python script met unexpected error, please check log for detail."})";
                    return 1;
                }
            }
            else {
                if (PyErr_Occurred())
                    PyErr_Print();
                TLOG_ERROR("Cannot find function function_gateway" << endl);
            }
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
        }
        else {
            PyErr_Print();
            TLOG_ERROR("Failed to load demo Module" << endl);
            return 1;
        }
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
        TLOG_ERROR(e.what() << endl);
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
        TLOG_ERROR("unknown exception." << endl);
    }
    return 0;
}


int RunPandasImp::test(const string &request, string &response, tars::TarsCurrentPtr current)
{
    PythonThreadLocker locker;
    string funcName = "example_group_by_mean";
    int result = CallPythonFunction(request, funcName, response);
    return result;
}