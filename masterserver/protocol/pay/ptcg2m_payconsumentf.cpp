#include "pch.h"
#include "pay/ptcg2m_payconsumentf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "pay/paymgr.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "network/dblink.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2016/12/22 10:42:32

void PtcG2M_PayConsumeNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if (NULL == role)
	{
		SSWarn << "role is NULL" << m_Data.roleid() <<END;
		return;
	}
	stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(role->GetID());
	if (NULL == pData)
	{
		return;
	}
	if (pData->m_oConsumeList.size() >= MAX_PAY_BILL_COUNT)
	{
		SSWarn << "roleid=" << role->GetID() << ",max pay consume bill count, failed" << END;
		return;
	}

	if (!CPayMgr::Instance()->isFinishConsume(pData, m_Data.list().type(), m_Data.list().index()))
	{
		SSWarn << "roleid=" << role->GetID() << ",has unfinished bill=" << m_Data.list().billno() << " type=" << m_Data.list().type() << ",index=" << m_Data.list().index() << ", failed" << END;
		return;
	}

	//set status;
	m_Data.mutable_list()->set_status(ConsumeBillStatus_Midas);
	pData->m_uLastCheckConsumeTime = GameTime::GetTime();
	pData->m_oConsumeList.insert(std::make_pair(m_Data.list().billno(), m_Data.list()));

	KKSG::PayAccessInfo oData;
	pData->ToKKSG(&oData);

	std::string strOther;
	if (!oData.otherinfo().SerializeToString(&strOther))
	{
		LogError("other info SerializeToString fail");
		return;
	}
	std::string strConsume;
	if (!oData.consumeinfo().SerializeToString(&strConsume))
	{
		LogError("consume info SerializeToString fail");
		return;
	}
	std::string strSend;
	if (!oData.sendinfo().SerializeToString(&strSend))
	{
		LogError("send info SerializeToString fail");
		return;
	}
	RpcM2D_SavePayAccessData* rpc = RpcM2D_SavePayAccessData::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_saveamt(pData->m_iSaveamt);
	rpc->m_oArg.set_nosaveamt(pData->m_iNoSaveAmt);
	rpc->m_oArg.set_other(strOther);
	rpc->m_oArg.set_consume(strConsume);
	rpc->m_oArg.set_send(strSend);
	rpc->m_oArg.set_op(KKSG::PayAccess_Consume);
	rpc->m_oArg.mutable_list()->CopyFrom(m_Data.list());
	DBLink::Instance()->SendTo(*rpc);
	//CPayMgr::Instance()->SaveData(role->GetID(), KKSG::PayAccess_Consume);
}
