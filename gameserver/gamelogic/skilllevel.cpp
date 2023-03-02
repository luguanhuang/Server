#include "pch.h"
#include "skilllevel.h"
#include "pb/project.pb.h"
#include "table/XSkillEffectMgr.h"
#include "table/ProfessionMgr.h"
#include "unit/combatattribute.h"
#include "unit/role.h"
#include "util/XCommon.h"
#include "bagtransition.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"
#include "rolesummarymgr.h"
#include "foreach.h"
#include "scene/scene.h"
#include "skill/XSkillCore.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"
#include "utility/tlogger.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/PlayerLevelManager.h"
#include "skill/ptcg2c_transskillnotfiy.h"

RoleSkillSet::RoleSkillSet(Role *pRole)
{
	m_pListener = NULL;
	m_pRole = pRole;
	m_freeresetskill = false;
	m_skillindex = 0;

	for (UINT32 i = 0; i < SKILL_SET_MAX; ++i)
	{
		m_skillsets[i].m_skillslot.resize(KKSG::Attack_Max);
		for (UINT32 j = 0; j < m_skillsets[i].m_skillslot.size(); ++j)
		{
			m_skillsets[i].m_skillslot[j] = 0;
		}
	}

	m_skills = &m_skillsets[m_skillindex].m_skills;
	m_skillslot = &m_skillsets[m_skillindex].m_skillslot;
	m_pEntity = NULL;

	m_canleveltrans = false;
	m_transformskillpoint = 0;
}

RoleSkillSet::~RoleSkillSet()
{

}

static int GetUpSkillNeedValue(SkillList::RowData* pRow, int skillLevel)
{
	int needValue = 0;
	if (skillLevel < (int)pRow->LevelupCost.size())
	{
		needValue = pRow->LevelupCost[skillLevel];
	}
	else
	{
		if (pRow->LevelupCost.empty())
		{
			needValue = 0;
		}
		else
		{
			needValue = pRow->LevelupCost.back();
		}
	}

	return needValue;
}

struct SkillEqual
{
	UINT32 skillhash;
	SkillEqual(UINT32 id) : skillhash(id){}
	bool operator()(const skillInfo &o) const
	{
		return o.skillhash == skillhash;
	}
};

struct SkillInfoSaver
{
	KKSG::SkillRecord* m_record;

	SkillInfoSaver(KKSG::SkillRecord* record, RoleSkillSet* skillset) : m_record(record)
	{
		m_record->Clear();
		m_record->set_freeresetskill(skillset->IsFreeResetSkill());
		m_record->set_index(skillset->GetSkillSetIndex());
	}
	void operator()(const skillInfo &skill)
	{
		KKSG::SkillInfo* pSkill = m_record->add_skills();
		pSkill->set_skillhash(skill.skillhash);
		pSkill->set_skilllevel(skill.skilllevel);
		pSkill->set_skillpoint(skill.skillpoint);
		if (skill.isbasic)
		{
			pSkill->set_isbasic(true);
		}
	}
	void operator()(const UINT32 v)
	{
		m_record->add_skillslot(v);
	}
};

struct SkillInfoSaverTwo
{
	KKSG::SkillRecord* m_record;

	SkillInfoSaverTwo(KKSG::SkillRecord* record, RoleSkillSet* skillset) : m_record(record)
	{
		m_record->set_freeresetskill(skillset->IsFreeResetSkill());
		m_record->set_index(skillset->GetSkillSetIndex());
	}
	void operator()(const skillInfo &skill)
	{
		KKSG::SkillInfo* pSkill = m_record->add_skillstwo();
		pSkill->set_skillhash(skill.skillhash);
		pSkill->set_skilllevel(skill.skilllevel);
		pSkill->set_skillpoint(skill.skillpoint);
		if (skill.isbasic)
		{
			pSkill->set_isbasic(true);
		}
	}
	void operator()(const UINT32 v)
	{
		m_record->add_skillslottwo(v);
	}
};

void RoleSkillSet::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		m_pRole->SetSkillPoint(SKILL_POINT, PlayerLevelManager::Instance()->GetSkillPoint(m_pRole->GetLevel(), true));
		m_pRole->SetSkillPoint(SKILL_POINT_TWO, PlayerLevelManager::Instance()->GetSkillPoint(m_pRole->GetLevel(), true));
	}
	InitSkill *pInitSkill = XEntityInfoLibrary::Instance()->GetInitSkill(m_pRole->GetProfession());
	if (pInitSkill)
	{
		LearnInitSkill(pInitSkill);
		m_modified.Set();
	}
}

