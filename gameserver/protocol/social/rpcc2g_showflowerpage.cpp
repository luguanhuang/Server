#include "pch.h"
#include "social/rpcc2g_showflowerpage.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/flowerrecord.h"

// generate by ProtoGen at date: 2015/9/23 16:11:28

RPC_SERVER_IMPLEMETION(RpcC2G_ShowFlowerPage, ShowFlowerPageArg, ShowFlowerPageRes)

void RpcC2G_ShowFlowerPage::OnCall(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes)
{
	/*
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	UINT64 otherroleid = roArg.roleid();

	CFlowerRecord* record = role->Get<CFlowerRecord>();
	record->Update();
	
	RoleSummary* otherrole = RoleSummaryMgr::Instance()->FindRoleSummary(otherroleid);
	if (NULL == otherrole)
	{
		return;
	}
	roRes.set_rolename(otherrole->strName);
	roRes.set_roleprofession(otherrole->dwProfession);

	FlowerOp op(otherroleid);
	Flower* flowerinfo = op.GetFlowerInfo();
	if (NULL == flowerinfo)
	{
		return;
	}
	flowerinfo->Update();


	for (auto i = flowerinfo->m_sendrecords.begin(); i != flowerinfo->m_sendrecords.end(); ++i)
	{
		RoleSummary* summary = RoleSummaryMgr::Instance()->FindRoleSummary(i->roleid);
		if (NULL != summary)
		{
			KKSG::FlowerInfo2Client* info = roRes.add_sendlog();
			info->set_roleid(summary->qwRoleID);
			info->set_rolename(summary->strName);
			info->set_itemid(i->itemID);
			info->set_count(i->count);
			info->set_timestamp(i->timestamp);
		}
	}
	flowerinfo->Sort();
	for (auto i = flowerinfo->m_rolecount.begin(); i != flowerinfo->m_rolecount.end(); ++i)
	{
		RoleSummary* summary = RoleSummaryMgr::Instance()->FindRoleSummary(i->roleid);
		if (NULL != summary)
		{
			KKSG::ReceiveRoleFlowerInfo2Client* info = roRes.add_receiverank();
			info->set_roleid(summary->qwRoleID);
			info->set_rolename(summary->strName);
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
	*/
}

void RpcC2G_ShowFlowerPage::OnDelayReplyRpc(const ShowFlowerPageArg &roArg, ShowFlowerPageRes &roRes, const CUserData &roUserData)
{
}
