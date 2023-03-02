#include "pch.h"
#include "DragonExpConfig.h"
#include "unit/role.h"
#include "pb/project.pb.h"
#include "gamelogic/stagemgr.h"
#include "SceneTable.h"
#include "sceneconfig.h"
#include "stage/ptcg2m_deprogresssyn.h"
#include "mslink.h"
#include "define/systemiddef.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/stagerankmgr.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"
#include "define/tlogenum.h"

INSTANCE_SINGLETON(DragonExpConfig)

bool DragonExpConfig::Init()
{
	if (!m_deTable.LoadFile("table/DragonExpList.txt"))
	{
		SSWarn<<"load file table/DragonExpList.txt failed"<<END;
		return false;
	}

	for(size_t i = 0; i < m_deTable.Table.size(); ++i)
	{
		UINT32 sceneID = m_deTable.Table[i]->SceneID;
		UINT32 groupID = SceneConfig::Instance()->GetSceneGroupID(sceneID);
		if(groupID)
		{
			m_groupsceneid = sceneID;
			break;
		}
	}

	if(0 == m_groupsceneid)
	{
		SSError << " find not scene groupid " << END;
	}

	return true;
}

void DragonExpConfig::Uninit()
{

}

DragonExpConfig::DragonExpConfig()
{
	m_groupsceneid = 0;
}

DragonExpConfig::~DragonExpConfig()
{

}

void DragonExpConfig::FillProgress(Role* pRole, KKSG::DEProgressRes &roRes)
{
	if(NULL == pRole)
	{
		return ;
	}
	UINT32 fightSceneID = GetFightSceneID(pRole);
	for(size_t i = 0; i < m_deTable.Table.size(); ++i)
	{
		DragonExpList::RowData* pConf = m_deTable.Table[i];
		UINT32 sceneid = pConf->SceneID;
		int bossavghppercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(sceneid);
		KKSG::DEProgress *pro = roRes.add_allpro();	
		pro->set_sceneid(sceneid);
		pro->set_bossavghppercent(bossavghppercent);
		if(0 == bossavghppercent)
		{
			pro->set_state(KKSG::DEPS_FINISH);

			if(pRole->Get<StageMgr>()->GetRank(sceneid) <= 0)//线上问题处理
			{
				pRole->Get<StageMgr>()->SetRank(sceneid, RANK_SSS);
				SSWarn << " dragon exp online err handle role = " << pRole->GetID() << " sceneid = " << sceneid << END;
			}
		}
		else if(sceneid == fightSceneID)
		{
			pro->set_state(KKSG::DEPS_FIGHT);
		}
		else
		{
			pro->set_state(KKSG::DEPS_NOTOPEN);
		}
		//SSDebug << __FUNCTION__ << " sceneid = " << pro->sceneid() << " bossavghp = " << pro->bossavghppercent() << " state = " << (int)(pro->state()) << END;
	}
	SceneInfo* pSceneInfo = SceneConfig::Instance()->FindSceneInfo(m_groupsceneid);
	if(pSceneInfo)
	{
		roRes.set_leftcount(pRole->Get<StageMgr>()->GetCanPlayCount(pSceneInfo));
		roRes.set_allcount(pSceneInfo->m_pConf->DayLimit);
		SSDebug << __FUNCTION__ << " leftcount = " << roRes.leftcount() << " allcount = " << roRes.allcount() << END;
	}
	roRes.set_serverseallevel(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel);
}

UINT32 DragonExpConfig::GetNextSceneID(UINT32 curSceneID)
{
	if(m_deTable.Table.empty())
	{
		return 0;
	}
	if(0 == curSceneID)
	{
		return m_deTable.Table[0]->SceneID;
	}

	for(size_t i = 0; i + 1 < m_deTable.Table.size(); ++i)
	{
		if(m_deTable.Table[i]->SceneID == curSceneID)
		{
			return m_deTable.Table[i+1]->SceneID;
		}
	}

	return 0;// if cursceneid is last one return 0 too.
}

UINT32 DragonExpConfig::GetFightSceneID(Role* pRole)
{
	if(NULL == pRole)
	{
		return 0;
	}
	if(m_deTable.Table.empty())
	{
		return 0;
	}
	if(!IsSystemOpened(pRole))
	{
		return 0;		
	}
	UINT32 fightSceneID = 0;
	for(size_t i = 0; i < m_deTable.Table.size(); ++i)
	{
		int hppercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(m_deTable.Table[i]->SceneID);
		if(hppercent > 0)
		{
			return m_deTable.Table[i]->SceneID;
		}
	}
	return 0;//已通过
}

void DragonExpConfig::OnSystemOpened(Role* pRole)
{
	if(pRole)
	{
		SynProgressToMS(pRole);
	}
}