bool RoleSkillSet::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_skill())
	{
		const KKSG::SkillRecord& record = poRoleAllInfo->skill();

		m_freeresetskill = record.freeresetskill();
		m_skillindex = record.index();

		// set one
		int n = record.skills_size();
		for (int i = 0; i < n; ++i)
		{
			const KKSG::SkillInfo &rskill = record.skills(i);
			skillInfo o;
			o.skillhash = rskill.skillhash();
			o.skilllevel = rskill.skilllevel();
			o.skillpoint = rskill.skillpoint();
			o.isbasic = rskill.isbasic();

			auto j = find_if(m_skillsets[0].m_skills.begin(), m_skillsets[0].m_skills.end(), SkillEqual(o.skillhash));
			if(j == m_skillsets[0].m_skills.end())
			{
				m_skillsets[0].m_skills.push_back(o);
			}
		}
		if (record.skillslot_size() > (int)m_skillsets[0].m_skillslot.size())
		{
			m_skillsets[0].m_skillslot.resize(record.skillslot_size());
		}
		for (int j = 0; j < record.skillslot_size(); ++j)
		{
			m_skillsets[0].m_skillslot[j] = record.skillslot(j);
		}
		// set two
		n = record.skillstwo_size();
		for (int i = 0; i < n; ++i)
		{
			const KKSG::SkillInfo &rskill = record.skillstwo(i);
			skillInfo o;
			o.skillhash = rskill.skillhash();
			o.skilllevel = rskill.skilllevel();
			o.skillpoint = rskill.skillpoint();
			o.isbasic = rskill.isbasic();

			auto j = find_if(m_skillsets[1].m_skills.begin(), m_skillsets[1].m_skills.end(), SkillEqual(o.skillhash));
			if(j == m_skillsets[1].m_skills.end())
			{
				m_skillsets[1].m_skills.push_back(o);
			}
		}
		if (record.skillslottwo_size() > (int)m_skillsets[1].m_skillslot.size())
		{
			m_skillsets[1].m_skillslot.resize(record.skillslottwo_size());
		}
		for (int j = 0; j < record.skillslottwo_size(); ++j)
		{
			m_skillsets[1].m_skillslot[j] = record.skillslottwo(j);
		}
	}

	m_skills = &m_skillsets[m_skillindex].m_skills;
	m_skillslot = &m_skillsets[m_skillindex].m_skillslot;

	RefreshSkillFlags();

	m_modified.TestAndReset();

	CheckInit();

	return true;
}

void RoleSkillSet::FillRecord(KKSG::SkillRecord* record)
{
   	SkillInfoSaver skillSaver(record, this);
   	std::for_each(m_skillsets[0].m_skills.begin(), m_skillsets[0].m_skills.end(), skillSaver);
   	std::for_each(m_skillsets[0].m_skillslot.begin(), m_skillsets[0].m_skillslot.end(), skillSaver);

   	SkillInfoSaverTwo skillSaverTwo(record, this);
   	std::for_each(m_skillsets[1].m_skills.begin(), m_skillsets[1].m_skills.end(), skillSaverTwo);
   	std::for_each(m_skillsets[1].m_skillslot.begin(), m_skillsets[1].m_skillslot.end(), skillSaverTwo);
}

void RoleSkillSet::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_modified.TestAndReset())
	{
		KKSG::SkillRecord* record = poRoleAllInfo->mutable_skill();
		record->Clear();
		FillRecord(record);
		roChanged.insert(record);
	}
}

int RoleSkillSet::BindSkillSlot(UINT32 slot, UINT32 skillhash)
{
	if (slot >= m_skillslot->size())
	{
		return KKSG::ERR_BIND_SKILL_OUTRANGE;
	}

	if ((*m_skillslot)[slot] == skillhash)
	{
		m_modified.Set();
		return KKSG::ERR_SUCCESS;
	}

	///> 重置技能
	if (skillhash == 0)
	{
		m_modified.Set();
		(*m_skillslot)[slot] = skillhash;
		return KKSG::ERR_SUCCESS;
	}

	///> 没学会技能
	if (FindSkillInfo(skillhash) == NULL)
	{
		return KKSG::ERR_BIND_SKILL_NOT_LEARN;
	}

	if (!XSkillEffectMgr::Instance()->CanBindSkillInSlot(skillhash, slot))
	{
		return KKSG::ERR_BIND_SKILL_MISSSLOT;
	}

	///> 如果装备pre技能，找ex技能
	UINT32 exhash = XSkillEffectMgr::Instance()->GetExSkill(skillhash, 0);
	if (0 != exhash)
	{
		if (NULL != FindSkillInfo(exhash))
		{
			skillhash = exhash;
		}
	}

	m_modified.Set();

	for(unsigned i = 0; i < m_skillslot->size(); ++i)
	{
		///> 交换技能
		if ((*m_skillslot)[i] == skillhash)
		{
			LogInfo("bind skill swap %u to slot %d", skillhash, slot);
			(*m_skillslot)[i] = (*m_skillslot)[slot];
			(*m_skillslot)[slot] = skillhash;
			return KKSG::ERR_SUCCESS;
		}
	}

	LogInfo("bind skill %u to slot %d", skillhash, slot);

	///> 设置技能
	(*m_skillslot)[slot] = skillhash;
	if (m_pListener) m_pListener->OnSkillSlotChanged();

	return KKSG::ERR_SUCCESS;
}

