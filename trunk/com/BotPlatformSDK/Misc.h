#pragma once

// create inner class object
template<class C_Type, class I_Type>
HRESULT CreateRealObject( I_Type** ppVal )
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
HRESULT CreateRealObject( I_Type** ppVal, C_Type** ppReal )
{
    HRESULT hr = CreateRealObject<C_Type>(ppVal);
    if ( FAILED( hr ) )
        return hr;

    ATLASSERT(ppReal);
    *ppReal = static_cast<C_Type*>(*ppVal);
    return S_OK;
}


// string convertion routine
const std::string  UnicToUtf8( const std::wstring& unic );
const std::wstring Utf8ToUnic( const std::string& utf8 );
const std::string  MakeMd5( const std::string& data );
const std::string  NumToStr( int num );


// safe handle for pointer
template<class T>
class CSafeHandle
{
public:
    typedef T* PtrType;

public:
    CSafeHandle( PtrType p ) : m_ptr(p) {}

    ~CSafeHandle()
    {
        m_ptr = NULL;
    }

public:
    PtrType GetPtr() const { return m_ptr; }

    void Clear() { m_ptr = NULL; }

public:
    PtrType m_ptr;
};

template<class T>
class CSafeHandlePtr : public boost::shared_ptr<CSafeHandle<T> >
{
public:
    typedef CSafeHandle<T> SafeHandleType; 
};


// no throw
#define BEGIN_SAFE  __try {
#define END_SAFE    } __except( EXCEPTION_EXECUTE_HANDLER ) {}


// com routine
#define UTF8_2_BSTR(u) CComBSTR( Utf8ToUnic(u).c_str() )

#define IMPL_GET_BSTR(pb, u8) \
    if ( !pb ) \
        return E_INVALIDARG; \
    CComBSTR str( Utf8ToUnic(u8).c_str() ); \
    *pb = str.Detach(); \
    return S_OK;

#define IMPL_SET_BSTR(u8, b) \
    if ( !b ) \
        return E_INVALIDARG; \
    u8 = UnicToUtf8(b); \
    return S_OK;

#define IMPL_GET_LONG(pl, l) \
    if ( !pl ) \
        return E_INVALIDARG; \
    *pl = l; \
    return S_OK;

#define IMPL_SET_LONG(l, v) \
    l = v; \
    return S_OK;


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

