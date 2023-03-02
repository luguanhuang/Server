#ifndef __SECURITYSKILLSTATISTICS_H__
#define __SECURITYSKILLSTATISTICS_H__

#include "unit/combatattrdef.h"
#include "define.h"
#include <map>

struct HurtInfo;
struct ProjectDamageResult;
class Role;
class Unit;
class TSecBattleFlow;

class XSecuritySkillStatistics
{
public:
	class XSkillInfo
	{
	public:
		UINT32 _SkillID;
		UINT32 _IntervalMin;
		int _CastCount;
		int _AttackCount;
		float _AttackTotal;
		float _AttackMax;
		float _AttackMin;
		float _CriticalAttackMax;
		float _CriticalAttackMin;
		int _SingleAttackMaxCount;

		UINT64 _last_cast;
		INT64 _last_token;
		int _last_single_attack_count;

		void OnCast();

		void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

		void Reset();

		void Merge(const XSkillInfo& other);
	};

private:
	std::map<UINT32, XSkillInfo*> _SkillInfos;
	XSkillInfo _NormalAttackInfo;

	// QTE统计信息，只统计玩家的
	XSkillInfo _QTEAttackInfo;

private:
	XSkillInfo* _TryGetSkillInfo(UINT32 skillID);

public:

	~XSecuritySkillStatistics();

	const std::map<UINT32, XSkillInfo*>& GetSkillInfos() { return _SkillInfos;}
	void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void OnCast(UINT32 skillID);

	void Reset();

	void OnAttach(Unit* pUnit);

	void OnEnd(Unit* pUnit);

	void Merge(const XSecuritySkillStatistics& other);

	static XSecuritySkillStatistics* TryGetStatistics(Unit *pUnit);

	void SendRoleData(Role* pRole, TSecBattleFlow& logFlow);

private:
	void _SendRoleData(const XSkillInfo* skillInfo, const std::string& keywords, TSecBattleFlow& logFlow);
};

#endif