#ifndef __XBEHITCOMPONENT_H__
#define __XBEHITCOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"
#include "pb/project.pb.h"

class XActionCurve;

class XBeHitComponent : public XActionStateComponent<XBeHitActionArgs>
{
public:
	static const UINT32 uuID = XBeHit_Component;

	XBeHitComponent(void);
	virtual ~XBeHitComponent(void);

	virtual UINT32 ID() { return XBeHitComponent::uuID; }
	virtual XPhysicsLayer CollisionLayer() { return XLayer_NoneEntity; }

	inline float TimeSpan() { return _total_time; }
	inline bool LaidOnGround(){ return GetEntity()->StateMachine()->GetCurrentState() == XState_BeHit && _hit_down; }

	inline XBeHitState CurrentStateinLogical(){ return _args.Data()->State; }

	inline UINT32 SkillFrom(){ return _args.SkillFrom; }
	inline UINT32 ResultIdx(){ return _args.ResultIdx; }
	inline UINT64 HitFrom() { return _args.HitFrom; }
	inline float ParalyzeFactor(){ return _args.Paralyze; }
	inline bool ForceToFly(){ return _change_to_fly; }

	KKSG::XQTEState GetQTESpecificPhase();
	void ReUpdatePresentStatus();

protected:
	virtual void ActionSubscribe();

	virtual void OnAttachToHost(XObject* pHost);
	virtual void Attached();
	virtual void OnDetachFromHost();

	virtual bool Begin();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);
	virtual void OnGetAction(XBeHitActionArgs* pAction);

	void TrytoTirggerQTE(bool bEnd = false);

private:
	void CalcDeltaPos(float deltaTime, float last_elapsed);

private:
	Vector3 _pos;
	Vector3 _des;

	XBeHitPhase _phase;
	KKSG::XQTEState _last_set_qte;

	float _last_offset;
	float _last_height;

	float _delta_x;
	float _delta_y;
	float _delta_z;

	float _deltaH;

	float _land_time;
	float _bounce_time;
	float _getup_time;

	float _gravity;
	float _rticalV;

	float _factor;
	float _elapsed;

	bool _change_to_fly;
	bool _hit_down;
	bool _hit_bounce;

	float _total_time;
	float _present_straight;
	float _hard_straight;
	float _offset;
	float _height;

	XActionCurve* _curve_h;
	XActionCurve* _curve_v;

	float _curve_height_scale;
	float _curve_offset_scale;
	float _curve_height_time_scale;
	float _curve_offset_time_scale;

	UINT64 _hit_by;

	bool _bHasFlyPresent;
	bool _bHasRollPresent;

	bool _bSelfHasFlyPresent;
	bool _bSelfHasRollPresent;
};

#endif	//__XBEHITCOMPONENT_H__