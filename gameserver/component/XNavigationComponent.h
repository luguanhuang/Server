#ifndef __XNAVIGATION_COMPONENT_H__
#define __XNAVIGATION_COMPONENT_H__

#include "component/XComponent.h"

//目前不给玩家用
class XNavigationComponent : public XComponent
{
public:
	XNavigationComponent();
	~XNavigationComponent();

	static const UINT32 uuID = XNAVIGATION_COMPONENT;
	virtual UINT32 ID() { return XNavigationComponent::uuID; };

	virtual void Update( float fDeltaT );
	
	bool IsOnUse() { return m_onUse; }
	void SetUseState(bool onUse , const Vector3& destPos = Vector3::zero );
	bool CheckMoveDest(const Vector3& destPos);
	void StopMove();

private:

	void MoveTo(const Vector3& myPos , const Vector3& destPos);
	void InitData();
	void CheckReachThrough();
	
	bool m_onUse;
	int m_nextIndex;//对应waypoint里面的位置
	int m_destIndex;
	Vector3 m_destPos;
	time_t m_nextUpTime;
};

#endif