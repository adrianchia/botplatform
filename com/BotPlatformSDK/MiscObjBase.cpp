#include "StdAfx.h"
#include "MiscObjBase.h"


void CheckToken::reset()
{
    InterlockedExchange( &m_time, 0 );
}

void CheckToken::step()
{
    InterlockedIncrement( &m_time );
}

bool CheckToken::isTimeOut() const
{
    return m_time >= TIME_OUT;
}

