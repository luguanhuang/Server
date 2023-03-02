#include "pch.h"
#include "securityskillstatistics.h"
#include "gamelogic/XCombat.h"
#include "securitystatistics.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"
#include "timeutil.h"
#include "component/XSkillComponent.h"
#include "utility/tloggerbattle.h"
#include "unit/role.h"
#include "gamelogic/skilllevel.h"

void XSecuritySkillStatistics::XSkillInfo::OnCast()
{
	++_CastCount;

	UINT64 nowtime = TimeUtil::GetMilliSecond();
	
	if (_last_cast > 0)
	{
		UINT32 interval = (UINT32)(nowtime - _last_cast);
		if (interval < _IntervalMin) _IntervalMin = interval;
	}
	else
	{
		_last_cast = nowtime;
	}
}

void XSecuritySkillStatistics::XSkillInfo::OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	++_AttackCount;
	_AttackTotal += (float)result.GetValue();

	if ((result.m_flag & DMGFLAG_CRITICAL) != 0)
	{
		_CriticalAttackMax = std::max((float)result.GetValue(), _CriticalAttackMax);

		if (result.GetValue() > 0)
			_CriticalAttackMin = std::min((float)result.GetValue(), _CriticalAttackMin);
	}
	else
	{
		_AttackMax = std::max((float)result.GetValue(), _AttackMax);

		if (result.GetValue() > 0)
			_AttackMin = std::min((float)result.GetValue(), _AttackMin);
	}

	if (rawInput.m_skilltoken != _last_token)
	{
		_last_token = rawInput.m_skilltoken;
		_last_single_attack_count = 0;
	}

	++_last_single_attack_count;
	if (_last_single_attack_count > _SingleAttackMaxCount)
		_SingleAttackMaxCount = _last_single_attack_count;
}

void XSecuritySkillStatistics::XSkillInfo::Reset()
{
	_SkillID = 0;
	_IntervalMin = MAX_UINT_VALUE;
	_AttackCount = 0;
	_CastCount = 0;
	_AttackTotal = 0;
	_AttackMax = 0;
	_AttackMin = MAX_FLOAT_VALUE;
	_CriticalAttackMax = 0;
	_CriticalAttackMin = MAX_FLOAT_VALUE;
	_SingleAttackMaxCount = 0;

	_last_cast = 0;
	_last_token = 0;
	_last_single_attack_count = 0;
}

void XSecuritySkillStatistics::XSkillInfo::Merge(const XSkillInfo& other)
{
	_AttackCount += other._AttackCount;
	_AttackTotal += other._AttackTotal;
	_AttackMax = std::max(_AttackMax, other._AttackMax);
	_AttackMin = std::min(_AttackMin, other._AttackMin);
	_CriticalAttackMax = std::max(_CriticalAttackMax, other._CriticalAttackMax);
	_CriticalAttackMin = std::min(_CriticalAttackMin, other._CriticalAttackMin);
	_SingleAttackMaxCount = std::max(_SingleAttackMaxCount, other._SingleAttackMaxCount);

	_CastCount += other._CastCount;
	_IntervalMin = std::min(_IntervalMin, other._IntervalMin);
}

XSecuritySkillStatistics::XSkillInfo* XSecuritySkillStatistics::_TryGetSkillInfo(UINT32 skillID)
{
	auto it = _SkillInfos.find(skillID);
	if (it != _SkillInfos.end())
		return it->second;

	XSkillInfo* info = new XSkillInfo();
	info->Reset();
	info->_SkillID = skillID;
	_SkillInfos[skillID] = info;
	
	return info;
}

void XSecuritySkillStatistics::OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	// 加血的不算
	if (result.GetValue() >= 0)
	{
		if (rawInput.m_skillId != 0)
		{
			XSkillInfo* info = _TryGetSkillInfo(rawInput.m_skillId);
			info->OnCastDamage(rawInput, result);
		}
	}
}

void XSecuritySkillStatistics::OnCast(UINT32 skillID)
{
	XSkillInfo* info = _TryGetSkillInfo(skillID);
	info->OnCast();
}

void XSecuritySkillStatistics::Reset()
{
	for(auto it = _SkillInfos.begin(); it != _SkillInfos.end(); ++it)
	{
		delete (XSkillInfo*)(it->second);
	}
	_SkillInfos.clear();

	_NormalAttackInfo.Reset();

	_QTEAttackInfo.Reset();
}

void XSecuritySkillStatistics::OnAttach(Unit *pUnit)
{
	if (pUnit == NULL)
		return;
}

void XSecuritySkillStatistics::OnEnd(Unit* pUnit){	if (pUnit != NULL && pUnit->GetXObject() != NULL)	{		XSkillMgr* skillMgr = pUnit->GetXObject()->SkillComponent() == NULL ? NULL : pUnit->GetXObject()->SkillComponent()->SkillManager();		if (skillMgr == NULL)			return;
		for(auto it = _SkillInfos.begin(); it != _SkillInfos.end(); ++it)		{			XSkillInfo* skillInfo = it->second;			if (skillMgr->IsPhysicalAttack(skillInfo->_SkillID))			{				_NormalAttackInfo.Merge(*skillInfo);			}			else if(skillMgr->IsQTEAttack(skillInfo->_SkillID))			{				_QTEAttackInfo.Merge(*skillInfo);			}		}	}}
void XSecuritySkillStatistics::Merge(const XSecuritySkillStatistics& other)
{
	_NormalAttackInfo.Merge(other._NormalAttackInfo);
	_QTEAttackInfo.Merge(other._QTEAttackInfo);
	for (auto it = other._SkillInfos.begin(); it != other._SkillInfos.end(); ++it)
	{
		XSkillInfo* pInfo = _TryGetSkillInfo(it->first);
		pInfo->Merge(*(it->second));
	}
}

