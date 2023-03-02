#include "pch.h"
#include <typeinfo>
#include "ainodeskill.h"
#include "aionlinedebugmgr.h"
#include "aibehaviortree.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "entity/XEntity.h"
#include "entity/XActionDefine.h"
#include "component/XAIComponent.h"
#include "component/XSkillComponent.h"
#include "component/XNavigationComponent.h"
#include "skill/XSkillMgr.h"
#include "unit/role.h"
#include "gamelogic/skilllevel.h"
#include "table/XSkillEffectMgr.h"
#include "aiconfig.h"
#include "table/SkillCombo.h"
#include "aitreemgr.h"
#include "scene/scene.h"
#include "gamelogic/XLevelWave.h"
#include "ainodefactory.h"
#include "scene/scenefinder.h"
#include "unit/dummyrole.h"
#include "scene/securitystatistics.h"
#include "scene/securityaistatistics.h"
#include "aionlinedebugmgr.h"

FilterSkill::FilterSkill(const tinyxml2::XMLElement* elem)
{
	if (elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
		if (attr) 
		{
			m_sharedTargetName = attr->Value();

			if (m_sharedTargetName != "")
				m_targetIndex = AINodeFactory::CreateIndexByName(m_sharedTargetName.c_str());
			else
				m_targetIndex = 0;
		}
		attr = elem->FindAttribute("UseMP");
		if (attr) attr->QueryBoolValue(&m_useMP);
		attr = elem->FindAttribute("UseName");
		if (attr) attr->QueryBoolValue(&m_useName);
		attr = elem->FindAttribute("UseHP");
		if (attr) attr->QueryBoolValue(&m_useHP);
		attr = elem->FindAttribute("UseCoolDown");
		if (attr) attr->QueryBoolValue(&m_useCoolDown);
		attr = elem->FindAttribute("UseAttackField");
		if (attr) attr->QueryBoolValue(&m_useAttackField);
		attr = elem->FindAttribute("UseCombo");
		if (attr) attr->QueryBoolValue(&m_useCombo);
		attr = elem->FindAttribute("UseInstall");
		if (attr) attr->QueryBoolValue(&m_useInstall);
		attr = elem->FindAttribute("SkillType");
		if (attr) attr->QueryIntValue(&m_ComboSkillType);
		attr = elem->FindAttribute("SkillName");
		if (attr) m_skillName = attr->Value();
		attr = elem->FindAttribute("DetectAllPlayInAttackField");
		if (attr) attr->QueryBoolValue(&m_useDetectAllInAttackField);
		attr = elem->FindAttribute("MaxSkillNum");
		if (attr) attr->QueryIntValue(&m_maxSkillNum);
		else m_maxSkillNum = 0;
	}
}

void PrintAISkill(Unit* unit, string msg)
{

	if (!unit->GetXObject()->SkillComponent()->IsCasting())
	{
		printf("The msg : %s\n", msg.c_str());
		printf("Start ++++++++++++++++++++++++++++++++++++++++++++++++\n");
		std::vector<XSkillCore*> skills = unit->GetXObject()->AIComponent()->GetCanCastSkill();
		for (UINT32 i=0; i<skills.size(); i++)
		{
			printf("Will filter skill Skill: %s \n", skills[i]->GetSoul()->Name.c_str());
		}
		printf("End ++++++++++++++++++++++++++++++++++++++++++++++++\n");
	}
}

