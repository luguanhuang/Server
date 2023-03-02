#ifndef _GLOBAL_AI_UNIT_
#define _GLOBAL_AI_UNIT_

#include "unit/unit.h"
#include "entity/XGlobalAIObject.h"

class XGlobalAIObject;

class GlobalAIUnit : public Unit
{
public:
	GlobalAIUnit();
	~GlobalAIUnit();

	void Init(Scene* scene);
	void Uninit();
	virtual std::string GetAIString();
	virtual void OnTreat(Unit *pTargetUnit, double value){}
	virtual void OnAttack(Unit *pTargetUnit, double value){}
	virtual void OnHurt(Unit* pCastUnit, double value){}
	virtual void OnDied(){}
	virtual void StreamOutput(std::ostream &stream){}

protected:
	virtual void OnEnterScene(Scene *scene){}
	virtual void OnLeaveScene(){}
private:
	XGlobalAIObject* m_pGlobalAIObject;
};

#endif