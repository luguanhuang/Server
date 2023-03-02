#include "pch.h"
#include "pet.h"
#include "time.h"
#include "timermgr.h"
#include "petconfigmgr.h"
#include "utility/uidhelper.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "unit/role.h"
#include "pet/ptcg2c_petchangenotfiy.h"
#include "table/globalconfig.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "buff/XBuff.h"
#include "entity/XRole.h"
#include "component/XBuffComponent.h"
#include "table/PowerPointMgr.h"
#include "unit/roleoutlook.h"
#include "scene/scene.h"
#include "scene/sceneconfig.h"
#include "pet/ptcg2c_noticehungrydown.h"
#include "unit/calcattribute.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "mslink.h"
#include "foreach.h"
#include "utility/tlogger.h"
#include "idip/idiprecord.h"
#include "gamelogic/payv2Record.h"
#include "event/eventmgr.h"
#include "unit/rolemanager.h"
#include "unit/roleoutlook.h"

//#define PET_MAXSLOTS 10
//#define PET_OPENSLOTS 10 
//#define PET_PASSIVESKILL_MAX 6
//#define PET_MOOD_TIME 3600

#define PET_INVITE_MAX 5

////////////////  pet

Pet::Pet()
{	
	uid = 0;
	petid = 0;
	level = 0;
	exp = 0;
	sex = 0;
	power = 0;
	mood = 0;
	hungry = 0;
	max_level_ = 0;
	canpairride = false;
}

Pet::Pet(UINT32 petId, Role* pRole)
{
	uid = UIDHelper::CreateUID(UID_Pet);
	petid = petId;
	level = 1;
	max_level_ = 1;
	exp = 0;
	UINT32 initSex = PetConfigMgr::Instance()->GetMale(petId);
	if (initSex == 1 || initSex == 2)
	{
		sex = initSex;  // 固定性别
	}
	else
	{
		sex = XRandom::randInt(0, 2) + 1;  // 随机性别
	}
	power = 0;
	mood = PetConfigMgr::Instance()->GetInitMood(petid);
	hungry = PetConfigMgr::Instance()->GetInitHungry(petid);
	RecalcPower(pRole);

	canpairride = PetConfigMgr::Instance()->CanPairRide(petid);
}

Pet::~Pet()
{
}

void Pet::ToKKSG(KKSG::PetSingle& petsingle)
{
	petsingle.set_uid(uid);
	petsingle.set_petid(petid);
	petsingle.set_level(level);
	petsingle.set_exp(exp);
	petsingle.set_sex(sex);
	petsingle.set_power(power);
	petsingle.set_mood(mood);
	petsingle.set_hungry(hungry);
	petsingle.set_max_level(max_level_);
	petsingle.set_canpairride(canpairride);

	foreach (i in fixedSkills)
	{
		petsingle.add_fixedskills(*i);
	}

	foreach (j in randSkills)
	{
		petsingle.add_randskills(*j);
	}

	KKSG::SPetRecord* precord = petsingle.mutable_record();
	precord->set_touchstarttime(record.touchStartTime);
	precord->set_touchhourattr(record.touchHourAttr);
	precord->set_touchtodayattr(record.touchTodayAttr);
	precord->set_followstarttime(record.followStartTime);
	precord->set_followtodayattr(record.followTodayAttr);
	precord->set_hungrystarttime(record.hungryStartTime);
	precord->set_moodstarttime(record.moodStartTime);
	precord->set_max_level(record.max_level);
}

void Pet::ReadKKSG(const KKSG::PetSingle& petsingle, Role* pRole)
{
	uid = petsingle.uid();
	petid = petsingle.petid();
	level = petsingle.level();
	exp = petsingle.exp();
	sex = petsingle.sex();
	power = petsingle.power();
	mood = petsingle.mood();
	hungry = petsingle.hungry();
	max_level_ = petsingle.max_level();
	canpairride = petsingle.canpairride();

	fixedSkills.clear();
	for (INT32 i = 0; i < petsingle.fixedskills_size(); ++i)
	{
		fixedSkills.push_back(petsingle.fixedskills(i));
	}

	randSkills.clear();
	for (INT32 j = 0; j < petsingle.randskills_size(); ++j)
	{
		randSkills.push_back(petsingle.randskills(j));
	}

	if (petsingle.has_record())
	{
		record.touchStartTime = petsingle.record().touchstarttime();
		record.touchHourAttr = petsingle.record().touchhourattr();
		record.touchTodayAttr = petsingle.record().touchtodayattr();
		record.followStartTime = petsingle.record().followstarttime();
		record.followTodayAttr = petsingle.record().followtodayattr();
		record.hungryStartTime = petsingle.record().hungrystarttime();
		record.moodStartTime = petsingle.record().moodstarttime();
		record.max_level = petsingle.record().max_level();
	}

	RecalcPower(pRole);
}

void Pet::GetAllAttrs(std::vector<ItemChangeAttr>& attrsVec)
{
	std::vector<UINT32> skillIdVec;
	foreach (i in fixedSkills)
	{
		skillIdVec.push_back(*i);
	}
	foreach (j in randSkills)
	{
		skillIdVec.push_back(*j);
	}

	PetConfigMgr::Instance()->GetAllAttrs(petid, level, skillIdVec, attrsVec);
}

UINT32 Pet::Touch(bool& isChenge)
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		SSWarn << " pet info data is NULL" << END;
		return KKSG::ERR_UNKNOWN;
	}

	time_t nowTime = time(NULL);
	if (XCommon::IsDaySame(record.touchStartTime, nowTime))
	{
		if (record.touchTodayAttr >= pRowData->touchDayMax)
		{
			return KKSG::ERR_SUCCESS;
		}
		if (XCommon::IsHourSame(nowTime, record.touchStartTime))
		{
			if (record.touchHourAttr >= pRowData->touchHourMax)
			{
				return KKSG::ERR_SUCCESS;
			}
		}
		else
		{
			record.touchHourAttr = 0;
			record.touchStartTime = nowTime;
		}
	}
	else
	{
		record.touchHourAttr = 0;
		record.touchTodayAttr = 0;
		record.touchStartTime = nowTime;
	}

	UINT32 addNum = pRowData->touchAttr;
	addNum = (addNum + record.touchHourAttr) > pRowData->touchHourMax ? (pRowData->touchHourMax - record.touchHourAttr) : addNum;
	addNum = (addNum + record.touchTodayAttr) > pRowData->touchDayMax ? (pRowData->touchDayMax - record.touchTodayAttr) : addNum;
	if (addNum > 0)
	{
		_ChangeMood(addNum);
		isChenge = true;
		record.touchHourAttr += addNum;
		record.touchTodayAttr += addNum;
		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_SUCCESS;
}

void Pet::SetCanPairRide(bool istrue)
{
	canpairride = istrue;
}

void Pet::FollowOn()
{
	record.followStartTime = time(NULL);
}

void Pet::FollowDown()
{
	_UpdateFollow();
	record.followStartTime = 0;
}