void RoleSkillSet::ClearBindSkill()
{
	m_modified.Set();
	///> 0 是 普通攻击
	///> 1 是 闪避 
	for (UINT32 i = 2; i < m_skillslot->size(); ++i)
	{
		(*m_skillslot)[i] = 0;
	}

	if (m_pListener) m_pListener->OnSkillSlotChanged();
}

bool RoleSkillSet::CheckProfession(UINT32 expectProfession, UINT32 realProfession)
{
	if (0 == expectProfession
		|| expectProfession == realProfession
		|| expectProfession == (realProfession % 10)
		|| expectProfession == (realProfession % 100))
	{
		return true;
	}
	return false;
}

SkillSetSyncToOtherPlayer RoleSkillSet::GetSyncSkill()
{
	SkillSetSyncToOtherPlayer syncSkill;

	foreach (i in (*m_skills))
	{
		syncSkill.Add(*i);
	}
	foreach (j in (*m_skillslot))
	{
		syncSkill.AddBind(*j);
	}
	return syncSkill;
}

UINT32 RoleSkillSet::GetUsedSkillPoint()
{
	UINT32 skillPoint = 0;
	foreach(i in (*m_skills))
	{
		if (i->isbasic)
		{
			continue;
		}

		skillPoint += i->skillpoint;
	}
	return skillPoint;
}

void RoleSkillSet::CastEnterSceneSkill(Scene* scene)
{
	if (SceneConfig::Instance()->IsBattleSceneExcept(scene->GetSceneType()))
	{
		return;
	}
	///> 筛选Ex技能 
	std::map<UINT32, UINT32> exs;
	std::map<UINT32, std::pair<UINT32, SkillList::RowData*>> all;
	for (auto i = m_skills->begin(); i != m_skills->end(); ++i)
	{
		if (i->isbasic)
		{
			continue;
		}
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, i->skilllevel);
		if (pRow != NULL && pRow->SkillType == KKSG::Skill_SceneBuff)
		{
			UINT32 pre = XSkillEffectMgr::Instance()->GetPreSkill(i->skillhash, 0);
			if (0 != pre)
			{
				exs[i->skillhash] = pre;
			}
			all[i->skillhash] = std::pair<UINT32, SkillList::RowData*>(i->skilllevel, pRow);
		}
	}
	for (auto j = exs.begin(); j != exs.end(); ++j)
	{
		auto k = all.find(j->second);
		if (k != all.end())
		{
			all.erase(k);
		}
	}

	XAddBuffActionArgs args;
	for (auto l = all.begin(); l != all.end(); ++l)
	{
		SkillList::RowData* pRow = l->second.second;
		if (pRow && 0 != pRow->AuraBuffID.seq[0])
		{
			BuffDesc buff(pRow->AuraBuffID[0], pRow->AuraBuffID[1] == 0 ? l->second.first : pRow->AuraBuffID[1], l->first);
			m_pRole->AddReviveBuff(buff);

			args.vecBuffs.push_back(buff);
		}
	}
	args.Caster = m_pRole->GetID();
	args.bClientOnly = !scene->IsSyncScene();
	m_pRole->GetXObject()->DispatchAction(&args);
}

const char *SkillTypeStr(SkillList::RowData* pConf)
{
	switch(pConf->SkillType)
	{
	case KKSG::Skill_None: return "[none skill]";
	case KKSG::Skill_Normal: return "[nomral skill]";
	case KKSG::Skill_Big: return "[big skill]";
	case KKSG::Skill_UnUsed: return "[unused]";
	case KKSG::Skill_SceneBuff: return "[scenebuff]";
	case KKSG::Skill_Help: return "[help skill]";
	case KKSG::Skill_Buff: return "[buff skill]";
	default: return "[unknow skill]";
	}

	return "[unknow skill]";
}

void RoleSkillSet::DumpPrintSkill()
{
	std::stringstream ss;

	foreach(i in (*m_skills))
	{
		auto pConf = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, i->skilllevel, 0);
		ss << pConf->SkillScript << " hash:" << i->skillhash << " level:" << i->skilllevel 
			<< " " << SkillTypeStr(pConf) << "\n";
	}

	LogDebug("Dump Role Skill:\n%s", ss.str().c_str());
}

