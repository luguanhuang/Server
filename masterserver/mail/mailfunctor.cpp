#include "pch.h"
#include "mailfunctor.h"
#include "mailmgr.h"
#include "role/role.h"
#include "table/globalconfig.h"


CMailDataAddFunctor::CMailDataAddFunctor()
:m_qwRoleID(INVALID_ROLE_ID)
,m_poRes(NULL)
{
}

CMailDataAddFunctor::~CMailDataAddFunctor()
{
}

void CMailDataAddFunctor::operator()()
{
	if(m_qwRoleID == INVALID_ROLE_ID)
	{
		return;
	}

	CMailMgr::Instance()->AddRoleMailInfo(m_qwRoleID);

	std::map<UINT64, KKSG::SMail>* poRoleMails = CMailMgr::Instance()->GetRoleMailInfo(m_qwRoleID);
	if(poRoleMails == NULL)
	{
		LogError("Can't find role [%llu] mail info", m_qwRoleID);
		return;
	}

	UINT32 dwMax = GlobalConfig::GetInstance().MailMax;
	INT32 nBeyond = (m_poRes->mails_size() > (INT32)dwMax ? m_poRes->mails_size() - (INT32)dwMax : 0 );

	for(INT32 i = 0; i < m_poRes->mails_size(); ++i)
	{
		if(i < nBeyond) continue;

		const KKSG::SMail& roMail = m_poRes->mails(i);
		poRoleMails->insert(std::make_pair(roMail.uid(), roMail));
	}

	LogInfo("Put role [%llu] 's mail data to cache, mail count [%d]", m_qwRoleID, m_poRes->mails_size());
}


CMailDataDelFunctor::CMailDataDelFunctor()
{
}

CMailDataDelFunctor::~CMailDataDelFunctor()
{
}

void CMailDataDelFunctor::operator()(UINT64 key)
{
	LogInfo("Del role [%llu] 's mail data from cache", key);
	CMailMgr::Instance()->DelRoleMaillInfo(key);
}