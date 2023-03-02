#include "pch.h"
#include "pay/ptcm2g_buypayaileenntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/payconfig.h"
#include "gamelogic/payv2Mgr.h"

// generate by ProtoGen at date: 2017/4/1 20:34:50

void PtcM2G_BuyPayAileenNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (NULL == pRole)
	{
		return ;
	}

	std::string paramid = CPayConfig::Instance()->GetParamByServiceCode(m_Data.paramid());
	if (paramid.empty())
	{
		return;
	}
	PayV2Mgr::Instance()->SetPayAileenStatus(pRole, paramid);
}
