#include "stdafx.h"

void WideToMulti( const std::wstring& wide, std::string& multi, UINT code_page )
{
    if ( wide.empty() )
    {
        multi.clear();
        return;
    }

    int new_size = WideCharToMultiByte( code_page, 0, wide.data(), (int)wide.size(), &multi[0], 0, NULL, NULL );
    multi.assign( new_size, '\0' );
    WideCharToMultiByte( code_page, 0, wide.data(), (int)wide.size(), &multi[0], (int)multi.size(), NULL, NULL );
}

void MultiToWide( const std::string& multi, std::wstring& wide, UINT code_page )
{
    if ( multi.empty() )
    {
        wide.clear();
        return;
    }

    wide.assign( multi.size(), L'\0' );
    int new_size = MultiByteToWideChar( code_page, 0, multi.data(), (int)multi.size(), &wide[0], (int)wide.size() );
    wide.resize( new_size );
}

const std::string unicToUtf8( const std::wstring& unic )
{
    std::string utf8;
    WideToMulti( unic, utf8, CP_UTF8 );
    return utf8;
}

const std::wstring utf8ToUnic( const std::string& utf8 )
{
    std::wstring unic;
    MultiToWide( utf8, unic, CP_UTF8 );
    return unic;
}

DWORD GetHash( CONST BYTE *pbData, DWORD dwDataLen, ALG_ID algId, LPTSTR pszHash )
{
    DWORD      dwReturn = 0;
    HCRYPTPROV hProv    = NULL;

    if ( !CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) )
        return (dwReturn = GetLastError());

    HCRYPTHASH hHash = NULL;
    if ( !CryptCreateHash(hProv, algId, 0, 0, &hHash) ) 
    {
        dwReturn = GetLastError();
        CryptReleaseContext(hProv, 0);
        return dwReturn;
    }

    if ( !CryptHashData(hHash, pbData, dwDataLen, 0) )
    {
        dwReturn = GetLastError();
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return dwReturn;
    }

    DWORD dwSize = 0;
    DWORD dwLen  = sizeof(dwSize);
    CryptGetHashParam( hHash, HP_HASHSIZE, (BYTE*)(&dwSize), &dwLen, 0 );

    BYTE* pHash = new BYTE[dwSize];
    dwLen = dwSize;
    CryptGetHashParam( hHash, HP_HASHVAL, pHash, &dwLen, 0 );

    lstrcpy( pszHash, _T("") );
    TCHAR szTemp[3] = {0};

    for ( DWORD i=0; i<dwLen; ++i )
    {
        wsprintf(szTemp, L"%02x", pHash[i]);
        lstrcat(pszHash, szTemp);
    }

    delete []pHash;
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return dwReturn;
}

const std::string makeMd5( const std::string& data )
{
    TCHAR buf[255] = {0};
    if ( GetHash( (const BYTE*)data.data(), data.size(), CALG_MD5, buf ) != 0 )
    {
        return "";
    }

    return std::string( (LPCSTR)CW2A(buf) );
}

const std::string numToStr( int num )
{
    char buf[255] = {0};
    sprintf_s( buf, "%d", num );
    return buf;
}

