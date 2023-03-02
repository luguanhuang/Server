#ifndef __XSKILL_H__
#define __XSKILL_H__

#include "state/XStateDefine.hpp"
#include "math/XMath.h"
#include "entity/XEntity.h"
#include "table/SkillReader.h"
#include "XSkillScriptMgr.h"
#include "skill/ptcg2c_skillresult.h"

using namespace std;

typedef unsigned char byte;

struct ProjectDamageResult;
struct XHitData;
struct XResultData;

class XSkillCore;
class XSkillData;
class XSkillMgr;
class XBullet;
class XBulletCore;

class XSkill
{
public:
	static const int XJAComboSkillHash;
	static const int XArtsSkillHash;
	static const int XUltraSkillHash;
	static const int XCombinedSkillHash;

	XSkill(XEntity* firer);
	virtual ~XSkill();

	//return false means skill is finished
	bool Update(float fDeltaT);
	//fire the skill
	bool Fire(UINT64 target, XSkillCore* pCore, XAttackActionArgs* pargs);
	//stop casting
	void Cease();
	//execute
	void Execute();

	// when skill is casting, can this action be performed?
	bool CanPerformAction(XStateDefine state, UINT64 token);
	void FocusTarget(XEntity* target);

	//result
	static void SkillResult(UINT64 token, XEntity* firer, XSkillCore* core, XBullet* bullet, int triggerTime, UINT32 resultID, int resultTime, const Vector3& hitdir, XEntity* target, PtcG2C_SkillResult* oPtc);
	static void SkillResult(UINT64 token, XEntity* firer, XSkillCore* core, int triggerTime, UINT32 resultID, int resultTime, const Vector3& forward, const Vector3& position);
	static void SkillResult(XBullet* bullet, const Vector3& forward, const Vector3& position, bool bulletcycle);

	static void SkillResult_TakeEffect(XEntity* firer, XEntity* target, const ProjectDamageResult& dResult, const XSkillCore* pCore, UINT32 triggerTime, const Vector3& hurtDirection, XStrickenResponse rps, bool standOn, float paralyze);

	inline UINT64 Token() const { return _token; }
	inline INT32 Slot() const { return _slot_pos; }
	inline XEntity* Firer() const { return _firer; }
	XEntity* Target() const;
	bool HasValidTarget() const;
	inline bool Casting() const { return _casting; }
	float TimeElapsed() const;
	inline float TimeScale() const { return _time_scale; }
	inline XSkillCore* Core() const { return _core; }
	virtual XSkillCore* MainCore() const { return _core; }

	virtual bool OnResult(IArgs*, void* param);

	inline const Vector3& SkillTowardsTo() const { return _skill_forward; }
	void SetSkillTowardsTo(const Vector3& forward) { _skill_forward = forward; }

	inline virtual UINT32 GetCombinedId() const { return 0; }
	bool ExternalCallback(IArgs* args, void*);

protected:
	//the point of skill to calculate hurt and other sequence.
	virtual void Result(XResultData* data) = 0;

	inline void AddedTimerToken(HTIMER token) { _actionToken.push_back(token); }

	//update effects per frame
	virtual bool Present(float fDeltaT) = 0;
	//set skill result point here

	//reflection
	void Reflection();

	void Puppetize();
	//skill do self starting here
	virtual void Start() { /*BUG #46723*/ Reflection(); }
	//skill do self stoppage here
	virtual void Stop() { }
	//
	virtual bool Launch(XSkillCore* next) { return true; }
	//kill all timer
	void KillTimerAll();

	void CalcForward();
	void Manual(const Vector3& dir);

	bool ExternalExecute(IArgs* args, void*);

protected:
	XEntity* _firer;

	UINT64 _token;
	INT32 _slot_pos;

	XSkillCore* _core;
	XSkillData* _data;

	UINT64 _target;

	float _time_scale;
	UINT64 _fire_at;

	pSkillScript _start_method;
	pSkillScript _update_method;
	pSkillScript _stop_method;
	pSkillScript _result_method;

private:
	bool _casting;
	bool _execute;

	Vector3 _skill_forward;

	vector<HTIMER> _actionToken;

	static PtcG2C_SkillResult sPtc;
};

#endif	//__XSKILL_H__