void DragonExpConfig::SynProgressToMS(Role* pRole, bool isbattleend/* = false*/)
{
	SSDebug << __FUNCTION__ << " enter fun " << END;
	if(NULL == pRole)
	{
		return ;
	}
	UINT32 synSceneID = GetFightSceneID(pRole);
	if(0 == synSceneID && IsSystemOpened(pRole))
	{
		synSceneID = GetLastSceneID();
	}
	if(synSceneID)
	{
		int bosshpavgpercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(synSceneID);

		PtcG2M_DEProgressSyn ptc;
		ptc.m_Data.mutable_prosyn()->set_sceneid(synSceneID);
		ptc.m_Data.mutable_prosyn()->set_bossavghppercent(bosshpavgpercent);
		ptc.m_Data.set_roleid(pRole->GetID());
		ptc.m_Data.set_isbattleend(isbattleend);

		SSDebug << __FUNCTION__ << " sceneid " << synSceneID << " hppercent = " << bosshpavgpercent << END;

		pRole->SendToMS(ptc);
	}
}

UINT32 DragonExpConfig::GetLastSceneID()
{
	if(m_deTable.Table.empty())
	{
		return 0;
	}
	int n = (int)m_deTable.Table.size();
	return m_deTable.Table[n-1]->SceneID;
}

bool DragonExpConfig::IsSystemOpened(Role* pRole)
{
	if(pRole && pRole->Get<CRoleSystem>()->IsSystemOpened(DragonExpSysID))
	{
		return true;		
	}
	return false;
}

void DragonExpConfig::GetJoinReward(UINT32 sceneID, std::vector<Sequence<UINT32,2>>& rewards)
{
	DragonExpList::RowData* pConf = m_deTable.GetBySceneID(sceneID);
	if(pConf)
	{
		for(size_t i = 0; i < pConf->ChallengeReward.size(); ++i)
		{
			rewards.push_back(pConf->ChallengeReward[i]);
		}
	}
}

void DragonExpConfig::GetWinReward(UINT32 sceneID, std::vector<Sequence<UINT32,2>>& rewards)
{
	DragonExpList::RowData* pConf = m_deTable.GetBySceneID(sceneID);
	if(pConf)
	{
		for(size_t i = 0; i < pConf->WinReward.size(); ++i)
		{
			rewards.push_back(pConf->WinReward[i]);
		}
	}
}

void DragonExpConfig::ResetProgress(Role* pRole, UINT32 sceneid)
{
	if(NULL == pRole)
	{
		return ;
	}
	SSWarn << " only use for test " << " sceneid = " << sceneid << " role = " << pRole->GetID() << END;
	for(size_t i = 0; i < m_deTable.Table.size(); ++i)
	{
		UINT32 confsid = m_deTable.Table[i]->SceneID;
		if(confsid >= sceneid)
		{
			pRole->Get<StageMgr>()->ClearProgress(confsid);
		}
		else
		{
			pRole->Get<StageMgr>()->ClearProgress(confsid);
			pRole->Get<StageMgr>()->SetBossHpProgress(confsid, 1, 0);
		}
	}
	SynProgressToMS(pRole);
}

void DragonExpConfig::GetSealBuff(UINT32& buffid, UINT32& bufflevel, UINT32 sceneid)
{
	buffid = 0;
	bufflevel = 0;
	auto pconf = m_deTable.GetBySceneID(sceneid);
	if(pconf && pconf->SealLevel > CLevelSealMgr::Instance()->GetLevelSealInfo().m_uLevel)
	{
		buffid = pconf->SealBuff[0];
		bufflevel = pconf->SealBuff[1];
	}
}

void DragonExpConfig::OnRoleLogin(Role* pRole)
{
	//if(NULL == pRole)
	//{
	//	return;
	//}
	//if(!IsSystemOpened(pRole))
	//{
	//	return;
	//}
	//UINT32 sceneID = GetLastSceneID();
	//if(0 == sceneID)
	//{
	//	return;
	//}
	//int hppercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(sceneID);
	//if(0 == hppercent)//已通关
	//{
	//	if(pRole->Get<StageMgr>()->GetPlayedCount(sceneID) > 0)//用来标记有没有发过
	//	{
	//		return ;
	//	}
	//	const auto pConf = MailConfig::Instance()->GetMailConf(MailConf_DragonExp_TongGuan);
	//	if(NULL == pConf)
	//	{
	//		return;
	//	}
	//	ItemDesc item;
	//	item.itemID = GetGlobalConfig().DETongGuanReward[0];
	//	item.itemCount = GetGlobalConfig().DETongGuanReward[1];

	//	MailSender mailsender;
	//	mailsender.SetContent(Mail_System, pConf->m_title, pConf->m_content);
	//	mailsender.AddItem(item);
	//	mailsender.SetReason(ItemFlow_Stage, ItemFlow_DragonExp_TongGuan);

	//	mailsender.Send(pRole->GetID());

	//	pRole->Get<StageMgr>()->AddScenePlayCount(sceneID);
	//}
}

UINT32 DragonExpConfig::GetSceneGroupID()
{
	return SceneConfig::Instance()->GetSceneGroupID(m_groupsceneid);
}

bool DragonExpConfig::IsDESceneGroup(UINT32 groupID)
{
	return groupID == GetSceneGroupID();
}
