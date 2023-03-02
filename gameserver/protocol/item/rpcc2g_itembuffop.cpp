#include "pch.h"
#include "item/rpcc2g_itembuffop.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "buff/buffrecord.h"
#include "time.h"

// generate by ProtoGen at date: 2016/11/9 11:30:27

RPC_SERVER_IMPLEMETION(RpcC2G_ItemBuffOp, ItemBuffOpArg, ItemBuffOpRes)

void RpcC2G_ItemBuffOp::OnCall(const ItemBuffOpArg &roArg, ItemBuffOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	BuffRecord* record = role->Get<BuffRecord>();
	record->Update();
	if (0 == m_oArg.op())
	{
		std::map<UINT32, SBuffItem>& items = record->GetBuffItems();
		for (auto i = items.begin(); i != items.end(); ++i)
		{
			roRes.add_itemid(i->second.itemid);					
			roRes.add_itemcount(i->second.itemcount);					
		}
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		return;
	}
	else if (1 == m_oArg.op())
	{
		if (!record->UseBuffItem(m_oArg.itemid()))
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else if (2 == m_oArg.op())
	{
		const std::map<UINT32, BuffDesc>* buffs = record->GetBuffs(1);
		if (NULL != buffs)
		{
			UINT32 nowtime = (UINT32)time(NULL);
			for (auto i = buffs->begin(); i != buffs->end(); ++i)
			{
				if (i->second.effecttime > nowtime)
				{
					roRes.add_buffid(i->second.buffID);
					roRes.add_lefttime(i->second.effecttime - nowtime);
				}
			}
		}
	}
	else if (3 == m_oArg.op())
	{
		if (!record->UseDoodItem(m_oArg.index()))
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		SSWarn<<"invalid op:"<<m_oArg.op()<<END;
	}
}

void RpcC2G_ItemBuffOp::OnDelayReplyRpc(const ItemBuffOpArg &roArg, ItemBuffOpRes &roRes, const CUserData &roUserData)
{
}