void RoleSkillSet::DebugClearSkill()
{
	m_skills->clear();
	std::fill(m_skillslot->begin(), m_skillslot->end(), 0);
}
std::string RoleSkillSet::GetLogSkill(std::vector<UINT32>& skillslot)
{
	std::stringstream ss;
	UINT32 j = 0;
	for (UINT32 i = 0; i < skillslot.size(); i ++)
	{
		if (skillslot[i])
		{
			if(j) ss << "#";
			ss << skillslot[i];
			j ++;
		}
	}
	return ss.str();
}
void RoleSkillSet::DoTxLog()
{
	TSkillFlow oLog(m_pRole);
	oLog.m_Page = m_skillindex+1;
	for (UINT32 i = 0; i < SKILL_SET_MAX; i ++)
	{
		if (i == 0)
		{
			oLog.skill1 = GetLogSkill(m_skillsets[0].m_skillslot);
		}
		else if (i == 1)
		{
			oLog.skill2 = GetLogSkill(m_skillsets[0].m_skillslot);
		}
	}
	oLog.Do();
}

void RoleSkillSet::RefreshSkillFlags()
{
	m_SkillFlags.Reset();
	foreach(i in (*m_skills))
	{
		auto pConf = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, i->skilllevel, 0);
		if (pConf == NULL)
			continue;
		m_SkillFlags.SetFlag(pConf->Flag);
	}
}

skillInfo* RoleSkillSet::GetSkillBySlot(UINT32 slot)
{
	if (slot >= m_skillslot->size())
	{
		return NULL;
	}
	UINT32 skillhash = (*m_skillslot)[slot];
	if (0 == skillhash)
	{
		return NULL;
	}
	return FindSkillInfo(skillhash);
}

int RoleSkillSet::GMTryLevelup(UINT32 skillhash)
{
	UINT32 skillLvl = GetSkillOriginLevel(skillhash);

	SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(skillhash, skillLvl + 1); 
	if (pRow == NULL)
	{
		return KKSG::ERR_SKILL_MAXLEVEL;
	}

	if (!CheckProfession(pRow->Profession, m_pRole->GetProfession()))
	{
		return KKSG::ERR_SKILL_PROFESSION_ERROR;
	}
	///> ex技能不能升级
	skillInfo *po = FindSkillInfo(skillhash);
	if (NULL != po && XSkillEffectMgr::Instance()->IsExSkill(skillhash, 0))
	{
		return KKSG::ERR_SKILL_MAXLEVEL;
	}
	int needValue = 0;
	if (po != NULL)
	{
		po->skilllevel += 1;
		po->skillpoint += needValue;

		//LogInfo("Levelup skill %u to level %u", skillhash, po->skilllevel);

		if (m_pListener) 
		{
			m_pListener->OnChangedSkillLevel(skillhash, po->skilllevel);
		}

		///> 如果有ex技能，也需要升级
		UINT32 exskill = XSkillEffectMgr::Instance()->GetExSkill(skillhash, 0);
		if (0 != exskill)
		{
			skillInfo *ex = FindSkillInfo(skillhash);
			if (NULL != ex)
			{
				ex->skilllevel = po->skilllevel;
			}
		}
	}
	else
	{
		// LogInfo("Learn new skill %u", skillhash);

		if(pRow->LevelupCostType == SKILL_POINT)
		{
			AddSkill(skillhash, 1, needValue, false);
		}
		///> 不需要技能点就能学的初始化技能
		else if (needValue == 0 && pRow->LevelupCostType == 0)
		{
			AddSkill(skillhash, 1, needValue, false);
		}
	}

	m_modified.Set();

	RefreshSkillFlags();

	///> 如果学习了ex技能自动替换pre技能
	UINT32 prehash = XSkillEffectMgr::Instance()->GetPreSkill(skillhash, 0);
	if (0 != prehash)
	{
		UINT32 count = 0;
		for (auto i = m_skillslot->begin(); i != m_skillslot->end(); ++i)
		{
			if (prehash == *i)
			{
				BindSkillSlot(count, skillhash);
				break;
			}
			++count;
		}
	}

	return KKSG::ERR_SUCCESS;
}

void RoleSkillSet::RefreshSelfLinkedLevels(Unit* pUnit)
{
	m_LinkedLevels.clear();
	if (pUnit == NULL || m_skills == NULL)
		return;

	for (auto it = m_skills->begin(); it != m_skills->end(); ++it)
	{
		SkillList::RowData* rowData = XSkillEffectMgr::Instance()->GetSkillConfig(it->skillhash, 1, pUnit->GetSkillCasterTypeID());
		if (rowData == NULL)
			continue;

		if (rowData->LinkedSkill != "")
		{
			UINT32 linkedHash = XCommon::XHash(rowData->LinkedSkill.c_str());

			UINT32 linkLevel = GetSkillOriginLevel(linkedHash);
			if (linkLevel != 0)
			{
				m_LinkedLevels[it->skillhash] = linkLevel;
			}
		}
	}
}

void RoleSkillSet::GetEntity()
{
	if(m_pEntity == NULL)
	{
		m_pEntity = m_pRole->GetXRole();
	}
}

