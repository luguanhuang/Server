#ifndef __XFREEZECOMPONENT_H__
#define __XFREEZECOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"

class XFreezeComponent : public XActionStateComponent<XFreezeActionArgs>
{
public:
	static const UINT32 uuID = XFreeze_Component;

	XFreezeComponent(void);
	virtual ~XFreezeComponent(void);

	virtual UINT32 ID() { return XFreezeComponent::uuID; }

	inline bool IsFromHit() { return _args.FreezeFromHit; }

	inline UINT32 SkillFrom(){ return _args.SkillFrom; }
	inline UINT32 ResultIdx(){ return _args.ResultIdx; }
	inline UINT64 HitFrom() { return _args.HitFrom; }

	inline bool IsPresent() { return _args.Present; }

protected:
	virtual void ActionSubscribe();

	virtual void OnGetAction(XFreezeActionArgs* pAction);
	virtual bool Begin();
	virtual void Cancel(XStateDefine next);
	virtual void ActionUpdate(float deltaTime);

	void TrytoTirggerQTE(bool bEnd = false);

private:
	float _elapsed;
	float _height_drop;
};

#endif	//__XFREEZECOMPONENT_H__