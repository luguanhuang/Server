#ifndef __XSKILLCOMPONENT_H__
#define __XSKILLCOMPONENT_H__

#include "XComponent.h"
#include "skill/XSkill.h"
#include "state/XStateDefine.hpp"
#include "table/XEntityPresentation.h"
#include "table/XSkillEffectMgr.h"

class XSkill;
class XEntity;
class XStateMachine;
class XSkillMgr;

/*
 *Architecture:
 * XEntity (XSkillComponent.dtor) -->
 *		XSkillComponent (XSkillMgr.dtor()) -->
 *			XSkillMgr (XSkillCore.dtor() & XSkill.dtor()) -->
 *				XSkillCore (XSkillData.dtor()) -->
 *					XSkillData
 *				XSkill (carrier)
 */

class XSkillComponent : public XComponent
{
public:
	static const UINT32 uuID = XSkill_Component;

	XSkillComponent(void);
	virtual ~XSkillComponent(void);

	virtual UINT32 ID() { return XSkillComponent::uuID; }

	virtual void Update(float fDeltaT);
	void EndSkill();

	inline XSkill* CurrentSkill() { return _skill; }
	inline bool IsCasting() { return _skill != NULL && _skill->Casting(); }

	bool IsOverResults();

	inline XSkillMgr* SkillManager() { return _skillMgr; }

	bool CanPerformAction(XStateDefine state, XActionArgs* pargs);
	void ReAttachSkill(UINT32 presentid);

	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();

	const std::vector<UINT64>& GetSkillMobs(){ return _skill_mobs; }

	bool AddSkillMob(XEntity* mob);

	void RemoveSkillMob(UINT64 id)
	{
		std::vector<UINT64>::iterator it = std::find(_skill_mobs.begin(), _skill_mobs.end(), id);
		if (it != _skill_mobs.end()) _skill_mobs.erase(it);
	}

	bool HasSkillReplaced() { return !_skills_replace.empty(); }
	XSkillCore* TryGetSkillReplace(UINT32 skillID, XSkillCore* soul);
	void SetSkillReplace(UINT32 from, UINT32 to);

protected:
	virtual void ActionSubscribe();

	void AttachFilter(uint id, const SkillList::RowData* data, const XEntityPresentation::RowData* pData, const string& skill_prefix);
	void OtherAttachFilter(uint id, const SkillList::RowData* data, const XEntityPresentation::RowData* pData, const string& skill_prefix);
	//message func
	bool OnPresevedStrengthStop(IArgs* pargs, void *);
	bool OnAttack(IArgs* pargs, void *);
	bool OnRealDead(IArgs* pargs, void *);
	bool OnLeaveScene(IArgs* pargs, void *);

	bool OnAddSkill(IArgs* pargs, void *);
	bool OnRemoveSkill(IArgs* pargs, void *);
	bool OnDynamicCDRatioChanged(IArgs* pargs, void *);

	void AttachSkill();
	void InnerAttachSkill(const XEntityPresentation::RowData* pData);
	XSkillCore* PhysicalAttackAdaptor(XSkillCore* soul);

	bool AutoEnd(IArgs* pargs, void *);

private:
	XSkill* _skill;
	XSkillMgr* _skillMgr;

	XStateMachine* _machine;

	UINT64 _last_target;
	HTIMER _skill_timer;

	std::vector<UINT64> _skill_mobs;
	std::unordered_map<UINT32, UINT32> _skills_replace;
};

#endif	//__XSKILLCOMPONENT_H__
