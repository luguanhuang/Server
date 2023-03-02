#ifndef __FINDBACKRECORD_H__
#define __FINDBACKRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "itemfindback.h"

class Role;
namespace KKSG
{
	class ItemFindBackInfo;
	class RoleFindBackRecord;
	class ItemFindBackInfoRes;
	class ItemFindBackInfo2Client;
}


class CFindBackRecord : public RoleSerializeInterface
{
public:
	CFindBackRecord(Role* role);
	virtual ~CFindBackRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void CheckOpen();
	void Change();
	inline bool IsExpFindBackOpen(){return m_openTime > 0;}

	void Update();

	void SlowUpdate();

	void UpdateExpFindBack(int now);

	void UpdateItemFindBack(int now);

	void OnFirstEnterScene();

	UINT32 GetFindBackReward(bool isFree);

	void AddUsedCount(int type, int count);

	void AddItemUsedCount(int id, int subtype, int count, int lastRecoverTime = 0, int thisReoverTime = 0);

	void OnDayPass();

	Role* GetRole(){return m_pRole;}

	UINT32 GetItemFindBackInfoRes(KKSG::ItemFindBackInfoRes& roRes);

	void DebugReset(int day);

	CItemFindBackInfo& GetItemFindBackInfo(){return m_itemFindBackInfo;}
private:
	Role* m_pRole;
	Switch m_switch;

	int m_openTime;
	int m_updateTime;
	bool m_isFoundBack;
	std::map<int, int> m_usedCountMap_h;
	std::map<int, int> m_usedCountMap;

	CItemFindBackInfo m_itemFindBackInfo;
};

#endif