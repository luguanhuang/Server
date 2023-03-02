#include "pch.h"
#include "crossprotocolbanmgr.h"
#include "task/rpcc2g_taskoperate.h"
#include "task/rpcc2g_getdailytaskreward.h"
#include "activity/rpcc2g_getactivitychest.h"
#include "mail/rpcc2m_mailop.h"
#include "battle/rpcc2g_sweep.h"
#include "spactivity/rpcc2g_getspactivityreward.h"
#include "findback/rpcc2g_itemfindback.h"


bool CrossProtocolBanMgr::Init()
{
	AddBan(RPCC2G_TASKOPERATE_TYPE);
	AddBan(RPCC2G_GETDAILYTASKREWARD_TYPE);
	AddBan(RPCC2G_GETACTIVITYCHEST_TYPE);
	AddBan(RPCC2M_MAILOP_TYPE);
	AddBan(RPCC2G_SWEEP_TYPE);
	AddBan(RPCC2G_GETSPACTIVITYREWARD_TYPE);
	AddBan(RPCC2G_ITEMFINDBACK_TYPE);
	return true;
}

void CrossProtocolBanMgr::Uninit()
{

}

bool CrossProtocolBanMgr::IsBan(UINT32 protocolId)
{
	auto it = m_banProtocolIds.find(protocolId);
	return it != m_banProtocolIds.end();
}

void CrossProtocolBanMgr::AddBan(UINT32 protocolId)
{
	m_banProtocolIds.insert(protocolId);
}
