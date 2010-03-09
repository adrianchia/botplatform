#pragma once

// create inner class object
template<class C_Type, class I_Type>
HRESULT createInnerObject( I_Type** ppVal )
{
    if ( !ppVal )
        return E_INVALIDARG;

    *ppVal = NULL;

    CComObject<C_Type>* c_obj = NULL;
    HRESULT hr = CComObject<C_Type>::CreateInstance( &c_obj );
    if ( FAILED(hr) )
        return hr;

    I_Type* i_obj = NULL;
    hr = c_obj->QueryInterface( &i_obj );
    if ( FAILED(hr) )
        return hr;

    *ppVal = i_obj;
    return S_OK;
};

template<class C_Type, class I_Type>
HRESULT createInnerObject( I_Type** ppVal, C_Type** ppReal )
{
    HRESULT hr = createInnerObject<C_Type>(ppVal);
    if ( FAILED( hr ) )
        return hr;

    ATLASSERT(ppReal);
    *ppReal = static_cast<C_Type*>(*ppVal);
    return S_OK;
}


// string convertion routine
const std::string  unicToUtf8( const std::wstring& unic );
const std::wstring utf8ToUnic( const std::string& utf8 );
const std::string  makeMd5( const std::string& data );
const std::string  numToStr( int num );


// safe handle for pointer
template<class T>
class SafeHandle
{
public:
    typedef T* PtrType;

public:
    SafeHandle( PtrType p ) : m_ptr(p) {}

    ~SafeHandle()
    {
        m_ptr = NULL;
    }

public:
    PtrType getPtr() const { return m_ptr; }

    void clearPtr() { m_ptr = NULL; }

public:
    PtrType m_ptr;
};

template<class T>
class SafeHandlePtr : public boost::shared_ptr<SafeHandle<T> >
{
public:
    typedef SafeHandle<T> SafeHandleType; 
};


// no throw
#define BEGIN_SAFE  __try {
#define END_SAFE    } __except( EXCEPTION_EXECUTE_HANDLER ) {}


// com routine
#define UTF8_2_BSTR(u) CComBSTR( utf8ToUnic(u).c_str() )

#define IMPL_GET_BSTR(pb, u8) \
    if ( !pb ) \
        return E_INVALIDARG; \
    CComBSTR str( utf8ToUnic(u8).c_str() ); \
    *pb = str.Detach(); \
    return S_OK;

#define IMPL_SET_BSTR(u8, b) \
    if ( !b ) \
        return E_INVALIDARG; \
    u8 = unicToUtf8(b); \
    return S_OK;

#define IMPL_GET_LONG(pl, l) \
    if ( !pl ) \
        return E_INVALIDARG; \
    *pl = l; \
    return S_OK;

#define IMPL_SET_LONG(l, v) \
    l = v; \
    return S_OK;

template<class T>
void safeRelease(T*& p)
{
    if ( p )
    {
        p->Release();
        p = NULL;
    }
}

inline bool isValidStr( BSTR str )
{
    return str && (*str != 0);
}

// json routine
#define BEGIN_JSON_PARSE(jv)        if ( !jv.isNull() ) { Json::Value& jval = jv;
#define JSON_BIND_STR(var, name)    if ( !jval[#name].isNull() ) { var = jval[#name].asString(); }
#define JSON_BIND_INT(var, name)    if ( !jval[#name].isNull() ) { var = jval[#name].asInt(); }
#define END_JSON_PARSE()            }


// property get/set define
#define DEFINE_TYPE_PROP(type, var, name) \
    public: \
        const type& get##name() const { return var; } \
        void set##name( const type& v ) { var = v; } \
    private: \
        type var;

#define DEFINE_STR_PROP(var, name) DEFINE_TYPE_PROP(std::string, var, name)

#define DEFINE_INT_PROP(var, name) \
    public: \
        void set##name( int v ) { var = v; } \
        int get##name() const { return var; } \
    private: \
        int var;

