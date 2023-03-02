#include "pch.h"
#include "ProfessionMgr.h"
#include "util/XCommon.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "gamelogic/activityrecord.h"
#include "table/globalconfig.h"
#include "pb/project.pb.h"
#include "unit/combatattrdef.h"

INSTANCE_SINGLETON(ProfessionMgr)

ProfessionMgr::ProfessionMgr()
{

}

ProfessionMgr::~ProfessionMgr()
{
	for(auto it = m_oConvertCoefficients.begin(); it != m_oConvertCoefficients.end(); ++it)
	{
		delete it->second;
	}
}

static int GetConvertParameKey(int profID, int attrID)
{
	profID %= 10; // 获得基础职业类型 profID个位数表示基础职业 比如 114 ---> 4
	return (profID << 16) | attrID;
}

bool ProfessionMgr::Init()
{
	if (!m_oGrowthTable.LoadFile("table/ProfessionGrowthPercent.txt"))
	{
		LogWarn("load file table/ProfessionGrowthPercent.txt failed!");
		return false;
	}

	if (!m_oConvertTable.LoadFile("table/ProfessionConversionParameter.txt"))
	{
		LogWarn("load file table/ProfessionConversionParameter.txt failed!");
		return false;
	}

	int keyID;
	for (UINT32 j = 0; j < m_oConvertTable.Table.size(); ++j)
	{
		auto cvRow = m_oConvertTable.Table[j];
		keyID = GetConvertParameKey(cvRow->ProfessionID, cvRow->AttributeID);
		m_oConvertIndex[keyID] = cvRow;

		ConvertCoefficientList* pList = new ConvertCoefficientList();
		m_oConvertCoefficients[keyID] = pList;

		ConvertCoefficientList& oList = *pList;
		if (cvRow->PhysicalAtk != 0)
			oList[BASIC_PhysicalAtk] = cvRow->PhysicalAtk;
		if (cvRow->PhysicalDef != 0)
			oList[BASIC_PhysicalDef] = cvRow->PhysicalDef;
		if (cvRow->MagicAtk != 0)
			oList[BASIC_MagicAtk] = cvRow->MagicAtk;
		if (cvRow->MagicDef != 0)
			oList[BASIC_MagicDef] = cvRow->MagicDef;
		if (cvRow->Critical != 0)
			oList[BASIC_Critical] = cvRow->Critical;
		if (cvRow->CritResist != 0)
			oList[BASIC_CritResist] = cvRow->CritResist;
		if (cvRow->Stun != 0)
			oList[BASIC_Stun] = cvRow->Stun;
		if (cvRow->StunResist != 0)
			oList[BASIC_StunResist] = cvRow->StunResist;
		if (cvRow->Paralyze != 0)
			oList[BASIC_Paralyze] = cvRow->Paralyze;
		if (cvRow->ParaResist != 0)
			oList[BASIC_ParaResist] = cvRow->ParaResist;
		if (cvRow->MaxHP != 0)
			oList[BASIC_MaxHP] = cvRow->MaxHP;
		if (cvRow->MaxMP != 0)
			oList[BASIC_MaxMP] = cvRow->MaxMP;
		if (cvRow->CombatScore != 0)
			oList[BASIC_CombatSocre] = cvRow->CombatScore;
		if (cvRow->CritDamage != 0)
			oList[BASIC_CritDamage] = cvRow->CritDamage;
	}
	return true;
}

void ProfessionMgr::Uninit()
{

}

ProfessionConvertTable::RowData * ProfessionMgr::GetProfessionAttrConvertParams(int profID, int attrID)
{
	auto i = m_oConvertIndex.find(GetConvertParameKey(profID, attrID));
	return (i == m_oConvertIndex.end()) ? NULL : i->second;
}

const ConvertCoefficientList* ProfessionMgr::GetProfessionAttrConvertCoefficients(int profID, int attrID)
{
	auto i = m_oConvertCoefficients.find(GetConvertParameKey(profID, attrID));
	return (i == m_oConvertCoefficients.end()) ? NULL : i->second;
}

ProfessionGrowthPercentTable::RowData * ProfessionMgr::GetProfessionGrowthPercentConf(int profID)
{
	for (UINT32 i = 0; i < m_oGrowthTable.Table.size(); ++i)
	{
		if (m_oGrowthTable.Table[i]->ProfessionID == profID)
		{
			return m_oGrowthTable.Table[i];
		}
	}

	return NULL;
}

int ProfessionMgr::GetBasicProfession(int Prof)
{
	return Prof%10;
}

// 转职规则：假设职业ID为 4  
//1转   id+10  = 10+4 =14 
//      id+20  = 20+4 =24
//2转    
//      id +110  = 110 + 4 = 114
//      id +120  = 120 + 4 = 124 
//		id +210  = 210 + 4 = 214 
//	    id +220  = 220 + 4 = 224			
// 由此可知，大于100的id为2转，与100取余即为转之前id
bool ProfessionMgr::CanPromote(int FromProf, int ToProf)
{
	if (ToProf > 100)
	{
		return (ToProf % 100) == FromProf;
	}
	else if (ToProf > 10)
	{
		return (ToProf % 10) == FromProf;
	}
	else
	{
		return false;
	}
}

const std::string& ProfessionMgr::GetPrefabName(int profID)
{
	int basic_id = GetBasicProfession(profID);
	return XEntityInfoLibrary::Instance()->TemplateID2Prefab_Role(basic_id);
}

int ProfessionMgr::PromoteLevel(int Prof)
{
	if (Prof < 10)
	{
		return 1;
	}
	else if (Prof < 100)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

int ProfessionMgr::CheckProtmote(int FromProf, int ToProf, int Level)
{
	// 检查能否转到次职业
	if (!CanPromote(FromProf, ToProf))
	{
		return KKSG::ERR_PROF_ERROR;
	}

	// 检查此次是第几次选择职业,以次核对是否达到了转换职业的等级  
	int Lv = PromoteLevel(ToProf);
	if (Lv == 2 && Level <  GetGlobalConfig().GetInt("Promote1", 0))
	{
		return KKSG::ERR_PROF_LEVELREQ;
	}

	if (Lv == 3 && Level <  GetGlobalConfig().GetInt("Promote2", 0))
	{
		return KKSG::ERR_PROF_LEVELREQ2;
	}

	return KKSG::ERR_SUCCESS;
}

bool ProfessionMgr::IsBasicProfession(int Prof)
{
	return Prof <= 10;
}

void ProfessionMgr::GetAllProfession(std::vector<UINT32>& profs)
{
	for(auto i = m_oGrowthTable.Table.begin(); i != m_oGrowthTable.Table.end(); i++)
	{
		profs.push_back((*i)->ProfessionID);
	}
}