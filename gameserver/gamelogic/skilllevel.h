#ifndef __SKILLEVEL_H__
#define __SKILLEVEL_H__

#include "define/itemdef.h"
#include "skilldefine.h"
#include "syncskill.h"
#include "unit/roleserilizeinterface.h"

#define SKILL_SET_MAX 2

typedef std::list<skillInfo> SkillInfoList;

class SkillChangedListener
{
public:
	virtual void OnAddSkill(UINT32 skillHash, int skillLevel) = 0;
	virtual void OnRemoveSkill(UINT32 skillHash) = 0;
	virtual void OnChangedSkillLevel(UINT32 skillHash, int skillLevel) = 0;
	virtual void OnSkillSlotChanged() = 0;
};

struct SkillOneSet
{
	///> 携带的技能
	std::vector<UINT32> m_skillslot;
	///> 所有学习的技能
	SkillInfoList m_skills;
};

class Role;
class Scene;
struct InitSkill;
class XEntity;
class RoleSkillSet : public RoleSerializeInterface, public ISkillSet
{
public:
	RoleSkillSet(Role *pRole);
	virtual ~RoleSkillSet();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	virtual UINT32 GetSkillLevel(UINT32 skillhash);
	virtual void RefreshSelfLinkedLevels(Unit* pUnit);
	virtual const SkillFlags* GetSkillFlags() { return &m_SkillFlags; }

	void CheckInit();

	void FillRecord(KKSG::SkillRecord* record);
	UINT32 GetSkillOriginLevel(UINT32 skillhash);

	///> 创角色初始化技能
	void LearnInitSkill(InitSkill *pInitSkill);
	///> 学习普通技能
	int TryLevelup(UINT32 skillhash, bool isbind = false);

	SkillSetSyncToOtherPlayer GetSyncSkill();

	// bind
	int BindSkillSlot(UINT32 slot, UINT32 skillhash);
	skillInfo* GetSkillBySlot(UINT32 slot);
	inline UINT32 GetBindSkill(UINT32 slot) { return (*m_skillslot)[slot]; }
	inline const std::vector<UINT32>& GetBindSkillIDS() const { return *m_skillslot; }

	// reset
	KKSG::ErrorCode ResetSkill();
	void ResetSkillTwo();
	void ResetInitSkill();

	// flags
	void RefreshSkillFlags();

	// change skill set
	bool ChangeSkillSet(UINT32 index);

	bool IsFreeResetSkill() { return m_freeresetskill; }
	void SetFreeResetSkill(bool isset) { m_freeresetskill = isset; m_modified.Set(); }

	inline SkillChangedListener* GetListener() { return m_pListener; }
	void SetListener(SkillChangedListener *pListener) { m_pListener = pListener; }

	typedef SkillInfoList::iterator iterator;
	inline iterator begin() { return m_skills->begin(); }
	inline iterator end() { return m_skills->end(); }

	UINT32 GetUsedSkillPoint();
	UINT32 GetSkillSetIndex() { return m_skillindex; }
	UINT32 GetSkillPointType() { return 0 == m_skillindex ? SKILL_POINT : SKILL_POINT_TWO; }
	UINT32 GetSkillPointTypeTwo() { return 0 == m_skillindex ? SKILL_POINT_TWO : SKILL_POINT; }
	void CastEnterSceneSkill(Scene* scene);

	int GMTryLevelup(UINT32 skillhash);
	void DumpPrintSkill();
	void DebugClearSkill();
	void DoTxLog();
	std::string GetLogSkill(std::vector<UINT32>& skillslot);

	// transform
	void ResetTranform();
	void OnTransform(UINT32 transid);
	void OnTransLevel(UINT32 skillhash);
	void NotifyTransLevel();
	KKSG::ErrorCode LevelTransformSkill(int type, UINT32 point, UINT32 level, UINT32 skillhash);

private:
	void ClearBindSkill();
	skillInfo* FindSkillInfo(UINT32 skillhash);
	bool CheckProfession(UINT32 expectProfession, UINT32 realProfession);
	void AddSkill(UINT32 skillhash, UINT32 skillLevel, UINT32 skillpoint, bool isBasic);
	
	void GetEntity();
private:
	Role  *m_pRole;
	XEntity* m_pEntity;
	Switch m_modified;

	bool m_freeresetskill;
	UINT32 m_skillindex;
	SkillFlags m_SkillFlags;

	///> 携带的技能
	std::vector<UINT32>* m_skillslot;
	///> 所有学习的技能
	SkillInfoList* m_skills;

	SkillOneSet m_skillsets[SKILL_SET_MAX];
	SkillChangedListener *m_pListener;

	///> 变身后的技能
	bool m_canleveltrans;
	UINT32 m_transid;
	UINT32 m_transformskillpoint;
	std::map<UINT32, skillInfo> m_transformskills;
};

#endif // __SKILLEVEL_H__