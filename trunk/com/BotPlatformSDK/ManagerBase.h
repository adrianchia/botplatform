#pragma once

class JSonCmdBase
{
public:
    virtual ~JSonCmdBase() {}

    virtual void execute( Json::Value& val, void* para ) = 0;
};


class CheckToken
{
    //friend class ManagerBase;

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


class ManagerBase
{
protected:
    typedef std::vector<boost::thread*> ThreadList;
    typedef std::vector<HANDLE>         HandleList;

    typedef boost::unordered_map<std::string, JSonCmdBase*> JSonCmdMap;

    struct CheckItem
    {
        boost::function<void(bool)> callback;
        CheckTokenPtr               token;
    };

    typedef boost::unordered_map<const void*, CheckItem> CheckItemMap;

public:
    ManagerBase();

    virtual ~ManagerBase();

public:
    void init( int threadCount );
    
    void close();

    template<class T>
    void addTask( const T& task )
    {
        if ( m_tp )
        {
            m_tp->schedule( task );
        }
    }

    template<class CmdType>
    void registerJSonCmd()
    {
        bool b = m_cmdMap.insert( JSonCmdMap::value_type( CmdType::getTypeName(), new CmdType() ) ).second;
        ATLASSERT(b);
    }

    JSonCmdBase* findJsonCmd( const std::string& name );

    void clearCmds();

    boost::asio::io_service& getIOService() { return m_ioService; }

    void startRun();

    template<class T>
    CheckTokenPtr registerCheck( const void* id, const T& func )
    {
        CheckItem      item;

        item.token    = CheckTokenPtr(new CheckToken);
        item.callback = func;

        {
            boost::lock_guard<boost::mutex> guard_(m_checkItemMutex);
            m_checkItemMap.insert( CheckItemMap::value_type(id, item) );
        }
        
        return item.token;
    }

    void unRegisterCheck( const void* id )
    {
        boost::lock_guard<boost::mutex> guard_(m_checkItemMutex);

        CheckItemMap::iterator it = m_checkItemMap.find( id );
        if ( it != m_checkItemMap.end() )
            m_checkItemMap.erase( it );
    }

private:
    void runIoService( int idx );

    void runCheckTime();

protected:
    boost::threadpool::pool* m_tp;

    ThreadList               m_runth;

    JSonCmdMap               m_cmdMap;

    boost::asio::io_service  m_ioService;

    volatile bool            m_run;

    HandleList               m_ioevent;

    boost::thread*           m_checkth;

    CheckItemMap             m_checkItemMap;

    boost::mutex             m_checkItemMutex;
};