bool FilterSkill::Update(Unit* unit)
{

	if (!XEntity::ValidEntity(unit->GetID()) || !unit->GetXObject()->AIComponent())
		AIRETURN("FilterSkill", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	AIBehaviorTree* behaviorTree = unit->BehaviorTree();
	XAIComponent* aicomponent = unit->AIComponent();
	Unit* target = unit->AIData()->GetTransformByIndex(m_targetIndex);
	std::vector<XSkillCore*> skillsnew;

	if (!ResetSkill(unit, behaviorTree, aicomponent, m_useInstall))
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Bind Skills; Failed");
		AIRETURN("FilterSkill Reset", false);
	}

	std::vector<XSkillCore*>& skills = aicomponent->GetCanCastSkill();
	double mp = unit->GetAttr(BASIC_CurrentMP);
	XSkillMgr *pSkillManager = aicomponent->GetSkillMgr();

	aicomponent->SetIgnoreSkillCD(!m_useCoolDown);

	auto it = skills.begin();

	while(it != skills.end())
	{

		if (m_useMP)
		{
			//double costMP = XSkillEffectMgr::Instance()->GetSkillCostMP(skillhash, skilllevel, unit->GetSkillCasterTypeID());
			double costMP = aicomponent->GetCostMP((*it)->ID());
			if (costMP > mp)
			{
				it = skills.erase(it);
				continue; 
			}
		}

		if (m_useName)
		{
			if ((*it)->GetSoul()->Name != m_skillName)
			{
				it = skills.erase(it);
				continue;
			}
		}

		if (m_useHP)
		{
			int nSkillHpMaxLimit = aicomponent->GetHPMax((*it)->ID());
			int nSkillHpMinLimit = aicomponent->GetHPMin((*it)->ID());
			int nHpPercent = (int) (100 * unit->GetAttr(BASIC_CurrentHP)/unit->GetAttr(TOTAL_MaxHP));

			if (!(((nSkillHpMaxLimit == 0) || (nHpPercent <= nSkillHpMaxLimit)) && ((nSkillHpMinLimit ==0) || nHpPercent >= nSkillHpMinLimit)))
			{
				it = skills.erase(it);
				continue;
			}
		}

		if (m_useCoolDown)
		{
			if (!pSkillManager->IsCooledDown((*it)))
			{
				it = skills.erase(it);
				continue;
			}
		}

		if (m_useAttackField)
		{
			if (!target)
			{
				if ((*it)->GetSoul()->NeedTarget)
				{
					it = skills.erase(it);
					continue;
				}
			}
			else
			{
				XEntity* entity = dynamic_cast<XEntity*>(target->GetXObject());

				if (entity)
				{
					// 只有怪能用这个
					if (m_useDetectAllInAttackField && unit->IsEnemy())
					{
						const std::list<Role*>& players = unit->GetCurrScene()->GetAllRoles();
						bool findplayer = false;

						for (auto i = players.begin(); i != players.end(); ++i)
						{
							XEntity* playerEntity = dynamic_cast<XEntity*>((*i)->GetXObject());

							if (playerEntity)
							{
								if ((*it)->IsInAttackField(unit->GetXObject()->GetPosition_p(), unit->GetXObject()->GetFace_p(), playerEntity))
								{
									findplayer = true;
									break;
								}
							}
						}

						if (!findplayer)
						{
							it = skills.erase(it);
							continue;
						}
					}
					else
					{
						if (!(*it)->IsInAttackField(unit->GetXObject()->GetPosition_p(), unit->GetXObject()->GetFace_p(), entity))
						{
							it = skills.erase(it);
							continue;
						}
					}
				}
				else
				{
					it = skills.erase(it);
					continue;
				}
			}
		}

		it ++;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << skills.size() << "; ");
	if (skills.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("FilterSkill", false);

	if (m_useCombo)
	{
		if(!FilterSkillByCombo(unit, behaviorTree, aicomponent, m_ComboSkillType))
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Become 0 after filter combo; Failed");
			AIRETURN("FilterSkill SkillCombo", false);
		}
		AI_DEBUG_STATEMENT(m_DebugInfo << "Final Candidates count " << skills.size() << "; ");
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("FilterSkill", true);
}

bool FilterSkill::BindSkill(XAIComponent* ai, const vector<XSkillCore*>& v)
{
	std::vector<XSkillCore*>& skills = ai->GetCanCastSkill();
	skills.clear();

	if (m_maxSkillNum != 0)
	{
		if (m_maxSkillNum < 0)
		{
			m_maxSkillNum *= -1;
			int num = m_maxSkillNum < (int)v.size() ? m_maxSkillNum:(int)v.size();

			for (int i=0; i<num; i++)
			{
				skills.push_back(v[i]);
			}
		}
		else
		{
			std::vector<XSkillCore*>& rangeskill = ai->GetRangeSkill();

			if (rangeskill.size() == 0)
			{
				for (UINT32 i=0; i<v.size(); i++)
				{
					rangeskill.push_back(v[i]);
				}

				int num = (int)v.size() - m_maxSkillNum;
				for (int i = 0; i < num; i++)
				{
					int index = XRandom::randInt(0, (UINT32)rangeskill.size());
					auto it = rangeskill.begin();
					it = it + index;
					rangeskill.erase(it);
				}
			}

			for (UINT32 i=0; i<rangeskill.size(); i++)
			{
				skills.push_back(rangeskill[i]);
			}
		}

	}
	else
	{
		for(auto i = v.begin(); i != v.end(); ++i)
		{
			skills.push_back(*i);
		}
	}


	return skills.size() != 0;
}

bool FilterSkill::BindLinkedSkill(XAIComponent* ai, const vector<XSkillCore*>& v)
{
	std::vector<XSkillCore*>& skills = ai->GetCanCastSkill();
	skills.clear();

	for(auto i = v.begin(); i != v.end(); ++i)
	{
		if (ai->GetEntity()->MobbedBy() != 0 && ai->IsLinkedSkillCannotCast((*i)->ID()))
			continue;

		skills.push_back(*i);
	}

	return skills.size() != 0;
}

bool FilterSkill::ResetSkill(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, bool useinstall)
{
	if (unit->IsDummyRole() || unit->IsRole())
	{
		return BindSkill(ai, ai->GetDummySkills());
	}
	else
	{
		if (ai->GetEntity()->MobbedBy() != 0)
			return BindLinkedSkill(ai, ai->GetSkillMgr()->GetSkillOrder());
		else
			return BindSkill(ai, ai->GetSkillMgr()->GetSkillOrder());
	}
}

bool FilterSkill::FilterSkillByCombo(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, int skillType)
{
	std::vector<XSkillCore*>& skills = ai->GetCanCastSkill();
	ai->SetSkillCombo(false);

	if (skillType == 0) // start skill
	{
		int profIndex = AIConfig::Instance()->GetProfIndex(unit);
		std::vector<string>* pskillnames = NULL;

		if (!unit->IsRole())
		{
			pskillnames = &(AIConfig::Instance()->GetStartSkillById(unit->GetTemplateID()));
		}
		else
		{
			if (!AIConfig::Instance()->HasStartSkill(profIndex))
				return false;

			pskillnames = &(AIConfig::Instance()->GetStartSkill(profIndex));
		}

		if (pskillnames == NULL || pskillnames->size() == 0)
			return false;

		const std::vector<string>& skillnames = *pskillnames;

		auto it = skills.begin();

		while (it != skills.end())
		{
			bool found = false;
			for (UINT32 i=0; i<skillnames.size(); i++)
			{
				if ((*it)->GetSoul()->Name == skillnames[i])
					found = true;
			}

			if (found)
				it ++;
			else
				it = skills.erase(it);
		}

		if (skills.size() == 0)
		{
			return false;
		}
		else
			return true;
	}
	else if (skillType == 1) // combo skill
	{
		string name = ai->GetCastingSkillName();
		//printf("The cast name: %s\n", name.c_str());
		if (!AIConfig::Instance()->HasComboSkill(name))
			return false;

		std::vector<string>& skillnames = AIConfig::Instance()->GetComboSkill(name);

		if (skillnames.size() == 0)
			return false;

		auto it = skills.begin();

		while (it != skills.end())
		{
			bool found = false;
			for (UINT32 i=0; i<skillnames.size(); i++)
			{
				if ((*it)->GetSoul()->Name == skillnames[i])
					found = true;
			}

			if (found)
				it ++;
			else
				it = skills.erase(it);
		}

		if (skills.size() == 0)
		{
			return false;
		}
		else
		{
			ai->SetSkillCombo(true);
			return true;
		}
	}
	else
		return false;
}

PhysicalAttack::PhysicalAttack( const tinyxml2::XMLElement *elem )
{
	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
		m_targetIndex = -1;
		if (attr) 
		{
			m_sharedTargetName = attr->Value();

			if (m_sharedTargetName != "")
				m_targetIndex = AINodeFactory::CreateIndexByName(m_sharedTargetName.c_str());
		}
	}
}

