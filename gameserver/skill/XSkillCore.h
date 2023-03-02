#ifndef __XSKILLCORE_H__
#define __XSKILLCORE_H__

#include "table/SkillReader.h"
#include "state/XStateDefine.hpp"
#include "timeutil.h"
#include "util/XCommon.h"
#include "math/XVector3.h"

class XEntity;
class XSkill;
class XSkillData;
class Enemy;

using namespace std;

class XSkillCore
{
public:
	struct XSkillWarningPackage
	{
	public:
		XSkillWarningPackage()
			:WarningAt(Vector3::zero), WarningTo(0)
		{}

		Vector3 WarningAt;
		UINT64 WarningTo;
	};

	struct XWarningRandomPackage
	{
	public:
		XWarningRandomPackage(UINT64 id)
			:ID(id)
		{}

		vector<UINT32> Pos;
		UINT64 ID;
	};

public:
	XSkillCore(XEntity* firer, XSkillData* data);
	~XSkillCore();

	static XSkillCore* GetSpecificCore(XEntity* pEntity, XSkillCore* main, int triggerTime, int resultToken);

	inline XEntity* GerFirer() const { return _firer; }
	inline XSkillData* GetSoul() const { return _soul; }
	inline bool HasInitCD() const { return _init_cd > 0; }
	inline bool CooledDown() const { return !_is_init_cooling && _current_running_time > 0; }
	inline bool Reloading() const { return _is_init_cooling || (_current_running_time < _totally_running_time); }
	inline void CheckRunningTime() 
	{
		if(_current_running_time < 0) _current_running_time = 0;
		else if(_current_running_time > _totally_running_time) _current_running_time = _totally_running_time;
	}

	inline int CarrierID() const { return _carrier_id; }
	inline XSkill* Carrier() const { return _carrier; }
	inline int CurrentRunTime() const { return _current_running_time; }
	inline UINT32 ID() const { return _id; }
	inline UINT32 Level() const { return _skill_level; }

	bool Fire(XSkill* carrier);
	void Execute(XSkill* carrier);
	void OnCdCall(int left_running_time, bool syntonic = false);
	inline void Halt() { _carrier = NULL; }

	float GetTimeScale() const;

	float GetElapsedCD();
	float GetLeftCD();
	inline float GetCoolDown(){ return CheckDynamicCD(CheckStaticCD()); }

	inline float GetRange (UINT32 id) const { return (id < _soul->Result.size()) ? _soul->Result[id].Range : 0; }
	inline float GetRangeLow (UINT32 id) const { return (id < _soul->Result.size()) ? _soul->Result[id].Low_Range : 0; }
	inline float GetScope (UINT32 id) const { return (id < _soul->Result.size()) ? (_soul->Result[id].Sector_Type ? _soul->Result[id].Scope * 0.5f : _soul->Result[id].Scope) : 0; }

	inline float CastRangeUpper() const { return _soul->Cast_Range_Upper; }
	inline float CastRangeLower() const { return _soul->Cast_Range_Rect ? 0 : _soul->Cast_Range_Lower; }
	inline float CastScope() const { return _soul->Cast_Range_Rect ? _soul->Cast_Scope : _soul->Cast_Scope * 0.5f; }

	inline bool IsOnSyntonic() const { return _is_on_syntonic; }

	bool CanRotate();
	bool CanCast(UINT64 token);
	bool CanAct(XStateDefine state, const Vector3& forward);
	bool CanReplacedBy(const XSkillCore* skill);

	bool IsInAttackField(const Vector3& pos, const Vector3& forward, XEntity* target);
	bool IsInAttackField(int triggerTime, const Vector3& pos, const Vector3& forward, XEntity* target);

	XEntity* FindTargetAt(const Vector3& pos, const Vector3& forward);

	int PreservedStrength();

	void AddHurtTarget(UINT64 id, int triggerTime);
	bool IsHurtEntity(UINT64 id, int triggerTime);

	float GetMultipleDirectionFactor();

	void CoolDown();

	void AccelerateCD(float delta, bool ratio);
	void ResetStaticCD();
	void AccelerateStaticCD(float delta);

	void MakeCoolDownAtLaunch();
	void ClearHurtTarget();

	void InitCoreData(bool ctor = false);

public:
	vector<vector<XSkillWarningPackage> > WarningPosAt;
	vector<vector<XWarningRandomPackage> > WarningRandomItems;

private:
	//delegate for cool down timer
	bool OnReloaded(IArgs* pargs, void *);
	//delegate for skill notify
	bool OnCastNotify(IArgs* pargs, void *);
	/*
	 * return < 0 means target is not in the attack field of the core;
     */
	float GetAttackWeights(const Vector3& pos, const Vector3& forward, XEntity* target, bool sector);

	bool InnerIsInAttackField(int triggerTime, const Vector3& pos, const Vector3& forward, const Vector3& targetPos, float yRot);
	float InnerGetAttackWeights(const Vector3& pos, const Vector3& forward, Enemy* target, const Vector3& logical, bool sector);

	Vector3 ResultPos(const Vector3& result_pos, const Vector3& logical, float radius);

	inline float CheckStaticCD(){ return _is_init_cooling ? _init_cd : _static_cd; }
	float CheckDynamicCD(float static_cd);

	void ClearWarningPos();
	bool TryCalcRandomWarningPos();

private:
	UINT32 _magic_num;

	UINT32 _id;
	int _carrier_id;
	int _current_running_time;
	int _totally_running_time;

	int _preserved_strength;

	XSkillData* _soul;
	XSkill* _carrier;
	XEntity* _firer;

	bool  _is_init_cooling;
	bool  _ever_fired;
	bool  _is_on_syntonic;

	float _static_cd;
	float _init_cd;
	UINT32 _skill_level;

	float _last_lock_and_load_time;

	HTIMER _timer_token;

	Vector3 _offset;
	Vector3 _vecs[4];

	vector<set<UINT64> > _hurt_target;

	float _semi_dynamic_cd_ratio;
	float _last_dynamic_cd;
	float _dynamic_cd_ratio;
	float _dynamic_cd_delta;

	UINT32 _sync_at_sequence;
};

#endif	//__XSKILLCORE_H__