UINT32 RoleSkillSet::GetSkillLevel(UINT32 skillhash)
{
	GetEntity();
	///> 变身直接返回1级
	if (m_pEntity != NULL && m_pEntity->IsTransform())
	{
		if (m_canleveltrans)
		{
			auto i = m_transformskills.find(skillhash);
			if (i == m_transformskills.end())
			{
				return 1;
			}
			else
			{
				return 0 == i->second.skilllevel ? 1 : i->second.skilllevel;
			}
		}
		else
		{
			return 1;
		}
	}

	auto it = m_LinkedLevels.find(skillhash);
	if (it != m_LinkedLevels.end())
		return it->second;

	return GetSkillOriginLevel(skillhash);
}

UINT32 RoleSkillSet::GetSkillOriginLevel(UINT32 skillhash)
{
	skillInfo *po = FindSkillInfo(skillhash);
	return (po != NULL) ? po->skilllevel : 0;
}

void RoleSkillSet::AddSkill(UINT32 skillhash, UINT32 skillLevel, UINT32 skillpoint, bool isBasic)
{
	m_modified.Set();
	skillInfo o;
	o.skillhash = skillhash;
	o.skilllevel = skillLevel;
	o.skillpoint = skillpoint;
	o.isbasic = isBasic;
	m_skills->push_back(o);

	if (m_pListener) 
	{
		m_pListener->OnAddSkill(skillhash, skillLevel);
	}
}

skillInfo* RoleSkillSet::FindSkillInfo(UINT32 skillhash)
{
	auto i = find_if(m_skills->begin(), m_skills->end(), SkillEqual(skillhash));
	if(i != m_skills->end())
	{
		return &*i;		
	}
	return NULL;
}

bool RoleSkillSet::ChangeSkillSet(UINT32 index)
{
	if (index >= SKILL_SET_MAX)
	{
		return false;
	}
	if (index == m_skillindex)
	{
		return false;
	}

	m_modified.Set();
	m_skillindex = index;

	m_skills = &m_skillsets[index].m_skills;
	m_skillslot = &m_skillsets[index].m_skillslot;

	RefreshSkillFlags();
	return true;
}

void RoleSkillSet::CheckInit()
{
	InitSkill *pInitSkill = XEntityInfoLibrary::Instance()->GetInitSkill(ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession()));
	if (NULL == pInitSkill)
	{
		return;
	}
	for (UINT32 k = 0; k < pInitSkill->skillhashs.size(); ++k)
	{
		auto info = pInitSkill->skillhashs[k];
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(info.skillhash, 1);
		if (NULL == pRow)
		{
			LogWarn("not find init skill [%s:%u]", XSkillEffectMgr::Instance()->GetSkillName(info.skillhash).c_str(), info.skillhash);
			continue;
		}
		skillInfo* skillinfo = FindSkillInfo(info.skillhash);
		if (NULL != skillinfo)
		{
			continue;
		}

		skillInfo o;
		o.skillhash = info.skillhash;
		o.skilllevel = 1;
		o.skillpoint = GetUpSkillNeedValue(pRow, 0);
		o.isbasic = pRow->IsBasicSkill != 0;
		for (int i = 0; i < SKILL_SET_MAX; ++i)
		{
			m_skillsets[i].m_skills.push_back(o);
			if (info.slot >= 0 && info.slot < (int)m_skillsets[i].m_skillslot.size())
			{
				m_skillsets[i].m_skillslot[info.slot] = info.skillhash;
			}
		}
		m_modified.Set();

		LogDebug("Learn init skill [%s:%u]", XSkillEffectMgr::Instance()->GetSkillName(info.skillhash).c_str(), info.skillhash);
	}
}

void RoleSkillSet::LearnInitSkill(InitSkill *pInitSkill)
{
	for (UINT32 k = 0; k < pInitSkill->skillhashs.size(); ++k)
	{
		auto info = pInitSkill->skillhashs[k];
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(info.skillhash, 1);

		skillInfo* skillinfo = FindSkillInfo(info.skillhash);
		if (NULL != skillinfo)
		{
			continue;
		}

		AddSkill(info.skillhash, 1, GetUpSkillNeedValue(pRow, 0), (pRow->IsBasicSkill != 0));

		if (info.slot >= 0 && info.slot < (int)m_skillslot->size())
		{
			(*m_skillslot)[info.slot] = info.skillhash;
		}

		LogDebug("Learn init skill [%s:%u]", XSkillEffectMgr::Instance()->GetSkillName(info.skillhash).c_str(), info.skillhash);
	}

	for (UINT32 i = 0; i < pInitSkill->bindSkillHashs.size(); ++i)
	{
		auto info = pInitSkill->bindSkillHashs[i];
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(info.skillhash, 1);

		skillInfo* skillinfo = FindSkillInfo(info.skillhash);
		if (NULL != skillinfo)
		{
			continue;
		}

		AddSkill(info.skillhash, 1, GetUpSkillNeedValue(pRow, 0), false);

		if (info.slot >= 0 && info.slot < (int)m_skillslot->size())
		{
			(*m_skillslot)[info.slot] = info.skillhash;
		}

		LogDebug("Learn init skill [%s:%u]", XSkillEffectMgr::Instance()->GetSkillName(info.skillhash).c_str(), info.skillhash);
	}

	m_skillsets[(0 == m_skillindex) ? 1 : 0] = m_skillsets[m_skillindex];

	RefreshSkillFlags();
}

