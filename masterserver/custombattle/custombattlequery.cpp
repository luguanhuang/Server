#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"


void CustomBattleHandleQuery::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	switch(req.op)
	{
	///> 系统战斗
	case KKSG::CustomBattle_Query:
		{
			// trans to world query
			errorcode = m_manager->TransReqToWorld(req);
			break;
		}
	///> 自己战斗
	case KKSG::CustomBattle_QuerySelf:
		{
			SCustomBattleRole* rolebattle = CustomBattleRoleMgr::Instance()->TryGetInfo(req.roleid);
			if (NULL == rolebattle || NULL == rolebattle->m_battlejoin)
			{
				ErrorNotify(KKSG::ERR_SUCCESS, req.delayid);
				return;
			}
			CustomBattleReq tempreq = req;
			tempreq.uid = rolebattle->GetJoinId();
			CustomBattle* battle = m_manager->GetBattle(rolebattle->GetJoinId());
			if (NULL == battle)
			{
				///> 本服找不到，到world上找
				// trans to world query
				errorcode = m_manager->TransReqToWorld(tempreq);
			}
			else
			{
				KKSG::CustomBattleClientInfo info;
				battle->ToKKSGBrief(info.mutable_joininfo()->mutable_data());
				battle->FillRole(tempreq.uid, tempreq.roleid, *info.mutable_joininfo()->mutable_role());
				m_manager->FillTag(battle, info.mutable_joininfo()->mutable_data(), battle->GetCreator(), tempreq.roleid);
				DelayReply(KKSG::ERR_SUCCESS, &info, tempreq.delayid);	
				return;
			}
			break;
		}
	///> 任意信息
	case KKSG::CustomBattle_QueryOne:
		{
			CustomBattle* battle = m_manager->GetBattle(req.uid);
			if (NULL == battle)
			{
				///> 本服找不到，到world上找
				errorcode = m_manager->TransReqToWorld(req);
			}
			else
			{
				KKSG::CustomBattleClientInfo info;
				battle->ToKKSGWithRank(info.mutable_queryinfo()->mutable_battleone()->mutable_data());
				m_manager->FillTag(battle, info.mutable_queryinfo()->mutable_battleone()->mutable_data(), battle->GetCreator(), req.roleid);
				DelayReply(KKSG::ERR_SUCCESS, &info, req.delayid);	
				return;
			}
			break;
		}
	///> 随机信息
	case KKSG::CustomBattle_QueryRandom:
		{
			SCustomBattleRole* rolebattle = CustomBattleRoleMgr::Instance()->TryGetInfo(req.roleid);
			UINT64 joinuid = (NULL == rolebattle) ? 0 : rolebattle->GetJoinId();		
			// local handle
			//if (!req.iscross)
			//{
			//	KKSG::CustomBattleClientInfo info;
			//	m_manager->FillRandomBattle(info.mutable_queryinfo(), joinuid, req.roleid);
			//	DelayReply(KKSG::ERR_SUCCESS, &info, req.delayid);	
			//}
			//else 
			//{
			//	CustomBattleReq tempreq = req;
			//	tempreq.uid = joinuid;
			//	errorcode = m_manager->TransReqToWorld(tempreq);
			//}
			///> 先去world取官方比赛
			CustomBattleReq tempreq = req;
			tempreq.uid = joinuid;
			tempreq.iscross = req.iscross;
			errorcode = m_manager->TransReqToWorld(tempreq);
			break;
		}
	default:
		break;
	}
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
}

void CustomBattleHandleQuery::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	switch(req.op)
	{
	///> 系统战斗
	case KKSG::CustomBattle_Query:
		{
			KKSG::CustomBattleClientInfo tempinfo;
			SCustomBattleRole* rolebattle = CustomBattleRoleMgr::Instance()->TryGetInfo(req.roleid);
			///> 系统
			for (int i = 0; i < info.queryinfo().battlesystem_size(); ++i)
			{
				KKSG::CustomBattleDataRole* datarole = tempinfo.mutable_queryinfo()->add_battlesystem();
				*datarole = info.queryinfo().battlesystem(i);
				if (NULL != rolebattle)
				{
					rolebattle->FillSystemRole(datarole->mutable_data()->uid(), datarole->mutable_role());
				}
			}

			DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);	

			temperrorcode = KKSG::ERR_SUCCESS;
			break;
		}
	///> 自定义战斗
	case KKSG::CustomBattle_QuerySelf:
		{
			KKSG::CustomBattleClientInfo tempinfo;
			///> 自定义
			if (info.has_joininfo())
			{
				///> 跨服的
				KKSG::CustomBattleDataRole* datarole = tempinfo.mutable_joininfo();
				*datarole = info.joininfo();
				DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);	
				return;
			}
			else
			{
				temperrorcode = KKSG::ERR_FAILED;
			}
			break;
		}
	///> 任意信息
	case KKSG::CustomBattle_QueryOne:
		{
			if (info.queryinfo().has_battleone())
			{
				///> world上找到了
				DelayReply(KKSG::ERR_SUCCESS, &info, req.delayid);	
				temperrorcode = KKSG::ERR_SUCCESS;
			}
			else
			{
				///> world上也没找到
				temperrorcode = KKSG::ERR_CUSTOMBATTLE_BATTLENOTFIND;
			}
			break;
		}
	///> 跨服比赛
	case KKSG::CustomBattle_QueryRandom:
		{
			KKSG::CustomBattleClientInfo tempinfo = info;
			if (!req.iscross)
			{
				m_manager->FillRandomBattle(tempinfo.mutable_queryinfo(), req.uid, req.roleid);
			}
			DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);	
			temperrorcode = KKSG::ERR_SUCCESS;
			break;
		}
	default:
		break;
	}
	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
	}
}
