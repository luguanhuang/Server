#ifndef __CHECKINMGR_H__
#define __CHECKINMGR_H__

#include "table/CheckinTable.h"
#include "pb/project.pb.h"

struct CanCheckinInfo
{
	UINT32 dwCanCheckDay;
	UINT32 dwStartDay;

	CanCheckinInfo()
	{
		dwCanCheckDay = 0;
		dwStartDay = 0;
	}
};

struct ItemDesc;
class CCheckinRecord;
class Role;
class CCheckinMgr
{
	CCheckinMgr();
	~CCheckinMgr();
	DECLARE_SINGLETON(CCheckinMgr);
public:
	bool Init();
	void Uninit();

	void Reset();
	bool LoadFile();
	bool Reload();

	bool OnTimer();

	void SendCheckinInfo(Role* pRole);

	bool IsCanCheckin(CCheckinRecord* pRecord, UINT32 dwMonthDay, UINT32 dwStartDay);

	bool OnCheckin(Role* pRole, CCheckinRecord* pRecord, UINT32 dwMonth, UINT32 dwMonthDay, UINT32& dwBonus, UINT32 dwStartDay);

	CanCheckinInfo GetCanCheckinInfo(Role* pRole, time_t dwNowTime);

	bool IsOddMonth();

private:
	void UpdateCheckinItems();

	UINT32 GetMonthDay(UINT32 dwYear, UINT32 dwMonth);

	bool GetItem(UINT32 dwMonth, UINT32 dwCheckDay, UINT32& dwBonus, ItemDesc& stItem);
	bool GetQQVipItem(Role* pRole, std::vector<ItemDesc>& stItem);
	bool GetStartUpItem(Role* pRole, std::vector<ItemDesc>& stItem);

	void Test();
	
private:
	UINT32 m_dwCurrentMonth;
	UINT32 m_timer;

	KKSG::CheckinInfoNotify m_oNotify;
	CheckinTable m_oTable;
};

#endif