KKSG::ErrorCode RoleSkillSet::ResetSkill()
{
	m_modified.Set();

	ClearBindSkill();

	int SkillPoint = 0;
	for (auto i = m_skills->begin(); i != m_skills->end(); )
	{
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, 1);
		if (NULL == pRow)
		{
			SSError<<"roleid:"<<m_pRole->GetID()<<" skill:"<<i->skillhash<<" not find"<<END;
			CHECK_COND_NORETURN(false);

			m_skills->erase(i++);

			continue;
		}

		SkillPoint += i->skillpoint - pRow->ResetSkillTo;

		if (pRow->ResetSkillTo == 0)
		{
			if (m_pListener) 
			{
				m_pListener->OnRemoveSkill(i->skillhash);
			}

			m_skills->erase(i++);
		}
		else
		{
			i->skillpoint = pRow->ResetSkillTo;
			i->skilllevel = 1;

			if (m_pListener)
			{
				m_pListener->OnChangedSkillLevel(i->skillhash, i->skilllevel);
			}

			++i;
		}
	}
	UINT32 type = GetSkillPointType();
	//m_pRole->AddSkillPoint(type, SkillPoint);
	m_pRole->SetSkillPoint(type, PlayerLevelManager::Instance()->GetSkillPoint(m_pRole->GetLevel(), false) - 1);

	RefreshSkillFlags();
	
	return KKSG::ERR_SUCCESS;
}

void RoleSkillSet::ResetSkillTwo()
{
	m_modified.Set();
	UINT32 index = SKILL_SET_MAX - m_skillindex - 1;
	std::vector<UINT32>* tempskillslot = &m_skillsets[index].m_skillslot;
	SkillInfoList* tempskills = &m_skillsets[index].m_skills;

	///> 0 是 普通攻击
	///> 1 是 闪避 
	for (UINT32 i = 2; i < tempskillslot->size(); ++i)
	{
		(*tempskillslot)[i] = 0;
	}

	int SkillPoint = 0;
	for (auto i = tempskills->begin(); i != tempskills->end(); )
	{
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, 1);
		if (NULL == pRow)
		{
			SSError<<"roleid:"<<m_pRole->GetID()<<" skill:"<<i->skillhash<<" not find"<<END;
			CHECK_COND_NORETURN(false);

			tempskills->erase(i++);

			continue;
		}

		SkillPoint += i->skillpoint - pRow->ResetSkillTo;

		if (pRow->ResetSkillTo == 0)
		{
			tempskills->erase(i++);
		}
		else
		{
			i->skillpoint = pRow->ResetSkillTo;
			i->skilllevel = 1;
			++i;
		}
	}
	UINT32 pointtype = (0 != m_skillindex) ? SKILL_POINT : SKILL_POINT_TWO;
	//m_pRole->AddSkillPoint(pointtype, SkillPoint);
	m_pRole->SetSkillPoint(pointtype, PlayerLevelManager::Instance()->GetSkillPoint(m_pRole->GetLevel(), false) - 1);
}

