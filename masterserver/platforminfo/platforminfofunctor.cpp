#include "pch.h"
#include "platforminfofunctor.h"
#include "platforminfo/platformfriendmgr.h"
#include "commondef.h"


CPlatFormInfoAddFunctor::CPlatFormInfoAddFunctor()
:m_qwRoleID(INVALID_ROLE_ID)
{
}

CPlatFormInfoAddFunctor::~CPlatFormInfoAddFunctor()
{
}

void CPlatFormInfoAddFunctor::operator()()
{
	if(m_qwRoleID == INVALID_ROLE_ID)
	{
		return;
	}
}


CPlatFormInfoDelFunctor::CPlatFormInfoDelFunctor()
{
}

CPlatFormInfoDelFunctor::~CPlatFormInfoDelFunctor()
{
}

void CPlatFormInfoDelFunctor::operator()(std::string key)
{
	LogInfo("Del account %s plat data from cache", key.c_str());
	CPlatformFriendMgr::Instance()->DelRolePlatDetailInfo(key);
}
