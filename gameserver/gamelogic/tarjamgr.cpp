#include "pch.h"
#include "unit/role.h"
#include "tarjamgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "unit/rolemanager.h"
#include "unit/systeminfo.h"
#include "define/itemdef.h"
#include "bagtransition.h"
#include "stagemgr.h"
#include "unit/unit.h"
#include "unit/attributewatcher.h"
#include "unit/role.h"
#include "unit/dummyrole.h"
#include "buff/XBuff.h"
#include "entity/XRole.h"
#include "component/XBuffComponent.h"
#include "scene/scene.h"
#include "scene/sceneteam.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "table/expeditionconfigmgr.h"
#include "table/ExpeditionTable.h"
#include "table/OpenSystemMgr.h"
#include "unit/systeminfo.h"
#include "item.h"

INSTANCE_SINGLETON(TarjaMgr);

TarjaMgr::TarjaMgr():m_nMaxAward(0){}

TarjaMgr::~TarjaMgr(){}


bool TarjaMgr::Init()
{
	bool bRet = LoadFile();
	return bRet;
}

void TarjaMgr::Uninit()
{
	ClearFile();
}

void TarjaMgr::ClearFile()
{
	m_Table.Clear();
	m_nMaxAward = 0;
	m_mapPveAttr.clear();
}

bool TarjaMgr::LoadFile()
{
	ClearFile();
	bool bRet = m_Table.LoadFile("table/PveProfession.txt");
	for (size_t i = 0; i < m_Table.Table.size(); i++)
	{
		UINT64 key = MakeUINT64(m_Table.Table[i]->ProfessionID,m_Table.Table[i]->SceneID);
		m_mapPveAttr.insert(std::pair<UINT64,PveProfessionTable::RowData*>(key,m_Table.Table[i]));
	}
	for(size_t j = 0; j <  GetGlobalConfig().PveGeraldGiftReward.size(); j++)
	{
		if (GetGlobalConfig().PveGeraldGiftReward[j][2]>m_nMaxAward)
		{
			m_nMaxAward = GetGlobalConfig().PveGeraldGiftReward[j][2];
		}
	}
	return bRet;
}


double TarjaMgr::SetPveAttribute(Unit* pUnit, Scene *pScene) 
{

	if (GetGlobalConfig().TarjaSwitch==0)
	{
		return 0;
	}

	if(NULL == pUnit||pUnit->IsRole()==false)
		return 0;
	Role *pRole = (Role*)pUnit;
	PveProfessionTable::RowData* pData = GetConfig(pRole);
	if (pData == NULL)
		return 0;
	StageMgr *pStage =  pRole->Get<StageMgr>();
	time_t nTime = GameTime::GetTime();
	bool bAdd = false;	
	if(GetGlobalConfig().PveGeraldGiftOpen==0)
	{
		if (pScene->IsHelper(pRole->GetID())==false)
		{	
			if (pStage->GetTarjaTime() > nTime)
			{
				bAdd =  true;
				pScene->SetTarja(pRole->GetID());
			}
		}
		else
		{
			if (pStage->GetTarjaTime() > nTime)
			{
				pScene->SetTarja(pRole->GetID());
				if (GetGlobalConfig().PveGeraldGiftAssistRequire==1)
				{
					bAdd =  true;
				}
			}
		}
	}
	else
	{
		bAdd =  true;
		pScene->SetTarja(pRole->GetID());
	}
	if (bAdd == false)
		return 0;

	double ppt = 0;
	SceneTeam* pSceneTeam = pScene->GetSceneTeam();
	if (pSceneTeam!=NULL)
	{		
		UINT32 nDneID = pSceneTeam->GetExpID();
		ExpeditionTable::RowData *pDne = ExpeditionConfigMgr::Instance()->GetExpeditionConf(nDneID);
		if(pDne!=NULL)
		{
			if (pRole->GetAttr(TOTAL_POWERPOINT) >=  pDne->DisplayPPT)
			{
				return 0;
			}
			else
			{
				ppt = pDne->DisplayPPT;
			}
		}
	}

	AttributeWatcher watcher(pUnit);
	watcher.SetAttr(BASIC_PhysicalAtk, pData->PhysicalAtk);
	watcher.SetAttr(PERCENT_PhysicalAtk, 0.0);
	watcher.SetAttr(BASIC_PhysicalAtkMod, pData->PhysicalAtk);
	watcher.SetAttr(PERCENT_PhysicalAtkMod, 0.0);
	watcher.SetAttr(BASIC_PhysicalDef, pData->PhysicalDef);
	watcher.SetAttr(PERCENT_PhysicalDef, 0.0);
	watcher.SetAttr(BASIC_PhysicalDefMod, pData->PhysicalDef);
	watcher.SetAttr(PERCENT_PhysicalDefMod, 0.0);
	watcher.SetAttr(BASIC_MaxHP, pData->MaxHP);
	watcher.SetAttr(PERCENT_MaxHP, 0.0);
	watcher.SetAttr(BASIC_HPRecovery, pData->HPRecovery);
	watcher.SetAttr(PERCENT_HPRecovery, 0.0);
	watcher.SetAttr(BASIC_MagicAtk, pData->MagicAtk);
	watcher.SetAttr(PERCENT_MagicAtk, 0.0);
	watcher.SetAttr(BASIC_MagicAtkMod, pData->MagicAtk);
	watcher.SetAttr(PERCENT_MagicAtkMod, 0.0);
	watcher.SetAttr(BASIC_MagicDef, pData->MagicDef);
	watcher.SetAttr(PERCENT_MagicDef, 0.0);
	watcher.SetAttr(BASIC_MagicDefMod, pData->MagicDef);
	watcher.SetAttr(PERCENT_MagicDefMod, 0.0);
	watcher.SetAttr(BASIC_MaxMP, pData->MaxMP);
	watcher.SetAttr(PERCENT_MaxMP, 0.0);
	watcher.SetAttr(BASIC_MPRecovery, pData->MPRecovery);
	watcher.SetAttr(PERCENT_MPRecovery, 0.0);
	watcher.SetAttr(BASIC_Critical, pData->Critical);
	watcher.SetAttr(PERCENT_Critical, 0.0);
	watcher.SetAttr(BASIC_CritResist, pData->CritResist);
	watcher.SetAttr(PERCENT_CritResist, 0.0);
	const auto& AttrValue = pData->AttrValue_s;
	for(size_t i = 0; i < AttrValue.size(); ++i)
	{
		int attr = (int)AttrValue[i][0];
		double value = AttrValue[i][1];
		watcher.SetAttr(CombatAttrDef(attr), value);
	}
	double hpMax = pUnit->GetAttr(TOTAL_MaxHP);
	watcher.SetAttr(BASIC_CurrentHP, hpMax);
	double mpMax = pUnit->GetAttr(TOTAL_MaxMP);
	watcher.SetAttr(BASIC_CurrentMP, mpMax);
	watcher.CheckChangedAttributeAndNotify(false);

	if (pData->BuffID.Size()==2)
	{
		XAddBuffActionArgs args;
		BuffDesc buff;
		buff.buffID = pData->BuffID[0];
		buff.BuffLevel = pData->BuffID[1];
		args.vecBuffs.push_back(buff);
		pRole->AddReviveBuff(buff);
		args.Caster = pRole->GetID();
		args.bClientOnly = !pScene->IsSyncScene();
		pRole->GetXObject()->DispatchAction(&args);
	}
	return ppt;
}

