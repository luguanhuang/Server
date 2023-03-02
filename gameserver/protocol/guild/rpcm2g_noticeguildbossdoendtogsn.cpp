#include "pch.h"
#include "guild/rpcm2g_noticeguildbossdoendtogsn.h"
#include "guild/guildboss.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "foreach.h"
// generate by ProtoGen at date: 2016/7/30 21:47:11

RPC_SERVER_IMPLEMETION(RpcM2G_NoticeGuildBossDoEndToGSN, NoticeGuildBossDoEndArg, NoticeGuildBossDoEndRes)

void RpcM2G_NoticeGuildBossDoEndToGSN::OnCall(const NoticeGuildBossDoEndArg &roArg, NoticeGuildBossDoEndRes &roRes)
{
	//const std::unordered_map<UINT64, vector<GuildBossInfo*> >& infoMap = GuildBossMgr::Instance()->GetInfoMap();
	//foreach (i in infoMap)
	//{
		/*
		if (i->s != NULL)
		{
			KKSG::GuildBossResultData* pData = roRes.add_resultdata();
			pData->set_guildid(i->second->GetGuildId());
			pData->set_endtime(i->second->GetEndTime());
			pData->set_bossindex(i->second->GetBossIndex());
			pData->set_bossname(GuildBossMgr::Instance()->GetBossAllName(i->second->GetGuildId()));
			pData->set_damage(i->second->IsDead() ? i->second->GetBossHp() : i->second->GetAllDamage());
			*/
			//if (!i->second->IsDead()) // 击杀  伤害数据走击杀协议
			//{
				/*foreach (j in i->second->m_roleid2damage)
				{
					KKSG::guildBossDamage* pDamage = pData->add_unitdamage();
					pDamage->set_roleid(j->first);
					pDamage->set_damage(j->second);
				}*/
			//}
		//}
	//}

	GuildBossMgr::Instance()->DoGiveReward();  // 时间结束发放参与奖
	GuildBossMgr::Instance()->ClearAfterEnd(); // 清理
}

void RpcM2G_NoticeGuildBossDoEndToGSN::OnDelayReplyRpc(const NoticeGuildBossDoEndArg &roArg, NoticeGuildBossDoEndRes &roRes, const CUserData &roUserData)
{
}
