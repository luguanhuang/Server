#ifndef __JADEMGR_H__
#define __JADEMGR_H__

#include "itemconfig.h"
#include "bag.h"
#include "table/JadeEquipTable.h"
#include "table/JadeTransferTable.h"
#include "table/JadeSlotTable.h"

#define JADESLOTCOUNT 4
#define JADESLOTCLOSE (0x0f)
#define JADESLOTNEVER 0

#define JADESLOTMASK (0x0f)
#define JADESLOTMASKCOUNT 4

#define JADEINVALIDSLOT (UINT32)(-1)
#define JADEMINLEVEL (UINT32)(-2)
#define JADEWRONGTYPE (UINT32)(-3)
#define JADEREPLACE (UINT32)(-4)

struct JadeComposeInfo
{
	UINT32 dwCount;
	UINT32 dwNextJade;

	JadeComposeInfo()
	{
		dwCount = 0;
		dwNextJade = 0;
	}
	void Init(UINT32 dwTempCount, UINT32 dwTempNextJae)
	{
		dwCount = dwTempCount;
		dwNextJade = dwTempNextJae;
	}
};


struct XItem;
struct ItemDesc;
class Role;
class CJadeMgr
{
	CJadeMgr();
	~CJadeMgr();
	DECLARE_SINGLETON(CJadeMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetSlotInfo(UINT32 level, UINT32 pos);

	UINT32 GetEmptySlot(JadeInfo* info, UINT32 level, UINT32 pos);

	///> ����װ��������Ϸ��ԣ����Ϸ�ж�·Żر�����
	void UpdateJadeOnEquip(Role* pRole, XItem* equip);

	///> ��Ƕ/ж�� ����
	KKSG::ErrorCode AttachJade(Role* pRole, XItem* pEquipItem, UINT32 dwJadePos, XItem* pJadeItem);
	KKSG::ErrorCode DetachJade(Role* pRole, XItem* pEquipItem, UINT32 dwJadePos);

	void GetAllJade(XItem* item, std::map<UINT32, UINT32>& items);

	///> ����ϳ�
	bool JadeCanBeComposed(UINT32 dwItemId);
	///> �ɺϳɵ�����
	bool ComposeJade(UINT32 dwItemId, UINT32 dwItemCount, bool bIsOnce, ItemDesc& stConsumeItem, ItemDesc& stGiveItem);

	bool GetComposeInfo(UINT32 dwItemId, JadeComposeInfo& stComposeInfo);

	UINT32 GetLevel(UINT32 itemid);

	///> ��origin�ϵ������滻��dest ���滻װ��ʱ��
	void SwapJadeOnEquip(Role* pRole, XItem* origin, XItem* dest, bool isNotify);

	///> ����װ��ʱ
	void UpgradeJadeOnEquip(Role* pRole, XItem* equip, EquipConf* pEquipConf);	

	bool CanWearJade(UINT32 dwJadeID, UINT32 level); // ������ȼ�Ҫ�����ж��Ƿ�����Ƕ��װ����

	UINT32 GetTransId(UINT32 itemid, UINT32 from, UINT32 to);

	KKSG::ErrorCode GetNeedItemsComposeJade(Role* pRole, UINT32 itemid, UINT32 addLevel, bool isBody, UINT32& destid, std::map<UINT32, UINT32>& costItems);	

private:
	//JadeEquipTable m_oTable;	
	//std::map<UINT32, std::vector<std::vector<UINT32>>> m_mapJadeType;
	JadeTransferTable m_transTable;
	JadeSlotTable m_slotTable;
};

#endif