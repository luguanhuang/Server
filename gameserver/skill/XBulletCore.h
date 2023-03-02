#ifndef __XBULLETCORE_H__
#define __XBULLETCORE_H__

#include "math/XVector3.h"
#include "entity/XEntity.h"
#include "skill/XSkillCore.h"

class XBulletCore
{
	friend class XBullet;

public:
	XBulletCore(
		UINT64 token, 
		UINT64 firer, 
		XEntity* target, 
		XSkillCore* core, 
		int idx, 
		UINT32 resultID, 
		int diviation,
		int wid);

	inline UINT64 Token(){ return _token; }

	inline UINT64 GetFirerID(){ return _firer; }
	inline UINT64 GetTargetID(){ return _target; }

	inline XEntity* GetFirer(){ return XEntity::ValidEntity(_firer); }
	inline XEntity* GetTarget(){ return XEntity::ValidEntity(_target); }
	inline XSkillCore* GetSkillCore(){ return _core; }

	inline int GetSequnce(){ return _sequnce; }
	inline int ResultTime(){ return _result_time; }
	inline UINT32 ResultID(){ return _result_id; }
	inline bool IsWarning(){ return _warning; }
	inline bool HasTarget(){ return GetTarget() != NULL; }
	inline bool FlyWithTerrain(){ return _with_terrain; }
	inline float GetVelocity(){ return _velocity; }
	inline float Life(){ return _life; }
	inline float Running(){ return _running; }
	inline float InitH(){ return _init_h; }
	inline float Radius(){ return _radius; }
	inline const Vector3& WarningPos(){ return _warning_pos; }

	inline const Vector3& Begin(){ return _begin; }
	inline const Vector3& FlyTo(){ return _fly_to; }

	const XResultData* GetResultData() const;

private:
	UINT64 _token;
	UINT64 _firer;
	UINT64 _target;

	XSkillCore* _core;
	XResultData* _result;

	int _sequnce;
	UINT32 _result_id;
	int _result_time;

	float _life;
	float _running;
	float _radius;
	float _velocity;
	float _init_h;

	bool _warning;
	bool _with_terrain;

	Vector3 _warning_pos;

	Vector3 _begin;
	Vector3 _fly_to;
};

#endif	//__XBULLETCORE_H__