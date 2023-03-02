#include "pch.h"
#include "XEnmityList.h"
#include "scene/scene.h"
#include "globalconfig.h"
#include "unit/unit.h"
#include "scene/scenefinder.h"
#include "entity/XEntity.h"
#include "entity/XEnemy.h"
#include "table/XSkillEffectMgr.h"
#include "unit/role.h"
#include "component/XAIComponent.h"
#include "component/XBuffComponent.h"

float XEnmityList::OT_VALUE = 1.2f;

bool Compare(const Enmity& a, const Enmity& b)
{
	return a.value > b.value;
}

XEnmityList::XEnmityList()
{
	m_Inited = false;
	m_Host = NULL;
	m_ProvokeContain = 10000; //ms
	m_ProvokeTime = 0;
}

XEnmityList::~XEnmityList()
{
	m_HatredList.clear();
	m_ProvokeInfos.clear();
}

void XEnmityList::Init(Unit* host)
{


	if (m_Inited)
		return;

	if (!host)
		return;

	m_Host = host;
}

bool XEnmityList::IsInited()
{
	return m_Inited;
}

void XEnmityList::Reset()
{
	m_HatredList.clear();
	m_ProvokeInfos.clear();
	m_ProvokeContain = 0;
	m_ProvokeTime = 0;
}

float XEnmityList::_CalcHateValue(Unit* unit, float value)
{
	float fixedEnmity = 0; 
	float enmityCoefficient = 1.0f; 

	if (unit->IsRole())
	{
		Role* role = dynamic_cast<Role*>(unit);

		if (role)
		{
			fixedEnmity = XSkillEffectMgr::Instance()->GetFixedEnmity(role->GetProfession());
			enmityCoefficient = XSkillEffectMgr::Instance()->GetEnmityCoefficient(role->GetProfession());
			value = value * enmityCoefficient * unit->GetAttr(TOTAL_ENMITY) / GetGlobalConfig().GeneralCombatParam + fixedEnmity;
		}
	}
	
	return value;
}

void XEnmityList::AddNewHateValue(Unit* unit, float value)
{
	float v = _CalcHateValue(unit, value);
	if (v > 0.0f)
	{
		Enmity newhatred;
		newhatred.value = v;;
		newhatred.id = unit->GetID();
		newhatred.self = m_Host;
		m_HatredList.push_back(newhatred);
	}
}

void XEnmityList::AddHateValue(Unit* unit, float value)
{
	//bool found = false;

	for (auto i= m_HatredList.begin(); i != m_HatredList.end(); i++)
	{
		if (i->id == unit->GetID())
		{
			i->value += _CalcHateValue(unit, value);
			if(i->value <= 0.0f)
			{
				m_HatredList.erase(i);
			}
			return;
		}
	}

	//if (found == false)
	AddNewHateValue(unit, value);

	//UpdateHateList();
}

struct NotValidEntityPred
{
	bool operator()(Enmity& ptr) const
	{
		XEntity* valid = XEntity::ValidEntity(ptr.id);

		if (valid == NULL)
		{
			return true;
		}
		else
		{
			if (ptr.self->GetCurrScene() != valid->GetUnit()->GetCurrScene())
				return true;
			return false;
		}
	}
};

void XEnmityList::UpdateHateList()
{
	if (m_HatredList.size() == 0)
		return;

	float firstHate = m_HatredList[0].value;
	int index = 0;
	float hateValue = 0.0f;

	//移除出了首位的所有已经牺牲的对象
	NotValidEntityPred pred;
	auto removeIter = std::remove_if(m_HatredList.begin(), m_HatredList.end(), pred);
	m_HatredList.erase(removeIter, m_HatredList.end());

	if (m_HatredList.size() == 0)
		return;

	//如果仇恨列表首位挂了，更新重新排序即可
	if (!XEntity::ValidEntity(m_HatredList[0].id))
	{
		m_HatredList.erase(m_HatredList.begin());


		if (m_HatredList.size() > 1)
			sort(m_HatredList.begin(), m_HatredList.end(), Compare);
		return;
	}

	//如果没挂，看后面的人有没有谁OT
	for (UINT32 i = 1; i < m_HatredList.size(); i++)
	{
		if (index == 0)
		{
			if (m_HatredList[i].value > firstHate * OT_VALUE)
			{
				index = i;
				hateValue = m_HatredList[i].value; 
			}
		}
		else
		{
			if (m_HatredList[i].value > hateValue)
			{
				index = i;
				hateValue = m_HatredList[i].value;
			}
		}
	}

	//有人OT，换位
	if (index != 0)
	{
		Enmity temp = m_HatredList[0];
		m_HatredList[0] = m_HatredList[index];
		m_HatredList[index] = temp;
	}
}

