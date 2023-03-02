#include "pch.h"
#include "social/rpcc2m_showflowerpagenew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "social/flowerop.h"
#include "social/flower.h"

// generate by ProtoGen at date: 2016/8/18 11:11:26

RPC_SERVER_IMPLEMETION(RpcC2M_ShowFlowerPageNew, ShowFlowerPageArg, ShowFlowerPageRes)

void RpcC2M_ShowFlowerPageNew::OnCall(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	UINT64 otherroleid = roArg.roleid();

	/*CFlowerRecord* record = role->Get<CFlowerRecord>();
	record->Update();*/

	CRoleSummary* otherrole = CRoleSummaryMgr::Instance()->GetRoleSummary(otherroleid);
	if (NULL == otherrole)
	{
		return;
	}
	roRes.set_rolename(otherrole->GetName());
	roRes.set_roleprofession(otherrole->GetProfession());

	FlowerOp op(otherroleid);
	Flower* flowerinfo = op.GetFlowerInfo();
	if (NULL == flowerinfo)
	{
		return;
	}
	flowerinfo->Update();


	for (auto i = flowerinfo->m_sendrecords.begin(); i != flowerinfo->m_sendrecords.end(); ++i)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
		if (NULL != summary)
		{
			KKSG::FlowerInfo2Client* info = roRes.add_sendlog();
			info->set_roleid(summary->GetID());
			info->set_rolename(summary->GetName());
			info->set_itemid(i->itemID);
			info->set_count(i->count);
			info->set_timestamp(i->timestamp);
		}
	}
	flowerinfo->Sort();
	int roleCount = 0;
	for (auto i = flowerinfo->m_rolecount.begin(); i != flowerinfo->m_rolecount.end() && roleCount < FLOWERROLELIMIT; ++i, ++roleCount)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
		if (NULL != summary)
		{
			KKSG::ReceiveRoleFlowerInfo2Client* info = roRes.add_receiverank();
			info->set_roleid(summary->GetID());
			info->set_rolename(summary->GetName());
			for (auto j = i->flowersCount.begin(); j != i->flowersCount.end(); ++j)
			{
				KKSG::MapIntItem * flower = info->add_flowers();
				flower->set_key(j->first);
				flower->set_value(j->second);
			}
		}
	}
	for (auto i = flowerinfo->m_mapSendFlowersTotal.begin(); i != flowerinfo->m_mapSendFlowersTotal.end(); ++i)
	{
		KKSG::MapIntItem* info = roRes.add_sendflowerstotal();
		info->set_key(i->first);
		info->set_value(i->second);
	}

	std::map<UINT32, UINT32> mapFlowers;
	flowerinfo->GetTotalReceiveCount2Client(mapFlowers);
	for (auto i = mapFlowers.begin(); i != mapFlowers.end(); ++i)
	{
		KKSG::MapIntItem* info = roRes.add_receiveflowerstotal();
		info->set_key(i->first);
		info->set_value(i->second);
	}
	return;
}

void RpcC2M_ShowFlowerPageNew::OnDelayReplyRpc(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes, const CUserData &roUserData)
{
}
