#ifndef __XCHARGECOMPONENT_H__
#define __XCHARGECOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XActionCurve;
class XEntity;

class XChargeComponent : public XActionStateComponent<XChargeActionArgs>
{
public:
	static const UINT32 uuID = XCharge_Component;

	XChargeComponent(void);
	virtual ~XChargeComponent(void);

	virtual UINT32 ID() { return XChargeComponent::uuID; }

protected:
	virtual void ActionSubscribe();

	virtual bool Begin();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);
	virtual void OnGetAction(XChargeActionArgs* pAction);

private:
	void Calibration();
	void HeightDrop();

	void GetNormalMove(Vector3& delta, float& h, float deltaTime);
	void GetCurveMove(Vector3& delta, float& h, float deltaTime);

	void CalibrateByMultipleDirection(Vector3& delta);

	Vector3 GetControlTowards();

private:
	bool _gravity_disabled;

	float _time_elapsed;
	float _time_span;
	float _land_time;

	float _gravity;
	float _rticalV;

	float _step_speed;
	float _offset;
	float _height;

	float _height_drop;

	Vector3 _begin_at;
	Vector3 _step_dir;
	Vector3 _curve_step_dir;

	float _last_offset_forward;
	float _last_offset_side;
	float _last_offset_up;

	XActionCurve* _curve_forward;
	XActionCurve* _curve_side;
	XActionCurve* _curve_up;

	float _distance;
	UINT64 _target;
};

#endif	//__XCHARGECOMPONENT_H__