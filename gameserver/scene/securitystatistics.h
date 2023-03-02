#ifndef __SECURITYSTATISTICS_H__
#define __SECURITYSTATISTICS_H__

#include "unit/combatattrdef.h"

#define MAX_UINT_VALUE 2147483647
#define MAX_FLOAT_VALUE 99999999999.0f
// 精度问题，弄个依旧属于最大值但比真正最大值小的
#define MAX_FLOAT_VALUE_LOWBOUND 9999999999.0f

class XSecurityDamageStatistics;
class XSecurityAttributeStatistics;
class XSecurityBuffStatistics;
class XSecuritySkillStatistics;
class XSecurityMobStatistics;
class XSecurityAIStatistics;
struct HurtInfo;
struct ProjectDamageResult;
class Unit;
struct RoleBattleRecord;
class Scene;
class XSecuritySceneStatistics;

class XSecurityStatistics
{
public:
	float _Distance;                // 位移总量
	float _InitHp;                  // 初始血量
	float _InitMp;                  // 初始蓝量
	float _FinalHp;                 // 退出副本时血量
	float _FinalMp;                 // 退出副本时蓝量

private:
	bool _bInited;

	Unit* m_pUnit;

public:
	XSecurityDamageStatistics* _DamageInfo;

	XSecurityAttributeStatistics* _AttributeInfo;

	XSecurityBuffStatistics* _BuffInfo;

	XSecuritySkillStatistics* _SkillInfo;

	XSecurityAIStatistics* _AIInfo;

	XSecurityMobStatistics* _MobInfo;
public:
	XSecurityStatistics(Unit* pUnit);

	~XSecurityStatistics();

	bool IsValid() { return _bInited; }
	void OnAttach();

	void Dump(XSecuritySceneStatistics* pSceneStatistics);

	void Reset();

	void OnStart();

	void OnEnd();
	
	void MergeToScene(XSecuritySceneStatistics* sss);

	void OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result);

	void OnReceiveDamage(const HurtInfo&  rawInput, const ProjectDamageResult& result);

	void OnAttributeChange(CombatAttrDef attr, double delta);

	bool IsUsefulAttr(CombatAttrDef attr);

	void OnMove(float distance);

	void SendRoleData(Scene* pScene, RoleBattleRecord* pBattleRecord);

	static void Append(const std::string& key, const std::string& value, std::map<std::string, std::string>& target);
	static void Append(const std::string& key, double value, std::map<std::string, std::string>& target);
	static void Append(const std::string& key, int value, std::map<std::string, std::string>& target);
	static float GetMinValue(float v) { return v > MAX_FLOAT_VALUE_LOWBOUND ? 0 : v; }
	static UINT32 GetMinValue(UINT32 v, UINT32 nullValue = 0) { return v == MAX_UINT_VALUE ? nullValue : v; }
};

#endif