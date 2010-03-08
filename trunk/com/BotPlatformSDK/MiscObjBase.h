#pragma once

// JSonCmdBase
class JSonCmdBase
{
public:
    virtual ~JSonCmdBase() {}

    virtual void execute( Json::Value& val, void* para ) = 0;
};


// CheckToken
class CheckToken
{
public:
    enum 
    { 
        TIME_LIMIT = 60 * 1000,
        TIME_OUT   = 12,
        TIME_INTER = TIME_LIMIT / TIME_OUT
    };

public:
    CheckToken() : m_time(0) {}

    ~CheckToken() { m_time = 0; }

public:
    void reset();

    void step();

    bool isTimeOut() const;

private:
    long m_time;
};

typedef boost::shared_ptr<CheckToken> CheckTokenPtr;