bool Pet::Feed(UINT32 exp, UINT32 hungry, UINT32 mood, Role* pRole, UINT32& delPer, std::map<UINT32, UINT32>& skillMap, bool& ismoodAdd, bool& ishungryAdd)
{
	if (pRole == NULL)
	{
		return 0;
	}

	UINT32 Percent = _GetHungryPercent() >= 100 ? 100 : _GetHungryPercent(); 
	if (Percent > 0)
	{
		exp = exp * Percent / 100;
	}
	else
	{
		exp = 0;
	}
	delPer = 100 - Percent;

	if (hungry > 0)
	{
		_UpdateHungry();
		ishungryAdd = _ChangeHungry(hungry);
	}

	if (mood > 0)
	{
		_UpdateMood();
		ismoodAdd = _ChangeMood(mood);
	}

	return AddExp(exp, pRole, skillMap);
}

void Pet::Update()
{
	_UpdateMood();
	_UpdateHungry();
	_UpdateFollow();
}

UINT32 Pet::_GetHungryPercent()
{
	for (auto i =  GetGlobalConfig().HungryExpPercent.begin(); i !=  GetGlobalConfig().HungryExpPercent.end(); ++i)
	{
		if (hungry >= i->seq[0])
		{
			return i->seq[1]; 
		}
	}
	return 0;
}

bool Pet::AddFixedSkill(std::map<UINT32, UINT32>& skillMap)
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		return 0;
	}

	UINT32 checkSkill = _CheckFixedSkill();
	if (checkSkill > 0)
	{
		fixedSkills.push_back(checkSkill);
		skillMap[level] = checkSkill;
		return true;
	}

	UINT32 nextSkill = _GetNextFixSkill();
	if (nextSkill == 0)
	{
		return false;
	}

	UINT32 randNum = XRandom::randInt(0, 10000);
	if (randNum < pRowData->LevelupInsight * PetConfigMgr::Instance()->GetComprehend(mood))
	{
		fixedSkills.push_back(nextSkill);
		skillMap[level] = nextSkill;
		SSInfo << "pet add fix skill petid : " << uid << "fix skillid : " << nextSkill << END; 
		return true;
	}
	return false;

	/*
	UINT32 skillId = PetConfigMgr::Instance()->GetFixedSkillId(petid, level);

	if (skillId != 0)
	{
		fixedSkills.push_back(skillId);
		skillMap[level] = skillId;
		return true;
	}

	return false;
	*/
}

void Pet::_UpdateFollow()
{
	if (record.followStartTime == 0)
	{
		return ;
	}

	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		SSWarn << " pet info data is NULL" << END;
		return;
	}

	UINT32 addNum = 0;
	time_t nowTime = time(NULL);
	if (XCommon::IsDaySame(record.followStartTime, nowTime))
	{
		if (record.followTodayAttr >= pRowData->followDayMax)
		{
			return ;
		}

		addNum = pRowData->followAttr * ((nowTime - record.followStartTime) / 60); 
		addNum = (addNum + record.followTodayAttr) > pRowData->followDayMax ? (pRowData->followDayMax -record.followTodayAttr) : addNum; 
		record.followTodayAttr += addNum;
		record.followStartTime = nowTime;
		_ChangeMood(addNum);
	}
	else
	{
		// 前一天的数
		UINT32 dayBeginTime = XCommon::GetDayBeginTime(nowTime);
		if (record.followTodayAttr < pRowData->followDayMax)
		{
			addNum = pRowData->followAttr * ((dayBeginTime - record.followStartTime) / 60);  
			addNum = (addNum + record.followTodayAttr) > pRowData->followDayMax ? (pRowData->followDayMax -record.followTodayAttr) : addNum; 
			_ChangeMood(addNum);
		}

		/// 今天的数值
		addNum = pRowData->followAttr * ((nowTime - dayBeginTime) / 60); 
		addNum = addNum > pRowData->followDayMax ? pRowData->followDayMax : addNum; 
		record.followTodayAttr = addNum;
		record.followStartTime = nowTime;
		_ChangeMood(addNum);
	}
}

void Pet::_UpdateMood()
{
	UINT32 nowTime = time(NULL); 
	if (nowTime <= record.moodStartTime)
	{
		return ;
	}

	UINT32 passHours = (nowTime - record.moodStartTime) / (60 * 60);
	record.moodStartTime += passHours * 60 * 60;
	while (passHours > 0)
	{
		int addNum = PetConfigMgr::Instance()->GetRandomMood(mood);
		_ChangeMood(addNum);
		--passHours;
	}
}

void Pet::_UpdateHungry()
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		SSWarn << " pet info data is NULL" << END;
		return;
	}

	int passHors = (time(NULL) - record.hungryStartTime) / (60 * 60);
	//int passMins = ((time(NULL) - record.hungryStartTime) % (60 * 60)) / 60; 
	record.hungryStartTime += passHors * 60 * 60/* + passMins * 60*/;
	while (passHors > 0)
	{
		int addNum = PetConfigMgr::Instance()->GetHungryAttr(petid, hungry);
		_ChangeMood(addNum * 10);
		_ChangeHungry(pRowData->hungryAuto);
		--passHors;
	}

	/*
	if (passMins > 0)
	{
	int add = PetConfigMgr::Instance()->GetHungryAttr(petid, hungry);
	_ChangeMood(add * passMins);
	_ChangeHungry((pRowData->hungryAuto * passMins) / 60);
	}
	*/

}

bool Pet::AddExp(UINT32 addexp, Role* pRole, std::map<UINT32, UINT32>& skillMap)
{
	bool isLvlUp = false;
	exp += addexp;
	if (level >= (UINT32)pRole->GetLevel() * 2) // 不能超过玩家等级*2
	{
		return false;
	}

	while(PetConfigMgr::Instance()->CanLevelUp(petid, level, exp))
	{
		if (level >= (UINT32)pRole->GetLevel() * 2) // 不能超过玩家等级*2
		{
			break;
		}

		++level;
		// 
		if ( level > max_level_)
		{
			AddFixedSkill(skillMap);
			/*
			if (!AddFixedSkill(skillMap))
			{
				AddRandSkill(skillMap);
			}*/

			++max_level_;
		}

		isLvlUp = true;
	}

	if (isLvlUp)
	{
		RecalcPower(pRole);
		pRole->OnPetChange(uid);
		pRole->Get<PetSys>()->UpdateRankList(uid);
		return true;
	}
	return false;
}

void Pet::_NoticeLvlup(Role* pRole, UINT32 type, UINT32 delExp)
{
	if (pRole == NULL)
	{
		return;
	}

	Update();
	PtcG2C_PetChangeNotfiy msg;
	msg.m_Data.set_type((KKSG::PetOP)type);
	msg.m_Data.set_delexp(delExp);
	KKSG::PetSingle* petsingle = msg.m_Data.add_pet();
	ToKKSG(*petsingle);
	pRole->Send(msg);
}

void Pet::_NoticeSkillBook(Role* pRole, UINT32 addSkill, UINT32 delSkill)
{
	if (pRole == NULL)
	{
		return;
	}

	Update();
	PtcG2C_PetChangeNotfiy msg;

	msg.m_Data.set_type(KKSG::useskillbook);
	msg.m_Data.set_delskillid(delSkill);

	KKSG::petGetSkill* pGetSkill = msg.m_Data.add_getskills();
	pGetSkill->set_petlvl(level);
	pGetSkill->set_skillid(addSkill);

	KKSG::PetSingle* petsingle = msg.m_Data.add_pet();
	ToKKSG(*petsingle);

	pRole->Send(msg);
}