bool PhysicalAttack::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	XAIComponent* ai = unit->AIComponent();
	Transform target = unit->AIData()->GetTransformByIndex(m_targetIndex);

	if (!target || !XEntity::ValidEntity(target->GetID()))
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; Failed");
		AIRETURN("PhysicalAttack", false);
	}

	if (unit->GetXObject()->NavigationComponent() && unit->GetXObject()->NavigationComponent()->IsOnUse())
		unit->GetXObject()->NavigationComponent()->SetUseState(false);


	if (unit->GetSecurityStatistics() && unit->GetSecurityStatistics()->_AIInfo)
		unit->GetSecurityStatistics()->_AIInfo->OnPhysicalAttack();

	XAttackActionArgs args;
	args.Identify = ai->GetSkillMgr()->GetPhysicalIdentity();
	args.Target = target->GetID();

	if (args.Identify != 0)
	{
		unit->GetXObject()->DispatchAction(&args);
		ai->SetCasingSkillName(ai->GetSkillMgr()->GetPhysicalSkill()->GetSoul()->Name);
		AI_DEBUG_RETURN_WITHOUT_DEFINE("PhysicalAttack", true);
	}
	else
	{
		LogError("No Physical skill, templateid[%d]", unit->GetTemplateID());
		AI_DEBUG_RETURN_WITHOUT_DEFINE("PhysicalAttack", false);
	}


}

