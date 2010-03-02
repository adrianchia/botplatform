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

const std::string UnicToUtf8( const std::wstring& unic )
{
    std::string utf8;
    WideToMulti( unic, utf8, CP_UTF8 );
    return utf8;
}

const std::wstring Utf8ToUnic( const std::string& utf8 )
{
    std::wstring unic;
    MultiToWide( utf8, unic, CP_UTF8 );
    return unic;
}

// 计算Hash，成功返回0，失败返回GetLastError()
//  CONST BYTE *pbData, // 输入数据 
//  DWORD dwDataLen,    // 输入数据字节长度 
//  ALG_ID algId        // Hash 算法：CALG_MD5,CALG_SHA
//  LPTSTR pszHash,     // 输出16进制Hash字符串，MD5长度为32+1, SHA长度为40+1
// 
DWORD GetHash(CONST BYTE *pbData, DWORD dwDataLen, ALG_ID algId, LPTSTR pszHash)
{
    DWORD dwReturn = 0;
    HCRYPTPROV hProv;
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        return (dwReturn = GetLastError());

    HCRYPTHASH hHash;
    //Alg Id:CALG_MD5,CALG_SHA
    if(!CryptCreateHash(hProv, algId, 0, 0, &hHash)) 
    {
        dwReturn = GetLastError();
        CryptReleaseContext(hProv, 0);
        return dwReturn;
    }

    if(!CryptHashData(hHash, pbData, dwDataLen, 0))
    {
        dwReturn = GetLastError();
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return dwReturn;
    }

    DWORD dwSize;
    DWORD dwLen = sizeof(dwSize);
    CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)(&dwSize), &dwLen, 0);

    BYTE* pHash = new BYTE[dwSize];
    dwLen = dwSize;
    CryptGetHashParam(hHash, HP_HASHVAL, pHash, &dwLen, 0);

    lstrcpy(pszHash, _T(""));
    TCHAR szTemp[3];
    for (DWORD i = 0; i < dwLen; ++i)
    {
        //wsprintf(szTemp, _T("%X%X"), pHash[i] >> 4, pHash[i] & 0xf);
        wsprintf(szTemp, L"%02x", pHash[i]);
        lstrcat(pszHash, szTemp);
    }
    delete [] pHash;

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return dwReturn;
}

const std::string MakeMd5( const std::string& data )
{
    TCHAR buf[255] = {0};
    if ( GetHash( (const BYTE*)data.data(), data.size(), CALG_MD5, buf ) != 0 )
    {
        //std::cout << "hash error\n";
        return "";
    }

    return std::string( (LPCSTR)CW2A(buf) );
}

const std::string NumToStr( int num )
{
    char buf[255] = {0};
    sprintf_s( buf, "%d", num );
    return buf;
}