#include "DemoPythonServer.h"
#include "RunPandasImp.h"
#include "python3.8/Python.h"


using namespace std;

DemoPythonServer g_app;

/////////////////////////////////////////////////////////////////
void
DemoPythonServer::initialize()
{
    //initialize application here:
    //...

    addServant<RunPandasImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".RunPandasObj");
}
/////////////////////////////////////////////////////////////////
void
DemoPythonServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('/root/PythonScript')");
        PyEval_InitThreads();
        Py_BEGIN_ALLOW_THREADS
            g_app.main(argc, argv);
            g_app.waitForShutdown();
        Py_END_ALLOW_THREADS
        Py_FinalizeEx();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
