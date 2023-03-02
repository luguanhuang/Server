#include "pch.h"
#include "XBuffReduceSkillCD.h"
#include "component/XComponent.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "util/XCommon.h"
#include "skill/XSkillMgr.h"
#include "gamelogic/combateffect.h"

bool XBuffReduceSkillCD::TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff)
{
	if (pEffectHeler->GetBuffData()->ReduceSkillCD.empty() && !pEffectHeler->HasEffect(CET_Buff_ReduceCD))
		return false;

	pBuff->AddBuffEffect(new XBuffReduceSkillCD(pBuff));
	return true;
}

XBuffReduceSkillCD::XBuffReduceSkillCD(XBuff *pBuff)
{
	m_pBuff = pBuff;
}

XBuffReduceSkillCD::~XBuffReduceSkillCD()
{

}

void XBuffReduceSkillCD::_AddCD(XSkillMgr* pSkillMgr, UINT32 skillHash, UINT32 value, UINT32 type)
{
	switch(type)
	{
	case 0:
		{
			float cd = value * 0.001f;
			pSkillMgr->Accelerate(skillHash, cd, false);
		}
		break;
	case 1:
		{
			float cdRatio = value * 0.01f;
			pSkillMgr->Accelerate(skillHash, cdRatio, true);
		}
		break;
	case 2:
		{
			float cdRatio = value * 0.01f;
			pSkillMgr->AccelerateStaticCD(skillHash, cdRatio);
		}
		break;
	default:
		break;
	}
}

void XBuffReduceSkillCD::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;
	XSkillMgr* pSkillMgr = pEntity->SkillManager();
	if (pSkillMgr == NULL)
		return;

	if (pEffectHelper->HasEffect(CET_Buff_ReduceCD))
	{
		for(auto it = m_pBuff->GetBuffInfo()->ReduceSkillCD.begin(); it != m_pBuff->GetBuffInfo()->ReduceSkillCD.end(); ++it)
		{
			Sequence<UINT32, 3> data;
			data[0] = XCommon::XHash((*it)[0].c_str());

			if((*it)[2].length() > 0)
				data[2] = convert<UINT32>((*it)[2]);

			data[1] = convert<UINT32>((*it)[1]);
			m_Datas.push_back(data);
		}

		pEffectHelper->GetBuffReduceSkillCD(m_Datas);

		for(auto it = m_Datas.begin(); it != m_Datas.end(); ++it)
		{
			_AddCD(pSkillMgr, (*it)[0], (*it)[1], (*it)[2]);
		}
	}
	else
	{
		for(auto it = m_pBuff->GetBuffInfo()->ReduceSkillCD.begin(); it != m_pBuff->GetBuffInfo()->ReduceSkillCD.end(); ++it)
		{
			UINT32 skillid = XCommon::XHash((*it)[0].c_str());

			UINT32 type = 0;
			if((*it)[2].length() > 0)
				type = convert<UINT32>((*it)[2]);

			_AddCD(pSkillMgr, skillid, convert<UINT32>((*it)[1]), type);
		}
	}

	m_pBuff->GetEffectData()->bReduceCD = true;
}


void XBuffReduceSkillCD::_RemoveCD(XSkillMgr* pSkillMgr, UINT32 skillHash, UINT32 value, UINT32 type)
{
	switch(type)
	{
	case 2:
		{
			pSkillMgr->ResetStaticCD(skillHash);
		}
		break;
	default:
		break;
	}
}

void XBuffReduceSkillCD::OnRemove(XBuffRemoveReason reason)
{
	XSkillMgr* pSkillMgr = m_pEntity->SkillManager();
	if (pSkillMgr == NULL)
		return;

	if (!m_Datas.empty())
	{
		for(auto it = m_Datas.begin(); it != m_Datas.end(); ++it)
		{
			_RemoveCD(pSkillMgr, (*it)[0], (*it)[1], (*it)[2]);
		}
	}
	else
	{
		for(auto it = m_pBuff->GetBuffInfo()->ReduceSkillCD.begin(); it != m_pBuff->GetBuffInfo()->ReduceSkillCD.end(); ++it)
		{
			UINT32 skillid = XCommon::XHash((*it)[0].c_str());

			UINT32 type = 0;
			if((*it)[2].length() > 0)
				type = convert<UINT32>((*it)[2]);

			_RemoveCD(pSkillMgr, skillid, convert<UINT32>((*it)[1]), type);
		}
	}
}
