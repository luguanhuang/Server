#ifndef __XRole_H__
#define __XRole_H__

#include "XEntity.h"

class XComponent;

class XRole : public XEntity
{
	friend class Role;

public:
	void CoolDownAllSkills();
    virtual void CoolDownAtLaunch();

	UINT64 SlotLastAttackAt(int slot){ return (slot < 7 && slot >= 0) ? _last_slot_attack_time[slot] : 0; }
	UINT64 SlotLastAttackTarget(){ return _last_slot_attack_target; }

	void SetSlotLastAttackAt(int slot, UINT64 target);
	void ZeroSlotLastAttack(int slot);
	inline void ZeroSlotLastTarget(){ _last_slot_attack_target = 0; }

	virtual void LateUpdate(float fDeltaT);
	virtual void Update(float fDeltaT);

	void InstallNavComponent();

	virtual bool OnTransform(UINT32 to);

protected:
	XRole(Unit* pUnit);
	virtual ~XRole();

	//override the Initilize
	virtual bool Initilize();

private:
	void A(Scene* pScene, const Vector3& me, const Vector3& it, float r);

private:
	UINT64 _last_slot_attack_time[7];
	UINT64 _last_slot_attack_target;
};

#endif	//__XRole_H__
