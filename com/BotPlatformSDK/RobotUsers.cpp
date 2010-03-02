// RobotUsers.cpp : CRobotUsers 的实现

#include "stdafx.h"
#include "RobotUsers.h"
#include "RobotUser.h"

// CRobotUsers

void CRobotUsers::FinalRelease()
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    for ( RobotUserMap::iterator it = m_userMap.begin(); it != m_userMap.end(); ++it )
    {
        CRobotUser* p = it->second;
        if ( p )
            p->Release();
    }

    m_userMap.clear();
    m_userVector.clear();
}

STDMETHODIMP CRobotUsers::Item(LONG idx, IRobotUser** ppUser)
{
    // TODO: 在此添加实现代码
    if ( !ppUser )
        return E_FAIL;

    CRobotUser* user = getUser( idx );
    if ( !user )
        return E_INVALIDARG;

    user->AddRef();
    *ppUser = user;
    return S_OK;
}

STDMETHODIMP CRobotUsers::Count(LONG* pVal)
{
    // TODO: 在此添加实现代码
    if ( !pVal )
        return E_INVALIDARG;

    *pVal = getUserCount();
    return S_OK;
}

void CRobotUsers::addUser( const std::string& userId, CRobotUser* user )
{
    if ( !user )
        return;

    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    if ( m_userMap.find(userId) != m_userMap.end() )
        return;

    m_userVector.push_back( user );
    m_userMap.insert( RobotUserMap::value_type(userId, user) );
}

void CRobotUsers::removeUser( const std::string& userId )
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    RobotUserMap::iterator it = m_userMap.find( userId );
    if ( it != m_userMap.end() )
    {
        CRobotUser* user = it->second;
        m_userMap.erase(it);
        
        RobotUserVector::iterator vit = std::find( m_userVector.begin(), m_userVector.end(), user );
        ATLASSERT ( vit != m_userVector.end() );
        m_userVector.erase( vit );

        if ( user )
            user->Release();
    }
}

CRobotUser* CRobotUsers::getUser( const std::string& userId )
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    RobotUserMap::iterator it = m_userMap.find( userId );
    if ( it != m_userMap.end() )
    {
        return it->second;
    }

    return NULL;
}

CRobotUser* CRobotUsers::getUser( size_t idx )
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    if ( idx < m_userVector.size() )
    {
        return m_userVector[idx];
    }

    return NULL;
}

size_t CRobotUsers::getUserCount()
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);
    return m_userVector.size();
}