void Pet::RecalcPower(Role* pRole)
{/*
	CombatAttribute tmpCombat;
	std::vector<ItemChangeAttr> attrsVec;
	GetAllAttrs(attrsVec);
	foreach (i in attrsVec)
	{
		tmpCombat.AddAttr((CombatAttrDef)(i->AttrID), i->AttrValue);
	}
	CalcRoleAttribute petAttribute;
	petAttribute.CalcAttributeConvert(&tmpCombat, pRole->GetProfession()); ///> 一级属性转化为二级属性
	///> 只加二级属性
	for(auto i = PowerPointMgr::Instance()->m_oCoeff.begin(); i != PowerPointMgr::Instance()->m_oCoeff.end(); i++)
	{
		CombatAttrDef attrType = (CombatAttrDef)(i->first);
		if(attrType != BASIC_Strength && attrType != BASIC_Agility && attrType != BASIC_Intelligence && attrType != BASIC_Vitality) ///> 非一级属性
		{
			double val = tmpCombat.GetAttr((CombatAttrDef)(CA_TOTAL_START + attrType));
			combatAttr->AddAttr((CombatAttrDef)attrType, val);

		}
	}
	*/

	CombatAttribute com;
	std::vector<ItemChangeAttr> attrsVec;
	GetAllAttrs(attrsVec);
	ApplyEffectAux(attrsVec, &com);
	CalcRoleAttribute attr;
	if (pRole != NULL)
	{
		attr.CalcAttributeConvert(&com, pRole->GetProfession());
	}
	power = PowerPointMgr::Instance()->CalcPowerPoint(&com, pRole->GetProfession());
}

bool Pet::_ChangeHungry(int num)
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		SSWarn << " pet info data is NULL" << END;
		return false;
	}

	bool  isAdd = false;
	if (hungry > 0 && hungry < pRowData->maxHungry)
	{
		isAdd = true;
	}

	if (((int)hungry + num) < 0)
	{
		hungry = 0;
	} 
	else if (((int)hungry + num) > pRowData->maxHungry)
	{
		hungry = pRowData->maxHungry;
	}
	else
	{
		hungry = (UINT32)((int)hungry + num);
	}

	return isAdd;
}

bool Pet::_ChangeMood(int num)
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		SSWarn << " pet info data is NULL" << END;
		return false;
	}

	bool  isAdd = false;
	if (num > 0 && mood < pRowData->maxMood)
	{
		isAdd = true;
	}

	if (((int)mood + num) < 0)
	{
		mood = 0;
	} 
	else if (((int)mood + num) > pRowData->maxMood)
	{
		mood = pRowData->maxMood;
	}
	else
	{
		mood = (UINT32)((int)mood + num);
	}

	return isAdd;
}

bool Pet::_HasFixSkill(UINT32 skillId)
{
	foreach (i in fixedSkills)
	{
		if (*i == skillId)
		{
			return true;
		}
	}

	return false;
}

bool Pet::_CanStudySkill(UINT32 skillId)
{
	foreach (i in fixedSkills)
	{
		if (skillId == *i)
		{
			return false;
		}
	}

	foreach (j in randSkills)
	{
		if (skillId == *j)
		{
			return false;
		}
	}

	std::set<UINT32> fixSkillSet;
	PetConfigMgr::Instance()->GetFixedSkills(petid, fixSkillSet);
	foreach (k in fixSkillSet)
	{
		if (skillId == *k)
		{
			return false;
		}
	}

	return true;
}

UINT32 Pet::_CheckFixedSkill()
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		return 0;
	}
	
	for (UINT32 cnt = 0; cnt < pRowData->LvRequire.size(); ++cnt)
	{
		if (level == pRowData->LvRequire[cnt])
		{
			if (cnt == 0 && !_HasFixSkill(pRowData->skill1[0].seq[0]))
			{
				return pRowData->skill1[0].seq[0]; 
			}
			if (cnt == 1 && !_HasFixSkill(pRowData->skill2[0].seq[0]))
			{
				return pRowData->skill2[0].seq[0]; 
			}
			if (cnt == 2 && !_HasFixSkill(pRowData->skill3[0].seq[0]))
			{
				return pRowData->skill3[0].seq[0]; 
			}
		}
	}

	return 0;
}

UINT32 Pet::_GetNextFixSkill()
{
	PetInfoTable::RowData* pRowData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		return 0;
	}

	if (!_HasFixSkill(pRowData->skill1[0].seq[0]))
	{
		return pRowData->skill1[0].seq[0];
	}

	if (!_HasFixSkill(pRowData->skill2[0].seq[0]))
	{
		return pRowData->skill2[0].seq[0];
	}

	if (!_HasFixSkill(pRowData->skill3[0].seq[0]))
	{
		return pRowData->skill3[0].seq[0];
	}

	return 0;
}

void Pet::AddRandSkill(std::map<UINT32, UINT32>& skillMap)
{
	if (randSkills.size() >= PetConfigMgr::Instance()->GetRandSkillMax(petid))
	{
		return ;
	}

	bool isMast = false;
	PetInfoTable::RowData* pData = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pData != NULL) // 保底机制
	{
		foreach (k in pData->MinimumInsight)
		{
			if (level >= k->seq[0] && randSkills.size() < k->seq[1])
			{
				isMast = true;
				break;
			}
		}
	}

	std::map<UINT32, UINT32> skillsMap;
	PetConfigMgr::Instance()->GetQualitySkill(petid, mood, skillsMap, isMast);

	UINT32 wight = 0;
	for (auto i = skillsMap.begin(); i != skillsMap.end();)
	{
		if (!_CanStudySkill(i->first))
		{
			skillsMap.erase(i++);
		}
		else
		{
			wight += i->second;
			++i;
		}
	}

	if (wight == 0)
	{
		return ;
	}

	UINT32 randNum = XRandom::randInt(0, wight);

	wight = 0;
	foreach (j in skillsMap)
	{
		wight += j->second;
		if (randNum < wight)
		{
			randSkills.push_back(j->first);
			skillMap[level] = j->first;
			return ;
		}
	}
}

UINT32 Pet::GetTotalExp()
{
	UINT32 total_experience = exp;
	total_experience += PetConfigMgr::Instance()->GetExpByLevel(petid, level);

	return total_experience;
}

void Pet::SetPetExp(const UINT32 new_exp,Role* role)
{
	exp = 0;
	level = 1;
	std::map<UINT32, UINT32> skillMap;
	AddExp(new_exp,role,skillMap);
	RecalcPower(role);
	role->OnPetChange(uid);
	role->Get<PetSys>()->UpdateRankList(uid);
}

bool Pet::CanAddSkill(UINT32 skillId)
{
	foreach (i in randSkills)
	{
		if (*i == skillId)
		{
			return false;
		}
	}

	std::set<UINT32> skillSet;
	PetConfigMgr::Instance()->GetFixedSkills(petid, skillSet);
	if (skillSet.find(skillId) != skillSet.end())
	{
		return false;
	}

	return true;
}

