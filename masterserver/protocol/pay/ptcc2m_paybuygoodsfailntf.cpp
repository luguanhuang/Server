#include "pch.h"
#include "pay/ptcc2m_paybuygoodsfailntf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "pay/paymgr.h"
#include "pay/ibgiftmgr.h"

// generate by ProtoGen at date: 2017/3/16 22:45:50

void PtcC2M_PayBuyGoodsFailNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	
	
	KKSG::PayGiftIbItem* pIbItem = IBGiftMgr::Instance()->GetIbItem(role->GetID(), m_Data.token());
	if (NULL == pIbItem)
	{
		SSWarn << "invalid billno=" << m_Data.token() << END;
		return;
	}
	pIbItem->set_status(KKSG::PayGiftIbStatus_Fail);
	IBGiftMgr::Instance()->Save(KKSG::MYSQL_OP_UPDATE, *pIbItem);
	LogInfo("PayBuyGoodsFailNtf::Process =fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",\
		pIbItem->fromroleid(),pIbItem->toroleid(),pIbItem->billno().c_str(),pIbItem->status(),pIbItem->itemid(),pIbItem->count(),pIbItem->ts(),pIbItem->text().c_str());
}
