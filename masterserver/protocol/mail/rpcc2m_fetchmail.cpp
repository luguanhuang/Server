#include "pch.h"
#include "mail/rpcc2m_fetchmail.h"
#include "role/rolemanager.h"
#include "mail/mailmgr.h"
#include "timeutil.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2016/7/22 10:18:01

RPC_SERVER_IMPLEMETION(RpcC2M_FetchMail, FetchMailArg, FetchMailRes)

void RpcC2M_FetchMail::OnCall(const FetchMailArg &roArg, FetchMailRes &roRes)
{
	roRes.set_page(0);
	roRes.set_pagecount(0);

	///> system open
	//if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysMailId))
	//{
	//	SSInfo<<"mail system close"<<END;
	//	return;
	//}

	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		return;
	}

	if (!poRole->IsSystemOpened(SysMailId))
	{
		SSInfo<<"system close"<<END;
		return;
	}

	UINT32 page = roArg.page();
	UINT32 count = roArg.count();
	if (0 == count)
	{
		return;
	}

	UINT32 allpage = 0;
	std::vector<KKSG::SMail*> mails;
	if(!CMailMgr::Instance()->Fetch(poRole->GetID(), page, allpage, count, mails))
	{
		roRes.set_page(0);
		roRes.set_pagecount(0);
		return;
	}

	UINT32 dwNow = TimeUtil::GetTime();
	roRes.set_page(page);
	roRes.set_pagecount(allpage);
	for(std::vector<KKSG::SMail*>::size_type i = 0; i < mails.size(); ++i)
	{
		KKSG::SMail* data = roRes.add_mails();
		*data = *mails[i];

		UINT32 dwTime = mails[i]->timeexpire();
		data->set_timeleft(dwTime > dwNow ? dwTime - dwNow : 0);
	}

	//MailMgr::Instance()->HintCheck(role);
}

void RpcC2M_FetchMail::OnDelayReplyRpc(const FetchMailArg &roArg, FetchMailRes &roRes, const CUserData &roUserData)
{
}
