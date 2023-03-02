#include "pch.h"
#include "util.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "buff/buffrecord.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "transform/rpcc2g_transformop.h"
#include "chat/ptcg2m_privatechat.h"
#include "gamelogic/notice.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "buff/XBuffTemplateManager.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2017/9/9 14:10:51

RPC_SERVER_IMPLEMETION(RpcC2G_TransformOp, TransformOpArg, TransformOpRes)

//#define TRANS_OTHER_ITEMID GetGlobalConfig().TransOtherItemId

static void ChatNotifyGS(UINT64 srcroleid, UINT64 destroleid, UINT32 buffid, UINT32 time)
{
	// monster
	BuffTable::RowData* buffdata = XBuffTemplateManager::Instance()->GetBuffInfo(buffid, 1, NULL);
	if (NULL == buffdata)
	{
		return;
	}
	UINT32 monsterid = buffdata->StateParam;
	const XEntityStatistics::RowData* monsterdata = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(monsterid);
	if (NULL == monsterdata)
	{
		return;
	}

	Notice notice(TransOtherNotice);
	notice.Replace("$C", monsterdata->Name);
	notice.Replace("$C", ToString(time));
	if (NULL == notice.chatinfo)
	{
		return;
	}

	PtcG2M_PrivateChat chat;
	chat.m_Data.set_sourceroleid(srcroleid);
	chat.m_Data.set_destroleid(destroleid);

	chat.m_Data.set_content(notice.chatinfo->info());

	MSLink::Instance()->SendTo(chat);
}

void RpcC2G_TransformOp::OnCall(const TransformOpArg &roArg, TransformOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	///> 变自己，看自己当前状态 self
	if (0 == roArg.op())
	{
		UINT32 ret = role->Get<BuffRecord>()->CanTrans();
		if (KKSG::ERR_SUCCESS != ret)
		{
			roRes.set_errorcode(KKSG::ErrorCode(ret));
			return;
		}
		BuffRecord* record = role->Get<BuffRecord>();
   		if (!record->HasTransBuff() && !record->HasSmallBuff())
   		{
   			roRes.set_errorcode(KKSG::ERR_TRANS_NONE);
   			return;
   		}
		else
		{
			bool istrue = false;
			if (record->HasTransBuff())
			{
				if (record->IsTransforming())
				{
					if (!record->IsTransformPassive())
					{
						record->DelTransBuff(0, 1);
						istrue = true;
					}
				}
				else
				{
					record->RecoverTransBuff(false, 1);
					istrue = true;
				}
			}
			if (record->HasSmallBuff())
			{
				if (record->IsSmalling())
				{
					record->DelTransBuff(0, 2);
					istrue = true;
				}
				else
				{
					record->RecoverTransBuff(false, 2);
					istrue = true;
				}
			}
			if (!istrue)
			{
				roRes.set_errorcode(KKSG::ERR_TRANS_PASSIVE);
				return;
			}
		}
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	///> 变别人 other
	else if (1 == roArg.op())
	{
		UINT64 otherroleid = convert<UINT64>(roArg.roleid());
		Role* other = RoleManager::Instance()->FindByRoleID(otherroleid);
		if (NULL == other)
		{
			roRes.set_errorcode(KKSG::ERR_TRANS_OTHERNOTEXIST);
			return;
		}

		///> 变大小只能变自己
		UINT32 itemid = roArg.itemid();
		if (!TransformBuff::CanTransOther(itemid))
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		// transform buff
		ItemTransformConf* transconf = ItemConfig::Instance()->GetItemTransformConf(itemid);
		if (NULL == transconf)
		{
			LogWarn("role [%llu], item [%u] not in ItemTransBuff.txt", role->GetID(), itemid);
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		// add buff
		BuffDesc buff = ItemConfig::Instance()->GetTransBuff(transconf);
		if (0 == buff.buffID)
		{
			SSError<<"roleid:"<<role->GetID()<<" itemid:"<<itemid<<" random trans buff error"<<END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		else
		{
			int mask = XBuffTemplateManager::Instance()->IsScaleBuff(buff.buffID) ? 2 : 1;
			UINT32 ret = other->Get<BuffRecord>()->CanTrans(mask);
			if (KKSG::ERR_SUCCESS != ret)
			{
				roRes.set_errorcode(KKSG::ErrorCode(ret));
				return;
			}

			// item
			BagTakeItemTransition taketransition(role);
			taketransition.SetReason(ItemFlow_UseItem, ItemFlow_TransOhter);
			if (!taketransition.TakeItem(itemid, 1))
			{
				taketransition.RollBack();
				roRes.set_errorcode(KKSG::ERR_TRANS_ITEMNOTENOUGH);
				return;
			}
			taketransition.NotifyClient();

			other->Get<BuffRecord>()->AddTransBuff(buff.buffID, buff.BuffLevel, role->GetID());
		}

		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		return;
	}
	else if (2 == roArg.op())
	{
		///> 获取变身的剩余时间
		UINT32 timeleft = role->Get<BuffRecord>()->GetTransTime();
		roRes.set_timeleft(timeleft);
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{ 
		roRes.set_errorcode(KKSG::ERR_FAILED); 
	} 
}  

void RpcC2G_TransformOp::OnDelayReplyRpc(const TransformOpArg &roArg, TransformOpRes &roRes, const CUserData &roUserData)
{

}
