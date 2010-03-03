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
        SafeRelease(p);
    }

    m_userMap.clear();
    m_userVector.clear();
}

STDMETHODIMP CRobotUsers::Item(LONG idx, IRobotUser** ppUser)
{
    if ( !ppUser )
        return E_INVALIDARG;

    CRobotUser* user = getUser( idx );
    if ( !user )
        return E_INVALIDARG;

    user->AddRef();
    *ppUser = user;
    return S_OK;
}

STDMETHODIMP CRobotUsers::Count(LONG* pVal)
{
    IMPL_GET_LONG(pVal, getUserCount())
}

void CRobotUsers::addUser( CRobotUser* user )
{
    if ( !user || user->getID().empty() )
        return;

    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    // check if already exist
    if ( m_userMap.find(user->getID()) != m_userMap.end() )
        return;

    // insert to vector
    m_userVector.push_back( user );

    // insert to map
    m_userMap.insert( RobotUserMap::value_type(user->getID(), user) );
}

void CRobotUsers::removeUser( const std::string& userId )
{
    boost::lock_guard<boost::mutex> guard_(m_userMutex);

    RobotUserMap::iterator it = m_userMap.find( userId );
    if ( it != m_userMap.end() )
    {
        CRobotUser* user = it->second;

        // erase in map
        m_userMap.erase(it);
        
        // erase in vector
        RobotUserVector::iterator vit = std::find( m_userVector.begin(), m_userVector.end(), user );
        ATLASSERT ( vit != m_userVector.end() );
        m_userVector.erase( vit );

        // release user
        SafeRelease(user);
    }
}

CRobotUser* CRobotUsers::getUser( const std::string& userId )
{
    // get user by userId
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
    // get user by index
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

