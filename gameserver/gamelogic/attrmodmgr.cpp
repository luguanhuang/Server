#include "pch.h"
#include "attrmodmgr.h"
#include "gamelogic/tarjamgr.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/attributepk.h"
#include "scene/scenecustompk.h"
#include "scene/scenecustompktwo.h"
#include "table/PowerPointMgr.h"
#include "unit/attributewatcher.h"

INSTANCE_SINGLETON(AttrModMgr);

bool AttrModMgr::Init()
{
	return LoadFile();
}

void AttrModMgr::Uninit()
{

}

void AttrModMgr::ClearFile()
{
	m_table.Clear();
	m_sceneid2data.clear();
	m_scenetype2data.clear();
    m_PveAttrModifyTable.Clear();
    m_PveAttrModify.clear();
}

bool AttrModMgr::CheckFile()
{
	AttrModifyTable temptable;
	if (!temptable.LoadFile("table/AttrModify.txt"))
	{
		SSWarn<<"load file table/AttrModify.txt failed"<<END;
		return false;
	}
    PVEAttrModify tempPveAttrModifyTable;
    if (!tempPveAttrModifyTable.LoadFile("table/PVEAttrModify.txt"))
    {
        SSWarn<<"load file table/PVEAttrModify.txt failed"<<END;
        return false;
    }
	return true;
}

bool AttrModMgr::LoadFile()
{
	ClearFile();
	if (!CheckFile())
	{
		return false;
	}
	m_table.LoadFile("table/AttrModify.txt");

	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		AttrModifyTable::RowData* data = *i;
		for (auto j = data->SceneType.begin(); j != data->SceneType.end(); ++j)
		{
			m_scenetype2data[*j] = data;
		}
		for (auto k = data->SceneID.begin(); k != data->SceneID.end(); ++k)
		{
			m_sceneid2data[*k] = data;
		}
	}

    m_PveAttrModifyTable.LoadFile("table/PVEAttrModify.txt");
    for(auto itor = m_PveAttrModifyTable.Table.begin(); itor != m_PveAttrModifyTable.Table.end(); itor++)
    {
        PVEAttrModify::RowData* data = *itor;
        if(NULL == data) continue;
        m_PveAttrModify[data->SceneID] = data;
    }
	return true;
}

void AttrModMgr::GetSceneInfo(bool& isfair, UINT32& scenetype, Scene* scene)
{
	isfair = AttributePkMgr::Instance()->IsUseSceneType(scenetype);
	///> 全民pk，玩家自定义公平模式
	if (NULL != scene->GetCustomPkHandler())
	{
		isfair = scene->GetCustomPkHandler()->IsFairMode();
		scenetype = KKSG::SCENE_PK;
	}
	if (NULL != scene->GetCustomPkTwoHandler())
	{
		isfair = scene->GetCustomPkTwoHandler()->IsFairMode();
		scenetype = KKSG::SCENE_PKTWO;
	}
}

AttrModifyTable::RowData* AttrModMgr::GetData(Scene* scene)
{
	auto i = m_sceneid2data.find(scene->GetSceneTemplateID());
	if (i != m_sceneid2data.end())
	{
		return i->second;
	}
	auto j = m_scenetype2data.find(scene->GetSceneType());
	if (j != m_scenetype2data.end())
	{
		return j->second;
	}
	return NULL;
}

