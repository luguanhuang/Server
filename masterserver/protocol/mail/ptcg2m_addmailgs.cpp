#include "pch.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "mail/ptcg2m_addmailgs.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/7/31 22:49:48

void PtcG2M_AddMailGS::Process(UINT32 dwConnID)
{
	KKSG::SMail* mail = m_Data.mutable_mail();
	for (int i = 0; i < m_Data.roleids_size(); ++i)
	{
		UINT64 roleid = m_Data.roleids(i);
		if (CRoleSummaryMgr::Instance()->GetRoleSummary(roleid) == NULL)
		{
			continue;
		}
		mail->set_uid(NewMailID());
		CMailMgr::Instance()->SendMail(roleid, *mail, 0, 0, 0);
	}
}
