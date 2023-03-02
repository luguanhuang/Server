#ifndef __BUFFRECORD_H__
#define __BUFFRECORD_H__

#include "buff/XBuffTemplateManager.h"
#include "unit/roleserilizeinterface.h"
#include "buff/transbuff.h"

struct SBuffItem
{
	UINT32 itemid;
	UINT32 itemcount;
	UINT32 timeexpire;

	SBuffItem()
	{
		itemid = 0;
		itemcount = 0;
		timeexpire = 0;
	}
};

class DoodadItem
{
public:
	DoodadItem()
	{
		m_itemID = 0;
		m_overlapLimit = 0;
		m_curOverlap = 0;
	}

	void Init(UINT32 itemID, UINT32 overlapLimit);

	bool IsEmpty();
	bool IsFull();

	bool AddOverlap();
	void DueOverlap();
	inline UINT32 GetItemID() { return m_itemID; }

	void FillData(UINT32 index, KKSG::DoodadItemSkill& data);

private:
	UINT32 m_itemID;
	UINT32 m_overlapLimit;
	UINT32 m_curOverlap;
};

class Role;
class BuffRecord : public RoleSerializeInterface
{
public:
	BuffRecord(Role* role);
	virtual ~BuffRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update();

	void AddBuff(UINT32 type, UINT32 id, UINT32 level);
	void OnEnterScene(Scene* scene);
	void OnLeaveScene(Scene* scene);

	void AddBuffItem(UINT32 itemid);
	void DelBuffItem(UINT32 itemid);

	bool UseBuffItem(UINT32 itemid);

	bool AddDoodItem(UINT32 itemid, UINT32 doodadid);
	void GetReHorseInfo(KKSG::HorseReConnectRes *roRes);
	bool UseDoodItem(UINT32 index);
	void NotifyItemAllSkils();
	void FillItemAllSkills(KKSG::DoodadItemAllSkill& data);
	void DoodItemInfoPrint();

	std::vector<DoodadItem>* GetDoodItem();

	const std::map<UINT32, BuffDesc>* GetBuffs(UINT32 type);

	std::map<UINT32, SBuffItem>& GetBuffItems() { return m_buffitems; }

	///> ±‰…Ìbuff
	UINT32 CanTrans(int mask = 3);

	// a. enter scene
	// b. role option
	void RecoverTransBuff(bool isenterscene, int mask = 3);
	// a. use item by self or other
	// b. buff
	void AddTransBuff(UINT32 buffid, UINT32 bufflevel, UINT64 casterid);
	// 0. role option
	// 1. trans state to other state
	// 2. add new trans buff x
	// 3. leave scene within activity time
	// buff time expire x
	void DelTransBuff(int reason, int mask = 3);

	void OnAddTransBuff(UINT32 buffid, UINT32 bufflevel, UINT32 timeleft, UINT64 casteruid);
	void OnDelTransBuff(UINT32 buffid, bool istruedelete);

	UINT32 GetTransTime();

	bool IsTransforming();
	bool HasTransBuff() { return m_transformbuff.Valid(); }
	bool IsTransformPassive() { return m_transformbuff.ispassive; }

	bool IsSmalling();
	bool HasSmallBuff() { return m_smallbuff.Valid(); }

	void ChatNotifyGS(UINT64 casterroleid, UINT64 destroleid, UINT32 buffid, UINT32 time);

private:
	void DoAddBuffs(Scene* scene);

private:
	Switch m_ischanged;
	Role* m_role;

	TransformBuff m_transformbuff;
	TransformBuff m_smallbuff;

	std::map<UINT32, SBuffItem> m_buffitems;
	std::map<UINT32, std::map<UINT32, BuffDesc>> m_buffs;

	std::vector<DoodadItem> m_doodads;
};

#endif