const std::vector<Unit*>& XEnmityList::GetHateEntity(bool filterImmortal)
{
	m_Targets.clear();

	if (m_ProvokeInfos.size() > 0)
	{
		const ProvokeInfo& curProvokeInfo = m_ProvokeInfos[m_ProvokeInfos.size()-1];
		Unit* provoke = Unit::FindUnit(curProvokeInfo.id);

		if (provoke)
		{
			m_Targets.push_back(provoke);
		}
		else
		{
			m_ProvokeTime = 0;
		}
		return m_Targets;
	}
	else
	{
		m_ProvokeTime = 0;
	}
	UpdateHateList();

	if (m_HatredList.size() == 0)
		return m_Targets;

	if (filterImmortal)
	{
		float firstval = 0;
		for (UINT32 i=0; i<m_HatredList.size(); i++)
		{
			Unit* unit = Unit::FindUnit(m_HatredList[i].id);
			if (!unit || !unit->GetXObject())
				continue;

			XBuffComponent* buff = unit->GetXObject()->BuffComponent();

			if (!buff || !buff->IsBuffStateOn(XBuffType_Immortal))
			{
				if (firstval == 0)
				{
					firstval = m_HatredList[i].value;
					m_Targets.push_back(unit);
					continue;
				}
				else if (firstval == m_HatredList[i].value)
				{
					m_Targets.push_back(unit);
					continue;
				}
				else
					break;
			}
		}
	}
	else
	{
		float firstval = 0;
		for (UINT32 i=0; i<m_HatredList.size(); i++)
		{
			Unit* unit = Unit::FindUnit(m_HatredList[i].id);
			if (!unit || !unit->GetXObject())
				continue;

			if (firstval == 0)
			{
				firstval = m_HatredList[i].value;
				m_Targets.push_back(unit);
				continue;
			}
			else if (firstval == m_HatredList[i].value)
			{
				m_Targets.push_back(unit);
				continue;
			}
			else
				break;
		}
	}

	return m_Targets;
}

void XEnmityList::AddInitHateValue(Unit* unit)
{
	if (m_HatredList.size() > 0 || !unit)
		return;

	float initvalue =  GetGlobalConfig().InitEnmityValue;
	AddHateValue(unit, initvalue);
}

void XEnmityList::AddInitHateValue(Unit* unit, float value)
{
	if (unit == NULL || value <= 0.0f)
		return;

	for (auto i= m_HatredList.begin(); i != m_HatredList.end(); i++)
	{
		if (i->id == unit->GetID())
		{
			return;
		}
	}

	AddNewHateValue(unit, value);
}

void XEnmityList::RemoveHateValue(Unit* unit)
{
	RemoveProvokeUnit(unit->GetID());
	
	auto listit = m_HatredList.begin();

	while(listit != m_HatredList.end())
	{
		if ((*listit).id == unit->GetID())
		{
			listit = m_HatredList.erase(listit);
		}
		else
		{
			++listit;
		}
	}
}

void XEnmityList::RemoveProvokeUnit(UINT64 uid)
{
	for(auto it = m_ProvokeInfos.begin(); it != m_ProvokeInfos.end();)
	{
		if (it->id == uid)
		{
			it = m_ProvokeInfos.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (m_Host->GetXObject()->AIComponent()->GetTarget() == uid)
	{
		m_ProvokeContain = 0;
		UpdateProvoke();
	}

}

bool XEnmityList::SetProvokeUnitIfNotProvoked(Unit* provoke, float effecttime)
{
	if (m_ProvokeInfos.size() > 0)
		return false;

	SetProvokeUnit(provoke, effecttime);
	return true;
}

void XEnmityList::SetProvokeUnit(Unit* provoke, float effecttime)
{
	if (m_Host->IsRole())
		return;

	m_ProvokeTime = TimeUtil::GetMilliSecond();
	m_ProvokeContain = (UINT64)(effecttime*1000);
	m_ProvokeInfos.push_back(ProvokeInfo(provoke->GetID(), m_ProvokeContain, m_ProvokeTime));

	//更换目标
	m_Host->GetXObject()->AIComponent()->SetTarget(provoke->GetID());
}

void XEnmityList::UpdateProvoke()
{
	if (m_ProvokeInfos.size() == 0)
		return;

	while (TimeUtil::GetMilliSecond() - m_ProvokeTime > m_ProvokeContain) // 目前的嘲讽过时
	{
		m_ProvokeInfos.erase(m_ProvokeInfos.begin()+m_ProvokeInfos.size()-1);

		int totalSize = m_ProvokeInfos.size();

		if (totalSize == 0) // 所有的嘲讽对象都经历过了
		{
			m_Host->GetXObject()->AIComponent()->SetTarget(0);
			m_ProvokeContain = 0;
			m_ProvokeTime = 0;
			break;
		}

		{
			const ProvokeInfo& info = m_ProvokeInfos[totalSize - 1];
			Unit* provoke = Unit::FindUnit(info.id);

			if (provoke && XEntity::ValidEntity(provoke))
			{
				m_Host->GetXObject()->AIComponent()->SetTarget(info.id);
				m_ProvokeContain = info.effectTime;
				m_ProvokeTime = info.startTime;
			}
			else
			{
				m_ProvokeContain = 0;
				m_ProvokeTime = 0;
			}
		}
	}
}

void XEnmityList::Print()
{
	LogMgr& pLog = SSInfo << "HostTemplate " << m_Host->GetTemplateID();
	for(auto it = m_HatredList.begin(); it != m_HatredList.end(); ++it)
	{
		Unit* pUnit = Unit::FindUnit(it->id);
		if (pUnit == NULL)
			continue;

		pLog << " [Target " << pUnit->GetTemplateID() << ", " << it->value << "]";
	}

	pLog << END;
}

ProvokeInfo::ProvokeInfo(UINT64 _id, UINT64 _effectTime, UINT64 _startTime)
	: id(_id)
	, effectTime(_effectTime)
	, startTime(_startTime)
{
}