XSecuritySkillStatistics::~XSecuritySkillStatistics()
{
	Reset();
}

XSecuritySkillStatistics* XSecuritySkillStatistics::TryGetStatistics(Unit *pUnit)
{
	if (pUnit == NULL || pUnit->GetSecurityStatistics() == NULL)
		return NULL;

	return pUnit->GetSecurityStatistics()->_SkillInfo;
}

void XSecuritySkillStatistics::SendRoleData(Role* pRole, TSecBattleFlow& logFlow)
{
	if (pRole == NULL)
		return;

	{// 普攻
		XSecurityStatistics::Append("PlayerATKMax", _NormalAttackInfo._AttackMax, logFlow.m_data);
		XSecurityStatistics::Append("PlayerATKMin", XSecurityStatistics::GetMinValue(_NormalAttackInfo._AttackMin), logFlow.m_data);
		XSecurityStatistics::Append("PlayerCritATKMax", _NormalAttackInfo._CriticalAttackMax, logFlow.m_data);
		XSecurityStatistics::Append("PlayerCritATKMin", XSecurityStatistics::GetMinValue(_NormalAttackInfo._CriticalAttackMin), logFlow.m_data);
		XSecurityStatistics::Append("PlayerAtkTotal", _NormalAttackInfo._AttackTotal, logFlow.m_data);
		XSecurityStatistics::Append("PlayerAtkCount", _NormalAttackInfo._CastCount, logFlow.m_data);
		XSecurityStatistics::Append("PlayerAtkTag", _NormalAttackInfo._SingleAttackMaxCount, logFlow.m_data);
	}

	{//发QTE
		XSecurityStatistics::Append("PlayerQTEMax", _QTEAttackInfo._AttackMax, logFlow.m_data);
		XSecurityStatistics::Append("PlayerQTEMin", XSecurityStatistics::GetMinValue(_QTEAttackInfo._AttackMin), logFlow.m_data);
		XSecurityStatistics::Append("PlayerCritQTEMax", _QTEAttackInfo._CriticalAttackMax, logFlow.m_data);
		XSecurityStatistics::Append("PlayerCritQTEMin", XSecurityStatistics::GetMinValue(_QTEAttackInfo._CriticalAttackMin), logFlow.m_data);
		XSecurityStatistics::Append("PlayerQTEDps", _QTEAttackInfo._AttackTotal, logFlow.m_data);
		XSecurityStatistics::Append("PlayerQTECount", _QTEAttackInfo._CastCount, logFlow.m_data);
		XSecurityStatistics::Append("PlayerQTEHitCount", _QTEAttackInfo._AttackCount, logFlow.m_data);
		XSecurityStatistics::Append("PlayerQTETag", _QTEAttackInfo._SingleAttackMaxCount, logFlow.m_data);
	}

	XEntity *pEntity = static_cast<XEntity *>(pRole->GetXObject());
	if (pEntity == NULL)
		return;

	XSkillMgr* pSkillMgr = pEntity->SkillManager();
	if (pSkillMgr == NULL)
		return;

	RoleSkillSet* record = pRole->Get<RoleSkillSet>();

	int index = 0;
	for (int i = 0; i < KKSG::Attack_Max; ++i)
	{
		skillInfo* info = record->GetSkillBySlot(i); 
		if (NULL != info)
		{
			if(pSkillMgr->IsPhysicalAttack(info->skillhash))
				continue;

			if (pSkillMgr->GetDashIdentity() == info->skillhash)
				continue;

			if (pSkillMgr->IsQTEAttack(info->skillhash))
				continue;

			auto it = _SkillInfos.find(info->skillhash);
			if(it != _SkillInfos.end())
			{
				_SendRoleData(it->second, ToString(++index), logFlow);
			}
			else
			{
				_SendRoleData(NULL, ToString(++index), logFlow);
			}
		}
		else
		{
			_SendRoleData(NULL, ToString(++index), logFlow);
		}
	}

	auto dashIt = _SkillInfos.find(pSkillMgr->GetDashIdentity());
	if(dashIt != _SkillInfos.end())
		_SendRoleData(dashIt->second, "10", logFlow);
	else
		_SendRoleData(NULL, "10", logFlow);

}

void XSecuritySkillStatistics::_SendRoleData(const XSkillInfo* skillInfo, const std::string& keywords, TSecBattleFlow& logFlow)
{
	XSecurityStatistics::Append(("PlayerSkillMax" + keywords), skillInfo == NULL ? 0 : skillInfo->_AttackMax, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillMin" + keywords), skillInfo == NULL ? 0 : XSecurityStatistics::GetMinValue(skillInfo->_AttackMin), logFlow.m_data);
	XSecurityStatistics::Append(("PlayerCritSkillMax" + keywords), skillInfo == NULL ? 0 : skillInfo->_CriticalAttackMax, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerCritSkillMin" + keywords), skillInfo == NULL ? 0 : XSecurityStatistics::GetMinValue(skillInfo->_CriticalAttackMin), logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillHitCount" + keywords), skillInfo == NULL ? 0 : skillInfo->_AttackCount, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillDPS" + keywords), skillInfo == NULL ? 0 : skillInfo->_AttackTotal, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillCount" + keywords), skillInfo == NULL ? 0 : skillInfo->_CastCount, logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillCD" + keywords), skillInfo == NULL ? 0 : (int)XSecurityStatistics::GetMinValue(skillInfo->_IntervalMin, 9999999), logFlow.m_data);
	XSecurityStatistics::Append(("PlayerSkillTag" + keywords), skillInfo == NULL ? 0 : skillInfo->_SingleAttackMaxCount, logFlow.m_data);
}
