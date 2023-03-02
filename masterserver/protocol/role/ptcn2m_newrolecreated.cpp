#include "pch.h"
#include "role/ptcn2m_newrolecreated.h"
#include "role/rolesummarymgr.h"
#include "loghelper/tlogr.h"
#include "loghelper/loghelper.h"

// generate by ProtoGen at date: 2016/11/4 0:47:46

void PtcN2M_NewRoleCreated::Process(UINT32 dwConnID)
{
	KKSG::RoleSummaryStored rolesummary;
	rolesummary.set_roleid(m_Data.roledata().brief().roleid());
	rolesummary.set_name(m_Data.roledata().brief().name());
	rolesummary.set_nickid(m_Data.roledata().brief().nickid());
	rolesummary.set_account(m_Data.roledata().brief().accountid());

	CRoleSummaryMgr::Instance()->InsertRoleSummary(rolesummary, 0, 0, false);
	CRoleSummaryMgr::Instance()->SetChanged(rolesummary.roleid());

	if(m_Data.totalrolenum() == 1)
	{
		TRegisterLog oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_szOpenID = m_Data.roledata().brief().accountid();
		oLog.m_uRoleID = m_Data.roledata().brief().roleid();
		oLog.m_stConf.CopyFrom(m_Data.clientinfo());
		oLog.m_iPlatID = m_Data.clientinfo().platid();
		oLog.Do();
	}
}
