#ifndef _RunPandasImp_H_
#define _RunPandasImp_H_

#include "servant/Application.h"
#include "RunPandas.h"

/**
 *
 *
 */
class RunPandasImp : public DemoApp::RunPandas
{
public:
    /**
     *
     */
    virtual ~RunPandasImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(const string &request, string &response, tars::TarsCurrentPtr current);
};
/////////////////////////////////////////////////////
#endif
