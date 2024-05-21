#ifndef _DemoPythonServer_H_
#define _DemoPythonServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class DemoPythonServer : public Application
{
public:
    /**
     *
     **/
    virtual ~DemoPythonServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern DemoPythonServer g_app;

////////////////////////////////////////////
#endif