void PhysicalAttack::Print()
{
	AINodeBase::Print();
	SSInfo << VAR_NAME(m_sharedTargetName) << " : " <<m_sharedTargetName << END;
}


bool DoSelectSkillInOrder::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("DoSelectSkillInOrder", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<XSkillCore*>& skills = unit->GetXObject()->AIComponent()->GetCanCastSkill();

	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << skills.size() << "; ");

	if (skills.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectSkillInOrder", false);

	auto it = skills.begin() + 1;

	while(it != skills.end())
	{
		it = skills.erase(it);
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << (*(skills.begin()))->GetSoul()->Name << "; ");
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectSkillInOrder", true);
}

bool DoSelectSkillRandom::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("DoSelectSkillRandom", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<XSkillCore*>& skills = unit->GetXObject()->AIComponent()->GetCanCastSkill();

	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << skills.size() << "; ");

	if (skills.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectSkillRandom", false);

	int index = XRandom::randInt(0, skills.size()) % skills.size();

	auto it = skills.begin();
	int i=0;

	while(it != skills.end())
	{
		if (i!= index)
			it = skills.erase(it);
		else
			it ++;
		i++;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << (*(skills.begin()))->GetSoul()->Name << "; ");
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectSkillRandom", true);
}

DoCastSkill::DoCastSkill(const tinyxml2::XMLElement* elem)
{
	m_targetIndex = -1;
	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
		if (attr) 
		{
			m_sharedTargetName = attr->Value();

			if (m_sharedTargetName != "")
				m_targetIndex = AINodeFactory::CreateIndexByName(m_sharedTargetName.c_str());
		}
	}
}

