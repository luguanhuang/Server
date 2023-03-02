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
#include "friend/friendop.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

void CustomBattleHandleSearch::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	///> 搜索
	//if (req.iscross)
	//{
		///> 到world上找
		//errorcode = m_manager->TransReqToWorld(req);
	//}

	///> 先到world上找
	errorcode = m_manager->TransReqToWorld(req);

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
}

void CustomBattleHandleSearch::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	// name
	std::vector<UINT64>* battles = NULL;
	if (!req.battlename.empty())
	{
		battles = m_manager->GetBattleByName(req.battlename);
	}
	KKSG::CustomBattleClientInfo result = info;
	if (NULL != battles)
	{
		for (auto i = battles->begin(); i != battles->end(); ++i)
		{
			CustomBattle* battle = m_manager->GetBattle(*i);
			if (NULL != battle)
			{
				///> 有社会关系的 不能被陌生人搜到
				if (battle->IsFriendOnly() || battle->IsGuildOnly())
				{
					bool isrelated = false;
					if (battle->GetCreator() == req.roleid)
					{
						isrelated = true;
					}
					///> 同好友
					if (!isrelated && battle->IsFriendOnly())
					{
						FriendOp op(req.roleid);
						if (op.IsFriend(battle->GetCreator()))
						{
							isrelated = true;
						}
					}
					///> 同公会
					if (!isrelated && battle->IsGuildOnly())
					{
						Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(battle->GetCreator());
						if (NULL != guild)
						{
							if (guild->IsPlayerInGuild(req.roleid))
							{
								isrelated = true;
							}
						}
					}
					if (!isrelated)
					{
						continue;
					}
				}
				KKSG::CustomBattleDataRole* datarole = result.add_searchinfo();
				battle->ToKKSGBrief(datarole->mutable_data());
				m_manager->FillTag(battle, datarole->mutable_data(), battle->GetCreator(), req.roleid);
			}
		}

	}
	if (0 == result.searchinfo_size())
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_BATTLENOTFIND, req.delayid);
	}
	else
	{
		DelayReply(KKSG::ERR_SUCCESS, &result, req.delayid);	
	}
}