int RoleSkillSet::TryLevelup(UINT32 skillhash, bool isbind)
{
	UINT32 prehash = XSkillEffectMgr::Instance()->GetPreSkill(skillhash, 0);
	///> 学习Ex技能先学习普通技能
	if (0 != prehash)
	{
		skillInfo* info = FindSkillInfo(prehash);
		if (NULL == info)
		{
			return KKSG::ERR_SKILL_NEED_EXPRESKILL;
		}
	}
	UINT32 skillLvl = GetSkillOriginLevel(skillhash);

	SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(skillhash, skillLvl + 1); 
	if (pRow == NULL)
	{
		return KKSG::ERR_SKILL_MAXLEVEL;
	}

	///> 消耗技能点限制
	if (GetUsedSkillPoint() < (UINT32)pRow->PreSkillPoint)	
	{
		return KKSG::ERR_SKILL_PREPOINTLIMIT;
	}
	///> 技能点判断
	if (0 != pRow->LevelupCostType && pRow->LevelupCostType != SKILL_POINT)
	{
		return KKSG::ERR_FAILED;
	}
	int needValue = GetUpSkillNeedValue(pRow, skillLvl);
	int skillpoint = m_pRole->GetSkillPoint();
	if (skillpoint < needValue)
	{
		return KKSG::ERR_SKILL_POINT;
	}

	if (!CheckProfession(pRow->Profession, m_pRole->GetProfession()))
	{
		return KKSG::ERR_SKILL_PROFESSION_ERROR;
	}
	if (skillLvl == 0 && !pRow->PreSkill.empty())
	{
		UINT32 preskill = (UINT32)XCommon::XHash(pRow->PreSkill.c_str());
		if (GetSkillOriginLevel(preskill) == 0)
		{
			return KKSG::ERR_SKILL_NEED_PRESKILL;
		}
	}

	// Level Request
	int NeedRoleLvl = 0;
	if (!pRow->UpReqRoleLevel.empty())
	{
		NeedRoleLvl = skillLvl >= (int)pRow->UpReqRoleLevel.size() ? pRow->UpReqRoleLevel.back() : pRow->UpReqRoleLevel[skillLvl];
	}
	if (m_pRole->GetLevel() < NeedRoleLvl)
	{
		return KKSG::ERR_SKILL_LEVELREQ;
	}

	///> ex技能不能升级
	skillInfo *po = FindSkillInfo(skillhash);
	if (NULL != po && XSkillEffectMgr::Instance()->IsExSkill(skillhash, 0))
	{
		return KKSG::ERR_SKILL_MAXLEVEL;
	}

	if (!isbind)
	{
		BagTakeItemTransition taketransition(m_pRole);
		taketransition.SetReason(ItemFlow_Skill, ItemFlow_Skill_Up);
		UINT32 costtype = SKILL_POINT == pRow->LevelupCostType ? GetSkillPointType() : pRow->LevelupCostType;
		if (!taketransition.TakeItem(costtype, needValue))
		{
			taketransition.RollBack();
			return KKSG::ERR_SHOP_LACKMONEY;
		}
		taketransition.NotifyClient();
	}

	bool isnewlearned = false;
	///> 已经学会了升级
	if (po != NULL)
	{
		po->skilllevel += 1;
		po->skillpoint += needValue;

		//LogInfo("Levelup skill %u to level %u", skillhash, po->skilllevel);

		if (m_pListener) 
		{
			m_pListener->OnChangedSkillLevel(skillhash, po->skilllevel);
		}

		///> 如果有ex技能，也需要升级
		UINT32 exskill = XSkillEffectMgr::Instance()->GetExSkill(skillhash, 0);
		if (0 != exskill)
		{
			skillInfo *ex = FindSkillInfo(skillhash);
			if (NULL != ex)
			{
				ex->skilllevel = po->skilllevel;
			}
		}
	}
	///> 没学过添加
	else
	{
		// LogInfo("Learn new skill %u", skillhash);

		if (pRow->LevelupCostType == SKILL_POINT)
		{
			AddSkill(skillhash, 1, needValue, false);
			isnewlearned = true;
		}
		///> 不需要技能点就能学的初始化技能
		else if (needValue == 0 && pRow->LevelupCostType == 0)
		{
			AddSkill(skillhash, 1, needValue, false);
			isnewlearned = true;
		}
	}

	m_modified.Set();

	RefreshSkillFlags();

	///> 如果学习了ex技能自动替换pre技能
	if (0 != prehash)
	{
		UINT32 count = 0;
		for (auto i = m_skillslot->begin(); i != m_skillslot->end(); ++i)
		{
			if (prehash == *i)
			{
				BindSkillSlot(count, skillhash);
				break;
			}
			++count;
		}
	}
	
	if (isnewlearned)
	{
		SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(skillhash, 0);
		if (NULL != data && !data->BindSkill.empty())
		{
			UINT32 hashid = XCommon::XHash(data->BindSkill.c_str());
			int ret = TryLevelup(hashid, true);
			SSInfo<<"role:"<<m_pRole->GetID()<<" learn skill:"<<skillhash<<" bind skill:"<<data->BindSkill.c_str()<<" ret:"<<ret<<END;
		}
	}
	return KKSG::ERR_SUCCESS;
}

void RoleSkillSet::ResetInitSkill()
{
	for (int i = 0; i < SKILL_SET_MAX; ++i)
	{
		m_skillsets[i].m_skills.clear();
	}
	m_SkillFlags.Reset(); 
}

void RoleSkillSet::ResetTranform()
{
	m_transid = 0;
	m_canleveltrans = false;
	m_transformskillpoint = 0;
	m_transformskills.clear();
}

