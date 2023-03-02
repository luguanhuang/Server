#include "pch.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/ranklist.h"
#include "firstpass/firstpassmgr.h"
#include "ranklist/firstpassranklist.h"
#include "ranklist/herobattleranklist.h"
#include "ranklist/military_ranklist.h"

// generate by ProtoGen at date: 2016/8/25 16:21:42

RPC_SERVER_IMPLEMETION(RpcG2M_UpdateMSRankList, UpdateMSRankListArg, UpdateMSRankListRes)

	void RpcG2M_UpdateMSRankList::OnCall(const UpdateMSRankListArg &roArg, UpdateMSRankListRes &roRes)
{
	if(!roArg.has_info())
	{
		return;
	}
	roRes.set_error(KKSG::ERR_SUCCESS);
	const KKSG::RankData& data = roArg.info();
	switch(roArg.ranktype())
	{
	case KKSG::FirstPassRank:
		{
			CFirstPassMgr::Instance()->UpdataFirstPassRank(roArg, roRes);/*roArg.info().rank() <= 20*/
			break;
		}
	case KKSG::SpritePowerPointRank:
		{	
			CRankListMgr::Instance()->OnChangeSpritePowerPoint(data.roleid(), data.powerpoint(), data.time());
			break;
		}
	case KKSG::PetPowerPointRank:
		{
			if (data.has_petuid())
			{
				CRankListMgr::Instance()->OnChangePetPowerPoint(data.petuid(), data.petid(), data.roleid(), data.powerpoint(), data.time());
			}
			break;
		}
	case KKSG::TowerRank:
		{
			std::vector<UINT64> roleids;
			for (int i = 0; i < data.roleids_size(); ++i)
			{
				roleids.push_back(data.roleids(i));
			}
			if (roleids.empty())
			{
				break;
			}

			CRankListMgr::Instance()->OnChangeTowerRank(roleids, data.has_towerhardlevel(), data.towerfloor(), data.towerthroughtime(), data.time());

			break;
		}
	case KKSG::HeroBattleRank:
		{
			HeroBattleRankList* ranklist = static_cast<HeroBattleRankList*>(CRankListMgr::Instance()->GetRankList(KKSG::HeroBattleRank));
			ranklist->OnChange(data.roleid(), data.time(), data.heroinfo());
			break;
		}
	case KKSG::MilitaryRank:
		{
			if (data.has_military_info())
			{
				MilitaryRankList* ranklist = static_cast<MilitaryRankList*>(CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank));
				ranklist->OnChange(
					data.roleid(), 
					data.military_info().military_exploit(),
					data.military_info().military_rank(),
					data.military_info().total_num(),
					data.time());
			}
			break;
		}
	default:
		break;
	}
}

void RpcG2M_UpdateMSRankList::OnDelayReplyRpc(const UpdateMSRankListArg &roArg, UpdateMSRankListRes &roRes, const CUserData &roUserData)
{
}