void Pet::_CheckDelSkill(UINT32 bookId, UINT32& delSkillId)
{
	PetSkillBook::RowData* pRowData = PetConfigMgr::Instance()->GetPetBookData(bookId);
	if (pRowData == NULL)
	{
		return ;
	}

	PetInfoTable::RowData* pRow = PetConfigMgr::Instance()->GetPetInfoData(petid);
	if (pRowData == NULL)
	{
		return ;
	}

	UINT32 pro = 0;
	foreach (i in pRowData->pro)
	{
		if (i->seq[0] == randSkills.size())
		{
			pro = i->seq[1];
			break;
		}
	}

	if (randSkills.size() > pRow->randSkillMax)
	{
		_DelSkill();
	}

	if (pro > 0)
	{
		UINT32 randNum = XRandom::randInt(1, 101);
		if (randNum >= pro || randSkills.size() >= pRow->randSkillMax)
		{
			_TryDelSkill(delSkillId);
		}
	}

	if (randSkills.size() >= pRow->randSkillMax)
	{
		_TryDelSkill(delSkillId);
	}
	/*
	if (randSkills.size() >= pRow->randSkillMax)
	{
		_DelSkill(delSkillId);
	}
	*/

}

void Pet::_TryDelSkill(UINT32& delSkillId)
{
	std::vector<UINT32> skillVec;
	foreach (j in randSkills)
	{
		if (*j != fixedSkill)
		{
			skillVec.push_back(*j);
		}
	}

	if (!skillVec.empty())
	{
		UINT32 randNum = XRandom::randInt(0, skillVec.size());
		auto iter = skillVec.begin();
		for (UINT32 cnt = 0; cnt < randNum; ++cnt)
		{
			++iter;
		}
		delSkillId = *iter;
	}
}

void Pet::_DelSkill()
{
	std::vector<UINT32> skillVec;
	foreach (j in randSkills)
	{
		if (*j != fixedSkill)
		{
			skillVec.push_back(*j);
		}
	}

	if (!skillVec.empty())
	{
		UINT32 delSkill = 0;
		UINT32 randNum = XRandom::randInt(0, skillVec.size());
		auto iter = skillVec.begin();
		for (UINT32 cnt = 0; cnt < randNum; ++cnt)
		{
			++iter;
		}
		delSkill = *iter;

		for (auto iter = randSkills.begin(); iter != randSkills.end(); ++iter)
		{
			if (*iter == delSkill)
			{
				randSkills.erase(iter);
				return ;
			}
		}
	}
}

void Pet::AddSkill(Role* pRole, UINT32 bookId)
{
	if (pRole == NULL)
	{
		return ;
	}

	PetSkillBook::RowData* pRowData = PetConfigMgr::Instance()->GetPetBookData(bookId);
	if (pRowData == NULL)
	{
		return ;
	}

	if (!CanAddSkill(pRowData->skillid))
	{
		return ;
	}

	UINT32 delSkillId = 0;
	_CheckDelSkill(bookId, delSkillId);
	if (delSkillId != 0)
	{
		foreach (i in randSkills)
		{
			if (*i == delSkillId)
			{
				*i = pRowData->skillid; 
				break;
			}
		}
	}
	else
	{
		randSkills.push_back(pRowData->skillid);
		SSInfo << "pet roleid : " << pRole->GetID() << " petid : " << uid << " skillid : " << pRowData->skillid << END; 
	}

	RecalcPower(pRole);
	pRole->OnPetChange(uid);
	pRole->Get<PetSys>()->UpdateRankList(uid);
	pRole->RecalcuateAttributeAndNotify(RecalAttr_Pet);

	if (pRole != NULL)
	{
		_NoticeSkillBook(pRole, pRowData->skillid, delSkillId);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
PetSys::PetSys() : m_petSeats(0), m_followPetId(0), m_fightPetId(0)
{
	m_tempPetId = 0;
	m_lastFollowId = 0;
	m_ispassive = false;
	m_followroleid = 0;   ///> 跟随的角色
	m_timehandler = INVALID_HTIMER;
}

PetSys::PetSys(Role* pRole) : m_role(pRole), m_petSeats(0), m_followPetId(0), m_fightPetId(0)
{
	m_tempPetId = 0;
	m_ispassive = false;
	m_followroleid = 0;   ///> 跟随的角色
	m_timehandler = INVALID_HTIMER;
}

PetSys::~PetSys()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

bool PetSys::Load(const KKSG::RoleAllInfo* poRoleAllInfo)
{
	if (poRoleAllInfo->has_petsys())
	{
		ReadKKSG(&poRoleAllInfo->petsys());
	}
	return true;
}

void PetSys::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	ToKKSG(poRoleAllInfo->mutable_petsys());

	roChanged.insert(poRoleAllInfo->mutable_petsys());
}

void PetSys::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		CheckOpen();
	}
}

void PetSys::ToKKSG(KKSG::PetSysData* pData)
{
	pData->Clear();

	pData->set_fightid(m_fightPetId);
	pData->set_followid(m_followPetId);
	pData->set_lastfollowid(m_lastFollowId);
	pData->set_petseats(m_petSeats);

	foreach (i in m_petsMap)
	{
		if (i->first != m_tempPetId)
		{
			KKSG::PetSingle* pPet = pData->add_pets();
			i->second.ToKKSG(*pPet);
		}
	}
}

void PetSys::ReadKKSG(const KKSG::PetSysData* pData)
{
	m_fightPetId = pData->fightid();
	m_followPetId = pData->followid();
	m_lastFollowId = pData->lastfollowid();
	m_petSeats = pData->petseats();

	for (int i = 0; i < pData->pets_size(); ++i)
	{
		Pet& pet = m_petsMap[pData->pets(i).uid()];
		pet.ReadKKSG(pData->pets(i), m_role);
		if (pet.power != pData->pets(i).power())
		{
			UpdateRankList(pData->pets(i).uid());
		}
	}
}

