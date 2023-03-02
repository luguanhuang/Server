#ifndef __ITEMFINDBACKRECORD_H__
#define __ITEMFINDBACKRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "table/ItemBackTable.h"

class Role;
class CFindBackRecord;
struct CItemFindBackInfo;
struct BaseFindBackInfo;

namespace KKSG
{
	class ItemFindBackInfo;
	class RoleFindBackRecord;
	class ItemFindBackInfoRes;
	class ItemFindBackInfo2Client;
}

enum CostType
{
	FindBack_Dragon = 1,
	FindBack_Gold   = 2,
	FindBack_Ticket = 3,
};

struct UnLockSealData
{
	UnLockSealData()
	{
		state = false;
		type = 0;
		time = 0;
	}
	bool state;
	int type;
	int time;
};

struct CItemFindBackInfo
{
	CItemFindBackInfo(CFindBackRecord* record);
	~CItemFindBackInfo();

	bool Load(const KKSG::RoleFindBackRecord* record);
	void Save(KKSG::RoleFindBackRecord* record);

	void CheckOpen();

	void AddUsedCount(int id, int subtype, int count, int dayBegin);

	void SlowUpdate();

	void DebugReset(int day);


	UINT32 GetOpenTime();

	bool IsOpen();

	bool IsDataValid();

	bool IsSealFirstUnlockDay(int time);
	bool IsSealLockedDay(int level, int day);

	UINT32 GetItemFindBackInfoRes(KKSG::ItemFindBackInfoRes& roRes);

	UINT32 GetItemFindBackReward(int id, int count, int type);

	void OnFirstEnterScene();

	bool IsUnlockedSeal(int day);

	CFindBackRecord* GetCFindBackRecord(){return m_pRecord;}

private:
	void Update(int now);
	void UpdateSealData(int now);
	UINT32 GetUnLockTime();

	void InitBackMap();

	void ChangeRegenerate();

	bool IsValidFindBackCostType(int type);

	UINT32 GenerateItemFindBackInfo2Client(KKSG::ItemFindBackInfoRes& roRes);



protected:
	CFindBackRecord* m_pRecord;
	int m_openTime;
	int m_updateTime;
	int m_unlockSealUpdateTime;
	int m_notifyBackTime;
	
	std::map<int, UnLockSealData> m_unlockSealData;

	std::map<int, BaseFindBackInfo*> m_backMap;

	Switch m_needReGenerate;
	KKSG::ItemFindBackInfoRes* m_tempInfo2ClientRes;
};

#endif