void RoleSkillSet::OnTransform(UINT32 transid)
{
	if (m_transid != transid)
	{
		ResetTranform();

		Scene* scene = m_pRole->GetCurrScene();
		if (NULL == scene)
		{
			SSWarn<<"role:"<<m_pRole->GetID()<<" current scene is NULL, when trans"<<END;
			scene = m_pRole->GetLoadingScene();
		}
		if (NULL == scene)
		{
			SSWarn<<"role:"<<m_pRole->GetID()<<" scene is NULL, when trans"<<END;
			return;
		}
		UINT32 scenetype = scene->GetSceneType();
		UINT32 sceneid = scene->GetSceneTemplateID();
		for (auto i = GetGlobalConfig().CanLevelTransSkill.begin(); i != GetGlobalConfig().CanLevelTransSkill.end(); ++i)
		{
			if (i->seq[0] == 1)
			{
				if (scenetype == i->seq[1])
				{
					m_canleveltrans = true;
					break;
				}
			}
			else if (i->seq[0] == 2)
			{
				if (sceneid == i->seq[1])
				{
					m_canleveltrans = true;
					break;
				}
			}
		}

		m_transid = transid;

		const XEntityStatistics::RowData* data = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(transid);
		if(NULL == data)
		{
			SSWarn<<"roleid:"<<m_pRole->GetID()<<" transid:"<<transid<<" error"<<END;
			return;
		}
		const XEntityPresentation::RowData* pData = XEntityInfoLibrary::Instance()->PresentID2Data(data->PresentID);
		if (NULL != pData)
		{
			///> 炸弹场景
			if (!pData->A.empty())
			{
				skillInfo info;
				info.skillhash = XCommon::XHash(pData->A.c_str());
				info.skilllevel = 1;
				m_transformskills[info.skillhash] = info;
			}
			if (!pData->OtherSkills.empty())
			{
				vector<string>::const_iterator it = pData->OtherSkills.begin();
				while (it != pData->OtherSkills.end())
				{
					skillInfo info;
					info.skillhash = XCommon::XHash(it->c_str());
					info.skilllevel = 0;
					m_transformskills[info.skillhash] = info;
					++it;
				}
			}
		}
	}
}

KKSG::ErrorCode RoleSkillSet::LevelTransformSkill(int type, UINT32 point, UINT32 level, UINT32 skillhash)
{
	if (!m_canleveltrans)
	{
		SSWarn<<"roleid:"<<m_pRole->GetID()<<" not in can trans scene"<<END;
		return KKSG::ERR_FAILED;
	}
	PtcG2C_TransSkillNotfiy ntf;
	ntf.m_Data.set_isincreasing(true);
	if (1 == type)
	{
		for (auto i = m_transformskills.begin(); i != m_transformskills.end(); ++i)
		{
			UINT32 skillLvl = i->second.skilllevel;
			SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(i->first, skillLvl + 1, m_transid); 
			if (pRow == NULL)
			{
				continue;
			}
			++i->second.skilllevel;

			ntf.m_Data.add_skillhash(i->first);
			ntf.m_Data.add_skilllevel(i->second.skilllevel);

			OnTransLevel(skillhash);
		}
	}
	else if (2 == type)
	{
		if (m_transformskillpoint >= point)
		{
			return KKSG::ERR_SKILL_POINT;
		}
		auto i = m_transformskills.find(skillhash);
		if (i == m_transformskills.end())
		{
			return KKSG::ERR_BIND_SKILL_NOT_LEARN;
		}
		UINT32 skillLvl = i->second.skilllevel;
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(skillhash, skillLvl + 1, m_transid); 
		if (pRow == NULL)

		{
			return KKSG::ERR_SKILL_MAXLEVEL;
		}
		int NeedRoleLvl = 0;
		if (!pRow->UpReqRoleLevel.empty())
		{
			NeedRoleLvl = skillLvl >= (int)pRow->UpReqRoleLevel.size() ? pRow->UpReqRoleLevel.back() : pRow->UpReqRoleLevel[skillLvl];
		}
		if ((int)level < NeedRoleLvl)
		{
			return KKSG::ERR_SKILL_LEVELREQ;
		}

		++m_transformskillpoint;
		++i->second.skilllevel;
		OnTransLevel(skillhash);

		ntf.m_Data.add_skillhash(i->first);
		ntf.m_Data.add_skilllevel(i->second.skilllevel);
	}
	else
	{
		SSWarn<<"roleid:"<<m_pRole->GetID()<<" type:"<<type<<" error"<<END;
		return KKSG::ERR_FAILED;
	}
	if (ntf.m_Data.skillhash_size() > 0)
	{
		m_pRole->Send(ntf);
	}
	return KKSG::ERR_SUCCESS;
}

void RoleSkillSet::OnTransLevel(UINT32 skillhash)
{
	if (NULL != m_pRole->GetXRole() )
	{
		XSkillMgr* skillmgr = m_pRole->GetXRole()->SkillManager();
		if (NULL != skillmgr)
		{
			XSkillCore* core = skillmgr->GetSkill(skillhash);
			if (NULL != core)
			{
				core->InitCoreData();
			}
		}
	}
}

void RoleSkillSet::NotifyTransLevel()
{
	PtcG2C_TransSkillNotfiy ntf;
	ntf.m_Data.set_isincreasing(false);
	for (auto i = m_transformskills.begin(); i != m_transformskills.end(); ++i)
	{
		ntf.m_Data.add_skillhash(i->first);
		ntf.m_Data.add_skilllevel(i->second.skilllevel);
	}
	m_pRole->Send(ntf);
}
