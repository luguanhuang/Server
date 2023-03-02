#include "pch.h"
#include "pk/rpcm2g_pkreqm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "pb/project.pb.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/pkmgr.h"
#include "gamelogic/bagtransition.h"
#include "idip/idiprecord.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/8/25 20:15:29

RPC_SERVER_IMPLEMETION(RpcM2G_PkReqM2G, PkReqM2GArg, PkReqM2GRes)

void RpcM2G_PkReqM2G::OnCall(const PkReqM2GArg &roArg, PkReqM2GRes &roRes)
{
	const PkReqArg& c2mArg = roArg.c2marg();
	PkReqRes& c2mRes = *roRes.mutable_c2mres();

	Role* pRole = RoleManager::Instance()->FindBySession(roArg.sessionid());
	if(NULL == pRole)//在其它GS
	{
		c2mRes.set_errorcode(ERR_FAILED);
		return ;
	}

	pRole->Get<CPkRecord>()->Update();

	ErrorCode ret = ERR_SUCCESS;
	switch(c2mArg.type())
	{
	case PKREQ_ADDPK:
		{
			UINT32 point = pRole->Get<CPkRecord>()->GetPoint();
			c2mRes.set_time(PkMgr::Instance()->GetPrepareTime(point));
			roRes.set_addrobot(pRole->Get<CPkRecord>()->NeedAddRobot());
			pRole->Get<CPkRecord>()->FillPkRec(*roRes.mutable_pkrec(), KKSG::PK_1v1);
			roRes.set_rolelevel(pRole->GetLevel());

			if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(PUNISH_USER_DAILY_PLAY) || 
				pRole->Get<CIdipRecord>()->CheckIdipAndNotify(PUNISH_USER_MULTI_ACTIVITY))
			{
				ret = ERR_FAILED;
			}
			if(!pRole->Get<CRoleSystem>()->IsSystemOpened(PkId))
			{
				ret = ERR_FAILED;//系统未开启
			}
		}
		break;
	case PKREQ_REMOVEPK:
		{
			;//no code
		}
		break;
	case PKREQ_ALLINFO:
		{
			CPkRecord* pRecord = pRole->Get<CPkRecord>();
			pRecord->Update();
			QueryPkInfoRes* allinfo = c2mRes.mutable_allinfo();
			allinfo->set_rewardcount(pRecord->GetRewardCount());//TODO ASK

			PkRecord* recinfo = allinfo->mutable_info();
			pRecord->ToKKSG(recinfo);

			//box reset for client
			UINT32 maxPoint = pRecord->GetMaxPoint();
			std::set<UINT32> boxtaken;
			for(int i = 0; i < recinfo->boxtaken_size(); ++i)
			{
				boxtaken.insert(recinfo->boxtaken(i));
			}
			recinfo->clear_boxtaken();
			PkPointTable& table = PkMgr::Instance()->GetPointTable();
			for(size_t i = 0; i < table.Table.size(); ++i)
			{
				UINT32 state = 0;
				PkPointTable::RowData* rowdata = table.Table[i];
				if(maxPoint >= rowdata->point)
				{
					auto j = boxtaken.find(i);
					state = (j == boxtaken.end())? 1 : 2;
				}
				recinfo->add_boxtaken(state);
			}

		}
		break;
	case PKREQ_FETCHPOINTREWARD:
		{
			UINT32 index = c2mArg.index();
			CPkRecord* pRecord = pRole->Get<CPkRecord>();
			pRecord->Update();
			if(pRecord->IsBoxTaken(index))
			{
				ret = ERR_ARENA_REWARDTAKEN;
				break;
			}

			std::vector<ItemDesc> items;
			PkMgr::Instance()->GetPointReward(pRecord->GetMaxPoint(), index, items);
			if (items.empty())
			{
				ret = ERR_UNKNOWN;
				break;
			}

			pRecord->SetBoxTaken(index);

			BagGiveItemTransition transition(pRole);	
			transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pk);
			for (auto i = items.begin(); i != items.end(); ++i)
			{
				transition.GiveItem(*i);
			}
			transition.NotifyClient();
		}
		break;
	default:
		break;
	}

	UINT32 point = pRole->Get<CPkRecord>()->GetPoint();
	roRes.set_point(point);

	c2mRes.set_errorcode(ret);
}

void RpcM2G_PkReqM2G::OnDelayReplyRpc(const PkReqM2GArg &roArg, PkReqM2GRes &roRes, const CUserData &roUserData)
{
}
