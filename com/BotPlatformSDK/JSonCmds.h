#pragma once
#include "ManagerBase.h"


//////////////////////////////////////////////////////////////////////////
// base class for json cmd

#define JSON_CMD_TYPE_NAME(s)  public: static const char* getTypeName() { return s; }

class CRobotServer;

class JSonCmdServerBase : public JSonCmdBase
{
public:
    virtual void execute( Json::Value& root, void* para )
    {
        std::string robotId   = root["robotId"].asString();
        std::string userId    = root["userId"].asString();
        std::string sessionId = root["sessionId"].asString();

        CRobotServer* pServer = (CRobotServer*)para;
        doTask( pServer, robotId, userId, sessionId, root );
    }

protected:
    virtual void doTask( CRobotServer* pServer, const std::string& robotId, const std::string& userId, const std::string& sessionId, Json::Value& root ) = 0;
};

