#include "pch.h"
#include "time.h"
#include "role/rolemanager.h"
#include "common/ptcc2m_rolestatereportnew.h"
#include "common/ptcm2c_rolestatentfnew.h"

// generate by ProtoGen at date: 2016/8/3 10:50:33

void PtcC2M_RoleStateReportNew::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	if (0 == m_Data.roleid_size())
	{
		return;
	}
	UINT32 nowtime = (UINT32)time(NULL);
	PtcM2C_RoleStateNtfNew ntf;
	for (int i = 0; i < m_Data.roleid_size(); ++i)
	{
		UINT64 roleid = m_Data.roleid(i);
		ntf.m_Data.add_roleid(roleid);
		UINT32 state = KKSG::Logoff;
		UINT32 lastlogout = 0;
		CRoleManager::Instance()->GetRoleState(roleid, state, lastlogout);
		ntf.m_Data.add_timelastlogin(lastlogout);
		ntf.m_Data.add_state(state);
	}
	role->Send(ntf);
}