void AttrModMgr::PveModify(Role* pRole, Scene* pScene)
{
    if(NULL == pRole) 
    {
        return;
    }
    if(NULL == pScene) 
    {
        return;
    }

    auto itor000 = m_PveAttrModify.find(0);
    if(m_PveAttrModify.end() == itor000) 
    {
        return;
    }
    PVEAttrModify::RowData* pRowData000 = itor000->second;
    if(NULL == pRowData000) 
    {
        return;
    }

    UINT32 sceneid = pScene->GetSceneTemplateID();

    auto itorPveAttrModify = m_PveAttrModify.find(sceneid);
    if(m_PveAttrModify.end() == itorPveAttrModify) 
    {
        return;
    }
    PVEAttrModify::RowData* pRowData = itorPveAttrModify->second;
    if(NULL == pRowData) 
    {
        return;
    }

    double playerPowerPoint = pRole->GetAttr(TOTAL_POWERPOINT);
    double combat = pRowData->Combat;
    double ratioAtk = pRowData->RatioA;
    double ratioDef = pRowData->RatioB;
    if(playerPowerPoint >= combat)
    {
        ratioAtk = pRowData->RatioC;
        ratioDef = pRowData->RatioD;
    }
    double tmpPPT = (combat - playerPowerPoint) / combat;

    AttributeWatcher watcher(pRole);

    double tmp = pRole->GetAttr(BASIC_PhysicalAtk) + pRowData->PhysicalAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->PhysicalAtk * pRowData->PhysicalAtk / 100.f);
    watcher.SetAttr(BASIC_PhysicalAtk, tmp);
    watcher.SetAttr(BASIC_PhysicalAtkMod, tmp);

    tmp = pRole->GetAttr(BASIC_MagicAtk) + pRowData->MagicAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->MagicAtk * pRowData->MagicAtk / 100.f);
    watcher.SetAttr(BASIC_MagicAtk, tmp);
    watcher.SetAttr(BASIC_MagicAtkMod, tmp);

    tmp = pRole->GetAttr(BASIC_Critical) + pRowData->Critical * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->Critical * pRowData->Critical / 100.f);
    watcher.SetAttr(BASIC_Critical, tmp);

    tmp = pRole->GetAttr(BASIC_CritResist) + pRowData->CritResist * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->CritResist * pRowData->CritResist / 100.f);
    watcher.SetAttr(BASIC_CritResist, tmp);

    tmp = pRole->GetAttr(BASIC_FireAtk) + pRowData->FireAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->FireAtk * pRowData->FireAtk / 100.f);
    watcher.SetAttr(BASIC_FireAtk, tmp);

    tmp = pRole->GetAttr(BASIC_WaterAtk) + pRowData->WaterAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->WaterAtk * pRowData->WaterAtk / 100.f);
    watcher.SetAttr(BASIC_WaterAtk, tmp);

    tmp = pRole->GetAttr(BASIC_LightAtk) + pRowData->LightAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->LightAtk * pRowData->LightAtk / 100.f);
    watcher.SetAttr(BASIC_LightAtk, tmp);

    tmp = pRole->GetAttr(BASIC_DarkAtk) + pRowData->DarkAtk * tmpPPT / ratioAtk;
    tmp = pvemax(tmp, pRowData000->DarkAtk * pRowData->DarkAtk / 100.f);
    watcher.SetAttr(BASIC_DarkAtk, tmp);

    tmp = pRole->GetAttr(BASIC_PhysicalDef) + pRowData->PhysicalDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->PhysicalDef * pRowData->PhysicalDef / 100.f);
    watcher.SetAttr(BASIC_PhysicalDef, tmp);
    watcher.SetAttr(BASIC_PhysicalDefMod, tmp);

    tmp = pRole->GetAttr(BASIC_MagicDef) + pRowData->MagicDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->MagicDef * pRowData->MagicDef / 100.f);
    watcher.SetAttr(BASIC_MagicDef, tmp);
    watcher.SetAttr(BASIC_MagicDefMod, tmp);

    tmp = pRole->GetAttr(BASIC_FireDef) + pRowData->FireDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->FireDef * pRowData->FireDef / 100.f);
    watcher.SetAttr(BASIC_FireDef, tmp);

    tmp = pRole->GetAttr(BASIC_WaterDef) + pRowData->WaterDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->WaterDef * pRowData->WaterDef / 100.f);
    watcher.SetAttr(BASIC_WaterDef, tmp);

    tmp = pRole->GetAttr(BASIC_LightDef) + pRowData->LightDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->LightDef * pRowData->LightDef / 100.f);
    watcher.SetAttr(BASIC_LightDef, tmp);

    tmp = pRole->GetAttr(BASIC_DarkDef) + pRowData->DarkDef * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->DarkDef * pRowData->DarkDef / 100.f);
    watcher.SetAttr(BASIC_DarkDef, tmp);

    tmp = pRole->GetAttr(BASIC_MaxHP) + pRowData->HP * tmpPPT / ratioDef;
    tmp = pvemax(tmp, pRowData000->HP * pRowData->HP / 100.f);
    watcher.SetAttr(BASIC_MaxHP, tmp);
    double currentHP = pRole->GetAttr(TOTAL_MaxHP);
    watcher.SetAttr(BASIC_CurrentHP, currentHP);
    LogInfo("PVE Attr MAXHP %f, currentHP %f", tmp, currentHP);

    watcher.CheckChangedAttributeAndNotify(false);
}

