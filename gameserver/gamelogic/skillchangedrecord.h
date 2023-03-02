#ifndef __SKILLCHANGEDRECORD_H__
#define __SKILLCHANGEDRECORD_H__

#include "skilllevel.h"
#include "skill/ptcg2c_skillchangedntf.h"

class Role;

class SkillChangedRecord : public SkillChangedListener
{
public:

	SkillChangedRecord(Role *pRole);
	~SkillChangedRecord();
	virtual void OnAddSkill(UINT32 skillHash, int skillLevel);
	virtual void OnRemoveSkill(UINT32 skillHash);
	virtual void OnChangedSkillLevel(UINT32 skillHash, int skillLevel);
	virtual void OnSkillSlotChanged();

	void SetChanged() { m_changed =  true; }

	void  SendNtf();
	Role *GetRole();

private:
	bool  m_changed;
	bool  m_slotchanged;
	int   m_skillpoint;
	Role *m_pRole;
	PtcG2C_SkillChangedNtf m_oPtcNtf;
};


#endif // __SKILLCHANGEDRECORD_H__