bool DoCastSkill::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("DoCastSkill", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	Unit* target = unit->AIData()->GetTransformByIndex(m_targetIndex);
	XAIComponent* aicomponent = unit->AIComponent();
	std::vector<XSkillCore*>& skills = aicomponent->GetCanCastSkill();

	if (skills.size() == 0)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Skills Can Cast; Failed");
		AIRETURN("DoCastSkill", false);
	}

	//if (unit->GetXObject()->SkillComponent()->IsCasting() && !aicomponent->GetSkillCombo())
	if (unit->GetXObject()->SkillComponent()->IsCasting())
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Is Casting " << 	unit->GetXObject()->SkillComponent()->CurrentSkill()->MainCore()->GetSoul()->Name << "; Success");
		AIRETURN("DoCastSkill", true);
	}
	if (aicomponent->IsDebugEnable())
	{
		char infobuf[256]; 
		sprintf(infobuf, "Will cast skill: %s", skills[0]->GetSoul()->Name.c_str());
		AIOnlineDebugMgr::Instance()->SendDebugMsg(unit->GetCurrScene()->GetSceneID(), infobuf);
	}

	if (aicomponent->GetIgnoreSkillCD())
		aicomponent->GetSkillMgr()->CoolDown(skills[0]->ID());

	XAttackActionArgs args;
	args.Identify = skills[0]->ID();
	if (target)
		args.Target = target->GetID();
	else
		args.Target = 0;
	bool res = unit->GetXObject()->DispatchAction(&args);

	if (target)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Cast [" << skills[0]->GetSoul()->Name << "] at [" << target->GetID() << ", " << target->GetTemplateID() << (res ? "] Success; " : "] Failed; "));
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Cast [" << skills[0]->GetSoul()->Name << "] no target, " << (res ? "Success; " : "] Failed; "));
	}
	
	if (res && unit->GetCurrScene() && unit->GetCurrScene()->GetSpawner())
	{
		if(target)
			unit->GetCurrScene()->GetSpawner()->m_oLevelState.SetTargetRoleID(target->GetID());

		bool sendtoglobal = false;
		const string& str = skills[0]->GetSoul()->Logical.Exstring;

		if (0 == skills[0]->GetSoul()->Logical.Exstring_At && !str.empty())
		{
			if (str.find("aicommand") != string::npos)
			{
				const std::vector<std::string>& params = Split(str, ':');

				if (params.size() >= 4)
				{
					int monsterid = atoi(params[1].c_str());
					string cmd = params[2];
					Scene* scene = unit->GetCurrScene();

					if (monsterid == -1) // send to global ai
					{
						int firerTemplateId = atoi(params[3].c_str());
						FindEnemyTemplateIdCondition cond0(firerTemplateId);
						std::vector<Unit *> units0;
						scene->GetUnitByCondition(cond0, units0);

						if (units0.size() > 0)
						{
							XAIEventArgs args;
							args.MsgTargetTemplateId = 0;
							args.EventArg = cmd;
							args.Templateid = firerTemplateId;
							args.Pos = units0[0]->GetXObject()->GetPosition();
							args.FloatArg = units0[0]->GetXObject()->GetFaceDegree();
							scene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);

							AI_DEBUG_STATEMENT(m_DebugInfo << "Send GlobalAI: " << cmd << "; ");
						}
					}
					else
					{
						XAIEventArgs args;
						args.MsgTargetTemplateId = monsterid;
						args.EventArg= cmd;
						scene->MsgChannel().SendMsgTo(AISceneMsgChannel::Enemy, AISceneMsgChannel::Cmd, args);

						AI_DEBUG_STATEMENT(m_DebugInfo << "Send EnemyAI: " << monsterid << ", " << cmd << "; ");
					}
					sendtoglobal = true;;
				}
			}
		}
	}

	if (unit->GetSecurityStatistics() && unit->GetSecurityStatistics()->_AIInfo)
		unit->GetSecurityStatistics()->_AIInfo->OnSkillAttack();

	if (res)
		aicomponent->SetCasingSkillName(skills[0]->GetSoul()->Name);

	if (unit->GetXObject()->NavigationComponent() && unit->GetXObject()->NavigationComponent()->IsOnUse())
		unit->GetXObject()->NavigationComponent()->SetUseState(false);

	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoCastSkill",  true);
}

bool IsSkillChoosed::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsSkillChoosed", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<XSkillCore*>& skills = unit->GetXObject()->AIComponent()->GetCanCastSkill();
	
	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << skills.size() << "; ");

	AIRETURN("IsSkillChoosed",  skills.size() == 1);
}


bool TryCastQTE::Update( Unit* unit )
{
	if (unit->GetXObject()->SkillComponent()->SkillManager()->GetQTE().size() == 0)
		AIRETURN("TryCastQTE", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	auto it = unit->GetXObject()->SkillComponent()->SkillManager()->GetQTE().begin();

	while( it != unit->GetXObject()->SkillComponent()->SkillManager()->GetQTE().end())
	{
		auto skillids = it->second;

		for (UINT32 i=0; i<skillids.size(); i++)
		{
			XAttackActionArgs args;
			args.Identify = skillids[i];
			bool res = unit->GetXObject()->DispatchAction(&args);
			if (res)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "Cast " << args.Identify << "; ");
				AI_DEBUG_RETURN_WITHOUT_DEFINE("TryCastQTE", res);
			}
		}

		it ++;
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("TryCastQTE", false);
}

bool CastDash::Update( Unit* unit )
{
	AIRETURN("CastDash", true);
}

bool StopCastingSkill::Update(Unit* unit)
{
	//LogInfo("Will stop casting skill, templateid[%d]", unit->GetTemplateID());
	unit->GetXObject()->SkillComponent()->EndSkill();
	AI_DEBUG_RETURN("StopCastingSkill", true);
}