#pragma once

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

const std::string  UnicToUtf8( const std::wstring& unic );
const std::wstring Utf8ToUnic( const std::string& utf8 );
const std::string  MakeMd5( const std::string& data );
const std::string  NumToStr( int num );

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


#define BEGIN_SAFE  __try {
#define END_SAFE    } __except( EXCEPTION_EXECUTE_HANDLER ) {}

#define UTF8_2_BSTR(u) CComBSTR( Utf8ToUnic(u).c_str() )

