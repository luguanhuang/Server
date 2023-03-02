#ifndef __XQTECOMPONENT_H__
#define __XQTECOMPONENT_H__

#include "XComponent.h"
#include "XComponentDefine.hpp"

using namespace std;

#define MAXQTE 63

class XQuickTimeEventComponent : public XComponent
{
public:
	static const UINT32 uuID = XQTE_Component;

	XQuickTimeEventComponent(void);
	virtual ~XQuickTimeEventComponent(void);

	virtual UINT32 ID() { return XQuickTimeEventComponent::uuID; }

	virtual void ActionSubscribe();

	bool OnQuickTimeAction(IArgs* pargs, void *);
	bool OnSkillFired(IArgs* pargs, void *);

	bool HasSkill(UINT32 id)
	{
		return _qte_skills.find(id) != _qte_skills.end();
	}

	inline bool IsInReservedState(){ return (_state & 0xFFE) != 0;}
	bool IsInState(UINT32 state);
	inline bool IsInAnyState() { return _state != 0; }
	bool IsInCanDashState();

	inline const set<UINT32>& GetQTESkills() const {return _qte_skills;}
private:
	UINT64 _state;
	set<UINT32> _qte_skills;
};

#endif	//__XQTECOMPONENT_H__