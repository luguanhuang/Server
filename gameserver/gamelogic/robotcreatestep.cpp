#include "pch.h"
#include "robotcreatestep.h"
#include "unit/role.h"
#include "bag.h"
#include "itemconfig.h"
#include "rolefashion.h"
#include "bagtransition.h"
#include "enhanceequipmgr.h"
#include "util/XRandom.h"
#include "table/XSkillEffectMgr.h"
#include "skilllevel.h"
#include "globalconfig.h"

void RobotGenBasicStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	UINT32 prof = pConf->RobotProfession;
	if (pConf->RobotProfession == 0)
	{
		prof = XRandom::randInt(1,5);

		// 一转
		if (pConf->RobotLevel >=  GetGlobalConfig().GetInt("Promote1", 30))
		{
			if (XRandom::randInt(0, 100) > 50)
			{
				prof += 10;
			}
			else
			{
				prof += 20;
			}
		}

		// 二转
		if (pConf->RobotLevel >=  GetGlobalConfig().GetInt("Promote2", 100))
		{
			if (XRandom::randInt(0, 100) > 50)
			{
				prof += 100;
			}
			else
			{
				prof += 200;
			}
		}
	}

	if (pRobot->GetID() == 0)
	{
		if (pConf->RobotName.empty())
		{
			pRobot->Init(prof, RobotConfig::Instance()->CreateRandomName());
		}
		else
		{
			pRobot->Init(prof, pConf->RobotName);
		}
	}
	else
	{
		pRobot->SetProfession(prof);
	}
	
	pRobot->SetLevel(pConf->RobotLevel);
	pRobot->SetCamp(pConf->RobotCamp);
}

void RobotGenEquipStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	Bag *pBag = pRobot->Get<Bag>();
	for (UINT32 i = 0; i < pConf->Equip.size(); ++i)
	{
		UINT32 itemID = pConf->Equip[i];
		if (itemID != 0)
		{
			ItemConf  *pConf = ItemConfig::Instance()->GetItemConf(itemID);
			if (NULL == pConf)
			{
				SSWarn<<"item id:"<<itemID<<" not find"<<END;
				return;
			}	

			///> 装备原型，把装备ID转换成对应职业的装备
			if (pConf->ItemType == EQUIP_PROTO)
			{
				UINT32 profItemID = EquipProtoCalc::ConvertToProfessionItemID(itemID, pRobot->GetProfession());

				pConf = ItemConfig::Instance()->GetItemConf(profItemID);
				if (pConf == NULL)
				{
					LogError("equip item %u not found! convert from equip_proto %u", profItemID, itemID);
					return;
				}

				itemID = profItemID;
			}

			pBag->AddItemToEquipPosition(itemID, i);
		}
	}
}

void RobotGenSkillStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	RoleSkillSet *pSkill = pRobot->Get<RoleSkillSet>();
	pSkill->DebugClearSkill();
	pRobot->AddSkillPoint(SKILL_POINT, pRobot->GetLevel()*2);

	int prof = pRobot->GetProfession();

	{
		const auto &vec = XSkillEffectMgr::Instance()->GetProfessionSkill(prof%10);
		for (UINT32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i].UnlockLevel <= pConf->RobotLevel)
			{
				pSkill->TryLevelup(vec[i].SkillHash);
			}
		}
	}
	
	if (prof > 10)
	{
		const auto &vec = XSkillEffectMgr::Instance()->GetProfessionSkill(prof%100);
		for (UINT32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i].UnlockLevel <= pConf->RobotLevel)
			{
				pSkill->TryLevelup(vec[i].SkillHash);
			}
		}
	}

	if (prof > 100)
	{
		const auto &vec = XSkillEffectMgr::Instance()->GetProfessionSkill(prof);
		for (UINT32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i].UnlockLevel <= pConf->RobotLevel)
			{
				pSkill->TryLevelup(vec[i].SkillHash);
			}
		}
	}

	pSkill->DumpPrintSkill();
}

void RobotGenEnhanceStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	Bag *pBag = pRobot->Get<Bag>();
	UINT32 n = pConf->Enhance.size();
	if (n > EquipCount)
	{
		n = EquipCount;
	}

	for (UINT32 i = 0; i < n; ++i)
	{
		XItem* pItem = pBag->GetItemOnBody(EQUIP, i);
		if (pItem)
		{
#if OLD_ENHANCE
			CEnhanceMgr::Instance()->DebugLevel(pRobot, pItem, pConf->Enhance[i]);
#endif
			EnhanceEquipMgr::Instance()->DebugLevel(pRobot, pItem, pConf->Enhance[i]);
		}
	}
}

void RobotGenFashionShowStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	RoleFashion *pFashion = pRobot->Get<RoleFashion>();
	for (UINT32 i = 0; i < pConf->FashionShow.size(); ++i)
	{
		if (pConf->FashionShow[i] != 0)
		{
			pFashion->Wear(pConf->FashionShow[i]);
		}
	}
}

void RobotGenItemStep::DoWork(RobotConf *pConf, Role *pRobot)
{
	BagGiveItemTransition transition(pRobot);
	transition.SetDoLog(false);
	for (UINT32 i = 0; i < pConf->Item.size(); ++i)
	{
		transition.GiveItem((UINT32)pConf->Item[i].seq[0], (UINT32)pConf->Item[i].seq[1], true);
	}
	//机器人，不调用NotifyClient
}