UINT32 PetSys::OPSetFight(UINT64 uid)
{
	if (uid == m_fightPetId)
	{
		return KKSG::ERR_SUCCESS;
	}

	auto iter = m_petsMap.find(uid);
	if (iter != m_petsMap.end())
	{
		m_fightPetId = uid;
		m_role->RecalcuateAttributeAndNotify(RecalAttr_Pet);
		m_role->OnPetChange(uid);

		TPetFlow oLog(m_role);
		oLog.m_iOperType = TX_Pet_Fight;
		oLog.SetData(&iter->second);
		oLog.Do();
	}
	else
	{
		m_fightPetId = 0;
		m_role->RecalcuateAttributeAndNotify(RecalAttr_Pet);
		m_role->OnPetChange(uid);
	}


	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPSetPairRide(UINT64 uid, bool istrue)
{
	Pet* pet = GetPet(uid);
	if (NULL == pet)
	{
		return KKSG::ERR_FAILED;
	}
	// config check
	if (!PetConfigMgr::Instance()->CanPairRide(pet->petid))
	{
		return KKSG::ERR_FAILED;			 
	}
	pet->SetCanPairRide(istrue);
	return KKSG::ERR_SUCCESS;
}

Pet* PetSys::GetPet(UINT64 uid)
{
	auto i = m_petsMap.find(uid);
	if (i != m_petsMap.end())
	{
		return &(i->second);
	}
	return NULL;
}

UINT32 PetSys::GetFollowPetId()
{
	Pet* pet = GetFollow();
	if (pet)
	{
		return pet->petid;
	}
	return 0;
}

UINT32 PetSys::GetFightPetId()
{
	Pet* pet = GetFight();
	if (pet)
	{
		return pet->petid;
	}
	return 0;
}

UINT64 PetSys::GetLastFollowId()
{
	auto iter = m_petsMap.find(m_lastFollowId);
	if (iter!=m_petsMap.end())
	{
		return m_lastFollowId;
	}else
	{
		if (!m_petsMap.empty())
		{
			m_lastFollowId = m_petsMap.begin()->first;
		}else
		{
			m_lastFollowId = 0;
		}
	}
	return m_lastFollowId;
}

void PetSys::AddTempPet(UINT32 petId)
{
	if (!m_petsMap.empty())
	{
		return ;
	}

	Pet newPet(petId, m_role);
	m_petsMap[newPet.uid] = newPet;	
	m_tempPetId = newPet.uid;
	//m_followPetId = newPet.uid;
	_AddTempBuff();
	_SynTempPet();
}

void PetSys::DelTempPet()
{
	if (m_tempPetId == 0)
	{
		return;
	}

	_ClearTempBuff();
	m_petsMap.erase(m_tempPetId);
	m_tempPetId = 0;
	_SynTempPet();
}

KKSG::ErrorCode PetSys::AddPet(UINT32 petId)
{
	//零收益检查
	if (m_role->Get<CIdipRecord>()->CheckZeroProFit())
	{
		SSWarn << "roleid:"<<m_role->GetID() <<" is zero profit now, can't give pet="<<petId <<END;
		return KKSG::ERR_FAILED;
	}

	KKSG::ErrorCode errorCode = CanAddPet(petId);
	if (errorCode != KKSG::ERR_SUCCESS)
	{
		return errorCode;
	}

	Pet newPet(petId, m_role);
	m_petsMap[newPet.uid] = newPet;	
	_NotifyPetChange(newPet, KKSG::PetBorn);
	m_role->OnPetChange(newPet.uid);
	m_role->Get<PetSys>()->UpdateRankList(newPet.uid);

	TPetFlow oLog(m_role);
	oLog.m_iOperType = TX_Pet_Get;
	oLog.m_iChangeValue = 1;
	oLog.SetData(&newPet);
	oLog.Do();

	PetInfoTable::RowData* pData = PetConfigMgr::Instance()->GetPetInfoData(petId);
	if (pData)
	{
		EventMgr::Instance()->AddGuildBonusEvent(m_role->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_PET, pData->quality);
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPReleasePet(UINT64 uid)
{
	Pet* pPet = GetPet(uid);
	if (pPet == NULL)
	{
		return KKSG::ERR_PET_NOT_EXIST;
	}

	if (m_fightPetId == uid || m_followPetId == uid)
	{
		return KKSG::ERR_PE_CAN_NOT_RELEASE;
	}

	if (m_lastFollowId == uid)
	{
		m_lastFollowId = 0;
	}
	TPetFlow oLog(m_role);
	oLog.m_iOperType = TX_Pet_Release;
	oLog.m_iChangeValue = 1;
	oLog.SetData(pPet);
	oLog.Do();

	m_role->Get<PetSys>()->UpdateRankList(uid, true);
	m_petsMap.erase(uid);
	m_role->OnPetChange(uid);
	SSInfo << "OPReleasePet petid : " << uid << END;
	return KKSG::ERR_SUCCESS;
}

std::string PetSys::GetStr()
{
	ostringstream ss;
	//CombatAttribute tmpCombat;
	Pet* pPet = GetFight();
	if (pPet == NULL)
	{
		return ss.str();
	}
	std::vector<ItemChangeAttr> attrsVec;
	pPet->GetAllAttrs(attrsVec);
	//GetAllAttrs(attrsVec);
	foreach (i in attrsVec)
	{
		ss << i->AttrID << " : " << i->AttrValue << " ; ";  
		//tmpCombat.AddAttr((CombatAttrDef)(i->AttrID), i->AttrValue);
	}
	return ss.str();
}

void PetSys::ApplyEffect(CombatAttribute *roleattrs)
{
	//CombatAttribute tmpCombat;
	Pet* pPet = GetFight();
	if (pPet == NULL)
	{
		return ;
	}
	std::vector<ItemChangeAttr> attrsVec;
	pPet->GetAllAttrs(attrsVec);
	//GetAllAttrs(attrsVec);
	foreach (i in attrsVec)
	{
		roleattrs->AddAttr((CombatAttrDef)(i->AttrID), i->AttrValue);
	}
	/*CalcRoleAttribute petAttribute;
	petAttribute.CalcAttributeConvert(&tmpCombat, m_role->GetProfession()); ///> 一级属性转化为二级属性
	///> 只加二级属性
	for(auto i = PowerPointMgr::Instance()->m_oCoeff.begin(); i != PowerPointMgr::Instance()->m_oCoeff.end(); i++)
	{
		CombatAttrDef attrType = (CombatAttrDef)(i->first);
		if(attrType != BASIC_Strength && attrType != BASIC_Agility && attrType != BASIC_Intelligence && attrType != BASIC_Vitality) ///> 非一级属性
		{
			double val = tmpCombat.GetAttr((CombatAttrDef)(CA_TOTAL_START + attrType));
			roleattrs->AddAttr((CombatAttrDef)attrType, val);
		}
	}


	Pet* pPet = GetFight();
	if (pPet != NULL)
	{
		std::vector<ItemChangeAttr> attrsVec;
		pPet->GetAllAttrs(attrsVec);
		ApplyEffectAux(attrsVec, roleattrs);
	}
	*/
}

void PetSys::_NotifyPetChange(Pet& pet, UINT32 type)
{
	PtcG2C_PetChangeNotfiy msg;
	msg.m_Data.set_type(KKSG::PetOP(type));	
	switch (type)
	{	
	case KKSG::PetBorn:
	case KKSG::PetTouch:
	case KKSG::PetUpdate:
		{
			pet.Update();
			KKSG::PetSingle* petsingle = msg.m_Data.add_pet();
			pet.ToKKSG(*petsingle);
			break;
		}
	default:
		break;
	}
	m_role->Send(msg);
}


void PetSys::_NotifyFeedPet(Pet& pet, UINT32 delExp, std::map<UINT32, UINT32>& getSkillMap)
{ 
	PtcG2C_PetChangeNotfiy msg;
	msg.m_Data.set_type(KKSG::PetFeed);
	msg.m_Data.set_delexp(delExp);
	pet.Update();
	KKSG::PetSingle* petsingle = msg.m_Data.add_pet();
	pet.ToKKSG(*petsingle);
	foreach (i in getSkillMap)
	{
		KKSG::petGetSkill* pSkill = msg.m_Data.add_getskills();
		pSkill->set_petlvl(i->first);
		pSkill->set_skillid(i->second);
	}

	m_role->Send(msg);
}

UINT64 PetSys::_GetLastFollowId()
{
	if (m_lastFollowId == 0 && !m_petsMap.empty())
	{
		m_lastFollowId = m_petsMap.begin()->first;
	}
	return m_lastFollowId;
}

void PetSys::_SynPetChange()
{	
	UINT32 petid = GetFollowConfId();
	if(petid != 0)
	{
		m_role->SetState(KKSG::OutLook_RidePet, petid);
	}
	else 
	{
		m_role->SetState(KKSG::OutLook_Normal, 0, true);
	}
	RoleOutLook outlook(m_role);
	outlook.Fill(SUMMARY_STATE, NULL);
	outlook.BroadCast();
}

void PetSys::_SynTempPet()
{
	if(m_tempPetId != 0)
	{
		m_role->SetState(KKSG::OutLook_RidePet, GetGlobalConfig().HorseID);
	}
	else 
	{
		m_role->SetState(KKSG::OutLook_Normal, 0, true);
	}
	RoleOutLook outlook(m_role);
	outlook.Fill(SUMMARY_STATE, NULL);
	outlook.BroadCast();
}

KKSG::ErrorCode PetSys::CanAddPet(UINT32 petId) 
{
	if (!IsOpen())
	{
		return KKSG::ERR_PETSYS_NOT_OPEN;
	}

	if (m_petSeats <= m_petsMap.size())
	{
		return KKSG::ERR_PET_SEAT_NOT_ENOUGH; 
	}

	if (!PetConfigMgr::Instance()->ExistPetId(petId))
	{
		SSWarn << "petId "<< petId <<" is not exist" << END;
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPExpandSeat()
{
	BagTakeItemTransition trans(m_role);
	trans.SetReason(ItemFlow_Pet, ItemFlow_Expand_Seat);
	if(!trans.TakeItem(DRAGON_COIN, _GetPetSeatCost(m_petSeats + 1)))
	{
		trans.RollBack();
		SSWarn << "OPExpandSeat dragoncoin not enough" << END;
		return KKSG::ERR_ITEM_NEED_DRAGONCOIN;
	}
	trans.NotifyClient();

	m_petSeats++;
	SSInfo << "OPExpandSeat and seat size is : " << m_petSeats << END; 

	TPetFlow oLog(m_role);
	oLog.m_iChangeValue = 1;
	oLog.m_iOperType = TX_Pet_ExpandSeat;
	oLog.Do();

	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPFeed(UINT64 uid, UINT32 itemid, UINT32 itemcount, bool& ismoodAdd, bool& ishungryAdd)
{
	if (itemcount != 1) // 只能一个一个的吃
	{
		return KKSG::ERR_UNKNOWN;
	}

	Pet* pPet = GetPet(uid);
	if (pPet == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (pPet->IsFull())
	{
		return KKSG::ERR_PET_IS_FULL;
	}

	UINT32 exp = 0;
	UINT32 hungry = 0;
	int mood = 0;
	if (!ItemConfig::Instance()->GetPetItem(itemid, itemcount, exp, hungry, mood))
	{
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition take(m_role);
	take.SetReason(ItemFlow_Pet, ItemFlow_Pet_Feed);
	if (!take.TakeItem(itemid, itemcount))
	{
		take.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	take.NotifyClient();

	UINT32 delPre = 0;
	std::map<UINT32, UINT32> skillMap;
	if (pPet->Feed(exp, hungry, mood, m_role, delPre, skillMap, ismoodAdd, ishungryAdd))
	{
		m_role->RecalcuateAttributeAndNotify(RecalAttr_Pet);
	}
	_NotifyFeedPet(*pPet, delPre, skillMap);
	m_role->OnPetChange(uid);

	TPetFlow oLog(m_role);
	oLog.m_iOperType = TX_Pet_Feed;
	oLog.m_iChangeValue = exp;
	oLog.SetData(pPet);
	oLog.Do();
	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPFollow(UINT64 uid, bool isSync)
{
	if (_CanFollowPet())
	{	
		Pet* pPetNow = NULL;
		Pet* pPetOld = NULL;
		if (uid == 0) // 快速骑乘
		{
			pPetNow = GetPet(_GetLastFollowId());
			pPetOld = GetFollow();

			if (!m_role->StateCanChangeTo(KKSG::OutLook_RidePet))
			{
				return KKSG::ERR_STATE_CANTCHANGE;
			}
		}
		else if (uid == m_followPetId) // 下马
		{
			pPetOld = GetFollow();
		}
		else // 上马
		{
			pPetOld = GetFollow();
			pPetNow = GetPet(uid);

			if(!m_role->StateCanChangeTo(KKSG::OutLook_RidePet))
			{
				return KKSG::ERR_STATE_CANTCHANGE;
			}
		}

		if (pPetOld != NULL)
		{
			///> 重置双人坐骑
			ResetPairRide(false);

			TPetFlow oLog(m_role);
			oLog.m_iOperType = TX_Pet_Down;
			oLog.SetData(pPetOld);
			oLog.Do();

			_ClearBuff();
			pPetOld->FollowDown();
			m_followPetId = 0;
		}

		if (pPetNow != NULL)
		{
			m_followPetId = pPetNow->uid;
			m_lastFollowId = pPetNow->uid;
			pPetNow->FollowOn();
			_AddBuff();

			TPetFlow Log(m_role);
			Log.m_iOperType = TX_Pet_Fellow;
			Log.SetData(pPetNow);
			Log.Do();
		}
		if(isSync)
		{
			_SynPetChange(); // 同步场景其他玩家
		}
	}
	else
	{
		return KKSG::ERR_SCENE_NOT_PET;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::OPTouch(UINT64 uid, bool& ischange)
{
	Pet* pPet = GetPet(uid);
	if (pPet == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 result = pPet->Touch(ischange);
	if (result == ERR_SUCCESS)
	{
		_NotifyPetChange(*pPet, KKSG::PetTouch);
		m_role->OnPetChange(uid);

		TPetFlow oLog(m_role);
		oLog.m_iOperType = TX_Pet_Touch;
		oLog.SetData(pPet);
		oLog.Do();
	}
	return result;
}

// 宠物经验转移
UINT32 PetSys::OPPetExpTransfer(const UINT64 pet_id,const UINT64 dest_pet_id)
{
	Pet* left_pet	= GetPet(pet_id);
	Pet* right_pet	= GetPet(dest_pet_id);
	if (NULL == left_pet || NULL == right_pet)
	{
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 left_exp		= left_pet->GetTotalExp();
	UINT32 right_exp	= right_pet->GetTotalExp();
	if (left_exp == right_exp)
	{
		// 经验一样多，没法转  
		LogDebug("exp equal %d  ",left_exp);
		return KKSG::ERR_PET_EXP_EQUAL; 
	}

	LogDebug("left_pet exp= %d,level=%d,randskill size = %d,fixedSkills =%d ",left_pet->GetTotalExp(),left_pet->level,left_pet->randSkills.size(),left_pet->fixedSkills.size());
	LogDebug("right_pet exp= %d,level=%d,randskill size = %d,fixedSkills =%d ",right_pet->GetTotalExp(),right_pet->level,right_pet->randSkills.size(),right_pet->fixedSkills.size());

	// 扣除物品
	auto it			= GetGlobalConfig().PetExpTransferCost.begin();
	auto it_end		= GetGlobalConfig().PetExpTransferCost.end();
	BagTakeItemTransition transition(m_role);
	transition.SetReason(ItemFlow_Pet, ItemFlow_Pet_Feed);
	for ( ; it != it_end; ++it)
	{
		if (!transition.TakeItem(it->seq[0],it->seq[1]))
		{
			transition.RollBack();	
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		SSInfo<<"itemid:"<<it->seq[0]<<" count:"<<it->seq[1]<<END;
	}
	transition.NotifyClient();

	Pet* source_pet		= NULL;
	Pet* dest_pet		= NULL;
	UINT32 change_exp	= 0;
	UINT32 remain_exp	= 0;	
	if (left_exp < right_exp)
	{
		source_pet	= right_pet;
		dest_pet	= left_pet;
		change_exp	= right_exp - left_exp;
		remain_exp	= left_exp;
	}
	else
	{
		dest_pet	= right_pet;
		source_pet	= left_pet;
		change_exp	= left_exp - right_exp;
		remain_exp	= right_exp;
	}

	// 经验变化
	std::map<UINT32, UINT32> skillMap;
	dest_pet->AddExp(change_exp,m_role,skillMap);	
	source_pet->SetPetExp(remain_exp,m_role);	

	// 饱腹度交换
	UINT32 hunTemp = source_pet->GetHungry();
	source_pet->SetHungry(dest_pet->GetHungry());
	dest_pet->SetHungry(hunTemp);

	// 重算战力
	source_pet->RecalcPower(m_role);
	dest_pet->RecalcPower(m_role);
	m_role->RecalcuateAttributeAndNotify(RecalAttr_Pet);

	NotifyTransferPetChange(*source_pet,*dest_pet,skillMap);

	LogDebug("source_pet remain_exp= %d,level= %d ,rand skill size:%d,fixedSkills:%d",remain_exp,source_pet->level,source_pet->randSkills.size(),source_pet->fixedSkills.size());
	LogDebug("dest_pet change_exp= %d,level = %d ,rand skill size :%d,fixedSkillss:%d",change_exp,dest_pet->level,dest_pet->randSkills.size(),dest_pet->fixedSkills.size());	

	return KKSG::ERR_SUCCESS;
}

UINT32 PetSys::_GetInitPetSeat()
{
	UINT32 cnt = 0;
	foreach (i in  GetGlobalConfig().PetSeatBuy)
	{
		if ((*i) == 0)
		{
			cnt++;
		}
		else if ((*i) > 0)
		{
			break;
		}
	}
	return cnt;
}

UINT32 PetSys::_GetPetSeatCost(UINT32 seatNum)
{
	if ( GetGlobalConfig().PetSeatBuy.size() <= 0)
	{
		return 0;
	}

	return seatNum >  GetGlobalConfig().PetSeatBuy.size() ?  GetGlobalConfig().PetSeatBuy.back() :  GetGlobalConfig().PetSeatBuy[seatNum - 1]; 
}

void PetSys::CheckOpen()
{
	if (m_petSeats == 0)
	{
		_Init();
	}
}

void PetSys::_Init()
{
	m_petSeats = _GetInitPetSeat();
}

bool PetSys::IsOpen()
{
	if (m_role->GetLevel() >= OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(15/*系统id*/))
	{
		return true;
	}

	return false;
}

/*
void PetSys::NotifyClient()
{
foreach (i in m_petsMap)
{
i->second.Update();
}

PtcG2C_PetNotify msg;
msg.m_Data.set_roleid(m_role->GetID());
ToKKSG(msg.m_Data.mutable_petsinfo());
m_role->Send(msg);
}
*/

/*
void PetSys::_UpdateBeforeSave()
{
foreach (i in m_petsMap)
{
i->second.Update();
}
}

void PetSys::_DoAfterLoad()
{
UINT32 nowTime = time(NULL);
foreach (i in m_petsMap)
{
i->second.record.hungryStartTime = nowTime;
if (i->second.uid == m_followPetId)
{
i->second.record.followStartTime = nowTime;
}
else
{
i->second.record.followStartTime = 0;
}
}

}
*/

void PetSys::_AddBuff()
{
	Pet* pPet = GetFollow();
	if (pPet == NULL)
	{
		return;
	}

	UINT32 buffId = PetConfigMgr::Instance()->GetBuffId(pPet->petid);
	//XBuff *pBuff = m_role->GetXRole()->BuffComponent()->GetBuffByID(buffId);
	if (m_role->GetXObject())
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = buffId;
		args.singleBuff.BuffLevel = 1;
		args.Caster = m_role->GetID();
		m_role->GetXObject()->DispatchAction(&args);
	}
}

void PetSys::_AddTempBuff()
{
	Pet* pPet = GetPet(m_tempPetId);
	if (pPet == NULL)
	{
		return;
	}

	UINT32 buffId = PetConfigMgr::Instance()->GetBuffId(pPet->petid);
	//XBuff *pBuff = m_role->GetXRole()->BuffComponent()->GetBuffByID(buffId);
	if (m_role->GetXObject())
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = buffId;
		args.singleBuff.BuffLevel = 1;
		args.Caster = m_role->GetID();
		m_role->GetXObject()->DispatchAction(&args);
	}
}


void PetSys::_ClearBuff()
{
	Pet* pPet = GetFollow();
	if (pPet == NULL)
	{
		return;
	}

	UINT32 buffId = PetConfigMgr::Instance()->GetBuffId(pPet->petid);
	XBuff *pBuff = m_role->GetXRole()->BuffComponent()->GetBuffByID(buffId);

	if (pBuff != NULL)
	{
		XRemoveBuffActionArgs args;
		args.nBuffId = buffId;
		if (m_role->GetXObject())
		{
			m_role->GetXObject()->DispatchAction(&args);
		}
	}
}

void PetSys::_ClearTempBuff()
{
	Pet* pPet = GetPet(m_tempPetId);
	if (pPet == NULL)
	{
		return;
	}

	UINT32 buffId = PetConfigMgr::Instance()->GetBuffId(pPet->petid);
	XBuff *pBuff = m_role->GetXRole()->BuffComponent()->GetBuffByID(buffId);

	if (pBuff != NULL)
	{
		XRemoveBuffActionArgs args;
		args.nBuffId = buffId;
		if (m_role->GetXObject())
		{
			m_role->GetXObject()->DispatchAction(&args);
		}
	}

}

bool PetSys::_CanFollowPet()
{
	UINT32 sceneId = 0;
	Scene* pScene = m_role->GetLoadingScene();
	if (pScene == NULL)
	{
		pScene = m_role->GetCurrScene();
		if (pScene == NULL)
		{
			return false;
		}
	}
	return SceneConfig::Instance()->CanFollowPet(pScene->GetSceneTemplateID());
}

UINT32 PetSys::OnEnterScene()
{
	if (_CanFollowPet())
	{
		if (m_followPetId > 0)
		{
			Pet* pPet = GetPet(m_followPetId);
			if (pPet == NULL)
			{
				m_followPetId = 0;
				return 0;
			}

			_AddBuff();
			if (pPet != NULL)
			{
				return pPet->petid;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (m_followPetId > 0)
		{
			Pet* pPet = GetPet(m_followPetId);
			if (pPet == NULL)
			{
				return 0;
			}

			_ClearBuff();
		}
		return 0;
	}
}

UINT32 PetSys::GetFollowConfId()
{
	if (!_CanFollowPet())
	{
		return 0;
	}
	else
	{
		Pet* pPet = GetPet(m_followPetId);
		return NULL == pPet ? 0 : pPet->petid;
	}
}

void PetSys::DoOnlineChek()
{
	Pet* pPet = GetPet(m_fightPetId);
	if (pPet == NULL)
	{
		return ;
	}

	pPet->Update();
	if (pPet->GetHungry() <= 50)
	{
		return ;
	}
	else
	{
		m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 600000, -1, __FILE__, __LINE__);
	}
}

void PetSys::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Pet* pPet = GetPet(m_fightPetId);
	if (pPet == NULL)
	{
		CTimerMgr::Instance()->KillTimer(m_timehandler);
		return ;
	}

	pPet->Update();
	if (pPet->GetHungry() < 50)
	{
		PtcG2C_NoticeHungryDown msg;
		msg.m_Data.set_petid(m_fightPetId);
		msg.m_Data.set_hungry(pPet->GetHungry());
		m_role->Send(msg);
		CTimerMgr::Instance()->KillTimer(m_timehandler);
	}
}

void PetSys::UpdateRankList(UINT64 petId, bool isRelease)
{
	Pet* pPet = GetPet(petId);
	if (pPet == NULL)
	{
		return ;
	}

	RpcG2M_UpdateMSRankList* pMsg = RpcG2M_UpdateMSRankList::CreateRpc();
	pMsg->m_oArg.set_ranktype(KKSG::PetPowerPointRank);
	KKSG::RankData* pData = pMsg->m_oArg.mutable_info();

	if (isRelease) // 放生坐骑  删除排行榜数据
	{
		pData->set_petuid(pPet->uid);
		pData->set_petid(0);
		MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
		return ;
	}

	pData->set_petuid(pPet->uid);
	pData->set_petid(pPet->petid);
	pData->set_roleid(m_role->GetID());
	pData->set_powerpoint(pPet->power);
	pData->set_time(time(NULL));
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
}

void PetSys::UpdateAllPetRankList()
{
	time_t nowTime = time(NULL);
	for (auto i = m_petsMap.begin(); i != m_petsMap.end(); i++)
	{
		Pet* pPet = &i->second;
		pPet->RecalcPower(m_role);
		RpcG2M_UpdateMSRankList* pMsg = RpcG2M_UpdateMSRankList::CreateRpc();
		pMsg->m_oArg.set_ranktype(KKSG::PetPowerPointRank);
		KKSG::RankData* pData = pMsg->m_oArg.mutable_info();
		pData->set_petuid(pPet->uid);
		pData->set_petid(pPet->petid);
		pData->set_roleid(m_role->GetID());
		pData->set_powerpoint(pPet->power);
		pData->set_time(nowTime);
		MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
	}
}

void PetSys::NotifyTransferPetChange(Pet& souce_pet,Pet& dest_pet,std::map<UINT32, UINT32>& getSkillMap)
{
	PtcG2C_PetChangeNotfiy msg;
	msg.m_Data.set_type(KKSG::PetExpTransfer);	

	// souce 降低了
	souce_pet.Update();		
	KKSG::PetSingle* soucepet_single = msg.m_Data.add_pet();
	souce_pet.ToKKSG(*soucepet_single);

	// dest 升级了
	dest_pet.Update();
	KKSG::PetSingle* destpet_single = msg.m_Data.add_pet();	
	dest_pet.ToKKSG(*destpet_single);

	foreach (i in getSkillMap)
	{
		KKSG::petGetSkill* pSkill = msg.m_Data.add_getskills();
		pSkill->set_petlvl(i->first);
		pSkill->set_skillid(i->second);
	}

	m_role->Send(msg);
	LogDebug("getSkillMap size =%d",getSkillMap.size());
}


// gm 接口
void PetSys::TestAddExp(const UINT32 test_exp)    
{
	auto pet = GetFight();
	if (NULL != pet)
	{
		pet->SetPetExp(test_exp,m_role);
		LogDebug("test pet level= %d  max_level=%d",pet->level,pet->max_level_);
	}
}

UINT32 PetSys::GetHorseSize()
{
	return m_petsMap.size();
}

bool PetSys::CanPairRideSelf()
{
	Pet* pet = GetFollow();
	if (NULL != pet)
	{
		return false;
	}
	return true;
}

bool PetSys::CanPairRideOther(bool isinitiative)
{
	if (0 != GetCopilotRoleId())
	{
		return false;
	}
	Pet* otherpet = GetFollow();
	if (NULL == otherpet)
	{
		return false;
	}
	if (!PetConfigMgr::Instance()->CanPairRide(otherpet->petid))
	{
		return false;
	}
	if (!isinitiative && !otherpet->canpairride)
	{
		return false;
	}
	return true;
}

bool PetSys::PushPairRideInvite(KKSG::PetInviteInfo& invite)
{
	for (auto i = m_invites.begin(); i != m_invites.end(); ++i)
	{
		if (invite.roleid() == i->roleid())
		{
			return false;
		}
	}
	if (m_invites.size() >= PET_INVITE_MAX)
	{
		m_invites.pop_front();
	}
	m_invites.push_back(invite);
	return true;
}

void PetSys::ResetPairRide(bool isstatechange)
{
	Role* otherrole = RoleManager::Instance()->FindByRoleID(m_followroleid);
	if (NULL != otherrole && otherrole != m_role)
	{
		otherrole->Get<PetSys>()->ResetPairRideData(false);
	}
	ResetPairRideData(isstatechange);
}

void PetSys::ResetPairRideData(bool isstatechange)
{
	if (!isstatechange && m_role->GetState().statetype() == KKSG::OutLook_RidePetCopilot)
	{
		m_role->SetStateRaw(KKSG::OutLook_Normal, 0, 0);

		RoleOutLook outlook(m_role);
		outlook.Fill(SUMMARY_STATE, NULL);
		outlook.BroadCast();
	}
	m_followroleid = 0;
	m_ispassive = false;

	m_role->GetRoleView()->SetBindRole(0, false);
	m_role->GetRoleView()->ClearMustSee();
}

void PetSys::SetPairRide(UINT64 other, bool ispassive)
{
	SetFollowRoleId(other);
	SetPassive(ispassive);
	m_role->GetRoleView()->SetMustSee(other);
	m_role->GetRoleView()->SetBindRole(other, ispassive ? true : false);
}

UINT32 PetSys::GetFollowPetPPT()
{
	Pet* pet = GetFollow();	
	if (NULL != pet)
	{
		return pet->power;
	}
	return 0;
}