PveProfessionTable::RowData* TarjaMgr::GetConfig(Role *pRole)
{
	if (pRole ==  NULL)
		return NULL;
	Scene *pScene  =  pRole->GetCurrScene();
	if (pScene == NULL)
		return NULL;
	int profession =  pRole->GetProfession();
	UINT64 key = MakeUINT64(profession,pScene->GetSceneTemplateID());
	auto iter = m_mapPveAttr.find(key);
	if (iter==m_mapPveAttr.end())
		return NULL;
	return iter->second;
}



void TarjaMgr::GiveTarjaAward(Role *pRole,Scene *pScene)
{
	if (GetGlobalConfig().TarjaSwitch==0)
		return;
	PveProfessionTable::RowData* pRow = GetConfig(pRole);
	if (pRow == NULL)
		return;
	StageMgr *pStage =  pRole->Get<StageMgr>();
	if(pStage->GetTarjaAward()>m_nMaxAward)
		return;

	bool bMail = false;
	if(pScene->IsHelper(pRole->GetID())==true)
	{
		if (GetGlobalConfig().PveGeraldGiftAssistRewardOpen==false)
		{
			return;
		}
		if (pScene->GetTarja(pRole->GetID()) != 0)
		{
			bMail = true;
		}
	}else
	{
		if (pScene->GetTarja(pRole->GetID())!=0)
		{
			bMail =  true;
		}
	}

	if (bMail == false)
		return;
	UINT32 nAward = pStage->GetTarjaAward();
	UINT32 nLevel = pRole->GetLevel();
	for (size_t i = 0;i < GetGlobalConfig().PveGeraldGiftReward.size(); i++)
	{
		if (nLevel>=GetGlobalConfig().PveGeraldGiftReward[i][0]&&nLevel<GetGlobalConfig().PveGeraldGiftReward[i][1]&&nAward<GetGlobalConfig().PveGeraldGiftReward[i][4])
		{
			UINT32 nRand = XRandom::randInt(0, GetGlobalConfig().PveGeraldGiftReward[i][3]);
			if (nRand < GetGlobalConfig().PveGeraldGiftReward[i][3])
			{
				const MailConf* conf = MailConfig::Instance()->GetMailConf(GetGlobalConfig().PveGeraldGiftReward[i][2]);
				if (conf)
				{
					MailSender mailsender;
					mailsender.SetContent(Mail_System, conf->m_title, conf->m_content);
					mailsender.SetReason(ItemFlow_TrajaAward);
					mailsender.AddConfigItem(conf);
					mailsender.Send(pRole->GetID());
					pStage->SetTarjaAward(nAward+1);
				}
			}
			break;
		}
	}

}
