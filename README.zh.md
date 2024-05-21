# Tars Python Server - 在TarsCpp中嵌入python脚本的方法

本项目主要用于实现在TarsCpp服务端中使用python脚本。

想要实现在c++中嵌入python脚本，主流方法是将python程序变成文本形式的动态链接库，在c/c++程序中调用其中定义的函数。本质上是在 c++ 中启动了一个`python`解释器，由解释器对 python 相关的代码进行执行，执行完毕后释放资源，达到调用目的。由于GIL的存在，这种方式一定程度上使tars服务失去了多线程使用CPU的优势，但是这一点可以通过部署多个节点的方式来避免。这种方式最大的优势在于可以将python脚本迅速融入tars框架中，实现快读开发。

## 编译python环境

### 安装python-dev及相关依赖

tarscpp-node的官方镜像中没有安装python-dev和pip，需要通过apt-get进行安装。

```bash
apt-get update
apt-get install python3-dev -y
apt-get install python3-pip -y
```

python脚本中依赖的module需要通过pip在环境中安装完备。

```bash
python3 -m pip install pandas
```

将需要执行的python脚本复制到tars环境中。

以上环境可以直接使用dockerfile打包好镜像作为部署python服务的tars-node的镜像。

### cmake配置

在CMakeLists.txt中链接Python3的解释器、编译器和开发环境。

```bash
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
aux_source_directory(. DIR_SRC)

target_link_libraries(TestServer ${Python3_LIBRARIES})
```

其中

target_link_libraries的第一个参数是目标位置，在本样例代码中即为当前Server名称TestServer

`find_package`方法可以找到 Python3 的解释器、编译器和开发环境（包括 include 的文件和库文件）

其提供如下组件：

`Interpreter`：寻找 Python3 解释器

`Compiler`：寻找 Python3 编译器。仅使用 IronPython 时提供

`Development`：寻找开发环境套件（包含 include 目录和库文件）

`NumPy`：寻找 NumPy 的 include 目录

如果没有 `COMPONENT` 被指定，那么默认使用 `Interpreter`

为了确保所有组件 `Interperter，Compiler，Development，NumPy`的版本一致，需要同时指定所有的组件。

## Server代码解释

在tars文件中定好接口函数及数据结构。

在代码中include python

`#include "python3.8/Python.h"`

因为Tars C++框架服务是单进程多线程RPC系统。生成Servant对象时，框架将会为本Servant启动相应的线程数。 那这样在C++多线程环境下，直接调用 api操作 Python解释器，特别是在引入第三方module时，肯定会导致 core dump。这是因为 Python解释器不是完全线程安全的。为了支持多线程Python程序，有一个全局锁，称为 global interpreter lock 或者 GIL，在当前线程能够安全访问Python对象之前，它必须由当前线程持有。没有锁，即使是最简单的操作也可能导致多线程程序出现问题。

所以我们需要在Server启动时创建解释器，无论有多少条线程调用python，python解释器只能初始化一次。每次线程要调用python时，都要拥有GIL。

在Server启动时还要设置python脚本的路径并启用线程支持。

然后Servant中定义具体通过python C api调用python函数的代码，通过宏控制线程锁的获取和释放。

本项目的示例代码是通过python pandas读取一个excel文件并将文件内容返回。

## 部署tars服务

按照正常编译tarscpp server的方式进行编译部署，需要将服务部署在使用安装了python依赖的镜像启动的tars节点上。并且该节点需要在指定位置放置python脚本。
