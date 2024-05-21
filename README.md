# Tars Python Server - Method for Embedding Python Scripts in TarsCpp

This project is mainly used to implement the use of Python scripts on the TarsCpp server.

To embed Python scripts in C++, the mainstream method is to turn the Python program into a text-based dynamic link library, and call the defined functions within the C/C++ program. Essentially, this starts a `Python` interpreter within C++, which executes the Python-related code, releases resources after execution, and achieves the purpose of calling the function. Due to the presence of the GIL (Global Interpreter Lock), this approach to some extent causes the Tars service to lose the advantage of multi-threaded CPU usage. However, this can be mitigated by deploying multiple nodes. The biggest advantage of this approach is that Python scripts can be quickly integrated into the Tars framework, enabling rapid development.

## Compiling the Python Environment

### Installing python-dev and Related Dependencies

The official TarsCpp-node image does not have python-dev and pip installed, so they need to be installed via apt-get.

```bash
apt-get update
apt-get install python3-dev -y
apt-get install python3-pip -y
```

Modules that the Python script depends on need to be fully installed in the environment via pip.

```bash
python3 -m pip install pandas
```

Copy the Python script that needs to be executed into the Tars environment.

The above environment can be directly packaged into an image using a Dockerfile, which serves as the Tars-node image for deploying Python services.

### CMake Configuration

Link the Python3 interpreter, compiler, and development environment in CMakeLists.txt.

```cmake
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
aux_source_directory(. DIR_SRC)

target_link_libraries(TestServer ${Python3_LIBRARIES})
```

Where:

- The first parameter of `target_link_libraries` is the target location, which in this sample code is the current server name TestServer.
- The `find_package` method can find the Python3 interpreter, compiler, and development environment (including include files and libraries).

It provides the following components:

- `Interpreter`: Finds the Python3 interpreter.
- `Compiler`: Finds the Python3 compiler. Only provided when using IronPython.
- `Development`: Finds the development environment suite (including include directories and libraries).
- `NumPy`: Finds the include directory for NumPy.

If no `COMPONENT` is specified, the default is `Interpreter`.

To ensure that the versions of all components `Interpreter, Compiler, Development, NumPy` are consistent, all components need to be specified simultaneously.

## Server Code Explanation

Define the interface functions and data structures in the Tars file.

Include Python in the code:

```cpp
#include "python3.8/Python.h"
```

Because the Tars C++ framework service is a single-process multi-threaded RPC system, when generating the Servant object, the framework will start the corresponding number of threads for this Servant. In a C++ multi-threaded environment, directly calling API operations on the Python interpreter, especially when introducing third-party modules, will definitely cause a core dump. This is because the Python interpreter is not completely thread-safe. To support multi-threaded Python programs, there is a global lock called the global interpreter lock or GIL, which must be held by the current thread before it can safely access Python objects. Without the lock, even the simplest operations can cause problems in multi-threaded programs.

Therefore, we need to create the interpreter when the server starts. No matter how many threads call Python, the Python interpreter can only be initialized once. Each time a thread wants to call Python, it must acquire the GIL.

When the server starts, the path of the Python script needs to be set, and thread support enabled.

Then, in the Servant, define the specific code that calls Python functions through the Python C API, using macros to control the acquisition and release of thread locks.

The example code in this project reads an Excel file using Python pandas and returns the file content.

## Deploying the Tars Service

Compile and deploy the TarsCpp server in the usual way. The service needs to be deployed on a Tars node started with an image that has the Python dependencies installed. Additionally, the Python script needs to be placed in a specified location on that node.