#include "pch.h"
#include "pay/ptcc2m_payparameterinfontf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "httptask/paynotify.h"
#include "pay/paymgr.h"
#include "httptask/httptaskmgr.h"

// generate by ProtoGen at date: 2016/12/19 19:28:28

void PtcC2M_PayParameterInfoNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	std::string openkey = role->GetPayParameterInfo().openkey();
	role->SetPayParameterInfo(m_Data);
	if (role->GetLoginType() == KKSG::LOGIN_PASSWORD)
	{
		// by huangds, 2018/5/16. 登陆拉取余额
		if (!role->GetIsBalance())
		{
			CPayMgr::Instance()->OnLogin(role);
			role->SetIsBalance(true);
		}
		return;
	}
	if (!role->GetIsBalance() && !m_Data.appid().empty() && !m_Data.pf().empty() && !m_Data.pfkey().empty())
	{
		CPayMgr::Instance()->OnLogin(role);
		role->SetIsBalance(true);
	}
	else if (!openkey.empty() && openkey != m_Data.openkey())
	{
		CPayMgr::Instance()->OnLogin(role);
		LogInfo("roleid=%llu,pre_openkey=%s,openkey=%s", role->GetID(), openkey.c_str(), m_Data.openkey().c_str());
	}
	LogInfo("roleid=%llu,openkey=%s,sessionid=%s,sessiontype=%s,pf=%s,pfkey=%s,appid=%s", role->GetID(), m_Data.openkey().c_str(), m_Data.sessionid().c_str(), m_Data.sessiontype().c_str(), m_Data.pf().c_str(), m_Data.pfkey().c_str(), m_Data.appid().c_str());
}