double AttrModMgr::pvemax(double a, double b)
{
    return a>b?a:b;
}

double AttrModMgr::GetPer(double value, std::vector<Sequence<float, 3>>& valuetemplate)
{
	double per = 1.0f;
	for (auto i = valuetemplate.begin(); i != valuetemplate.end(); ++i)
	{
		if (value >= i->seq[0] && value <= i->seq[1])
		{
			per = i->seq[2];
			break;
		}
	}
	return per;
}

void AttrModMgr::Modify(Role* role, Scene* scene)
{
	AttrModifyTable::RowData* data = GetData(scene);
	if (NULL != data)
	{
		AttributeWatcher watcher(role);
		double per = 1.0;
		//PhysicalAtk;
		double atk = role->GetAttr(BASIC_PhysicalAtk);
		per = GetPer(atk, data->PhysicalAtk);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_PhysicalAtk, std::pow(atk, per));
			watcher.SetAttr(BASIC_PhysicalAtkMod, std::pow(atk, per));
		}
		//PhysicalDef;
		double def = role->GetAttr(BASIC_PhysicalDef);
		per = GetPer(def, data->PhysicalDef);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_PhysicalDef, std::pow(def, per));
			watcher.SetAttr(BASIC_PhysicalDefMod, std::pow(def, per));
		}
		//MaxHP;
		double hp = role->GetAttr(BASIC_MaxHP);
		per = GetPer(hp, data->MaxHP);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_MaxHP, std::pow(hp, per));
		}
		//MagicAtk;
		double matk = role->GetAttr(BASIC_MagicAtk);
		per = GetPer(matk, data->MagicAtk);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_MagicAtk, std::pow(matk, per));
			watcher.SetAttr(BASIC_MagicAtkMod, std::pow(matk, per));
		}
		//MagicDef;
		double mdef = role->GetAttr(BASIC_MagicDef);
		per = GetPer(mdef, data->MagicDef);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_MagicDef, std::pow(mdef, per));
			watcher.SetAttr(BASIC_MagicDefMod, std::pow(mdef, per));
		}
		//Critical;
		double cri = role->GetAttr(BASIC_Critical);
		per = GetPer(cri, data->Critical);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_Critical, std::pow(cri, per));
		}
		//CritResist;
		double critre = role->GetAttr(BASIC_CritResist);
		per = GetPer(critre, data->CritResist);
		if (1.0 != per)
		{
			watcher.SetAttr(BASIC_CritResist, std::pow(critre, per));
		}
		watcher.CheckChangedAttributeAndNotify(false);
	}
}

///> 进场景属性缩放
void AttrModMgr::ModifyAttr(Role* poRole, Scene* poScene)
{
	bool isfair = false;
	UINT32 attrscenetype = poScene->GetSceneType();
	GetSceneInfo(isfair, attrscenetype, poScene);
	if(isfair)
	{
		///> 公平模式
		AttributePkMgr::Instance()->SetPkAttribute(poRole, attrscenetype);
	}
	else
	{
		///> 塔姐 pve
		double ppt = TarjaMgr::Instance()->SetPveAttribute(poRole, poScene);
		if (0 == ppt)
		{
			ppt = poRole->GetAttr(TOTAL_POWERPOINT);
		}

        // pve 战力压制
        PveModify(poRole, poScene);

		///> 战力压制 - new
		Modify(poRole, poScene);

		///> 战力压制 - old
		double recommendpower = poScene->GetSceneInfo()->m_pConf->RecommendPower;
		if (0.0l != recommendpower)
		{
			float atkcoeff = PowerPointMgr::Instance()->GetAtkCoeffcient(poRole->GetLevel(), ppt, recommendpower);
			float defcoeff = PowerPointMgr::Instance()->GetDefCoeffcient(poRole->GetLevel(), ppt, recommendpower);

			if (poScene->GetSceneInfo()->m_pConf->PPTCoff != 0)
			{
				atkcoeff = atkcoeff * poScene->GetSceneInfo()->m_pConf->PPTCoff;
				defcoeff = defcoeff * poScene->GetSceneInfo()->m_pConf->PPTCoff;
			}
			PowerPointMgr::Instance()->AdjustAttributeByCoeffcient(poRole, atkcoeff, defcoeff);
		}
		poRole->SetCombatUseLevel(0);
	}
}
