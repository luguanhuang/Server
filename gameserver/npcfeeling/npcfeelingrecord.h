#ifndef __NPCFEELINGRECORD_H__
#define __NPCFEELINGRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
class CombatAttribute;

//enum NFGiveCountType
//{
//	NFGCT_NORMAL = 1 << 0,
//	NFGCT_BUY = 1 << 1,
//	NFGCT_VIP = 1 << 2,
//	NFGCT_MAX,
//};
//
//enum NFGiveCTCombi
//{
//	NFGCTC_NORMAL_BUY = NFGCT_NORMAL | NFGCT_BUY,
//	NFGCTC_NORMAL_VIP = NFGCT_NORMAL | NFGCT_VIP,
//	NFGCTC_NORMAL_BUY_VIP = NFGCT_NORMAL | NFGCT_BUY | NFGCT_VIP,
//};

class CNpcFeelingRecord : public RoleSerializeInterface
{
public:
	CNpcFeelingRecord(Role* pRole);
	~CNpcFeelingRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update(); 
	void DayZero(UINT32 diffDayCount);

	void SetChanged() { m_modified.Set(); }

	void FillClientData(KKSG::NpcFlRes& roRes);
	void FillOneNpc(KKSG::NpcFlRes& roRes, UINT32 npcID);
	void FillOneUnite(KKSG::NpcFlRes& roRes, UINT32 uniteID);
	void FillClientCount(KKSG::NpcFlRes& roRes);
	void FillUniteList(KKSG::NpcFlRes& roRes);
	void ClearNpcNewFlag();

	const KKSG::NpcFeelingOneNpc* GetOneNpc(UINT32 npcID);
	void SetNpcExpLevel(UINT32 npcID, UINT32 exp, UINT32 level);
	void AddNpc(UINT32 npcID);
	void FillNpcIDs(std::set<UINT32>& npcIDs);
	UINT32 GetAllNpcTotalExp();
	UINT32 GetOneNpcTotalExp(const KKSG::NpcFeelingOneNpc* pOneNpc);

	UINT32 GetTriggerFavorLeftCount();
	void AddTriggerFavorLeftCount();
	void AddNpcFavorItem(UINT32 npcID);

	void DelNpcItem(UINT32 npcID, const KKSG::NpcLikeItem& litem);

	const KKSG::NpcFeelingUnite* GetNpcUnite(UINT32 uinteID);
	void SetNpcUniteLevel(UINT32 uniteID, UINT32 level);

	void AddExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role);
	void DelExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role);
	bool HaveExchange(UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role);
	int GetExchangeIndex(const KKSG::NpcFeelingOneNpc& oneNpc, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role);

	void AddReturnDrop(UINT32 drapID, UINT32 gtime, UINT32 mailConfID);
	void CheckReturnDrop(UINT32 now);

	bool IsSystemOpened();

	void AddGiveCount();
	UINT32 GetGiveLeftCount();
	UINT32 GetDayGiveCount();
	UINT32 GetGiveMaxCount();

	void AddExchangeCount();
	UINT32 GetExchagneLeftCount();

	void AddReturnCount();
	UINT32 GetReturnLeftCount();

	void AddBuyCount();
	UINT32 GetBuyCount();
	UINT32 GetBuyLeftCount();
	UINT32 GetBuyCostCount();

	void ResetBaseCount();

	void ApplyEffect(CombatAttribute *combatAttr);

	UINT32 GetEnhanceReduce();

	void Clear();

	void InfoPrint();
	void InfoPrint(const KKSG::NpcFeelingOneNpc& data);
	void InfoPrint(const KKSG::NpcFeelingUnite& data);

private:
	KKSG::NpcFeelingOneNpc* GetOneNpcNoConst(UINT32 npcID);

	void ResetNpcLikeItem(KKSG::NpcFeelingOneNpc& oneNpc);
	void MakeNpcNormalLikeItem(KKSG::NpcFeelingOneNpc& oneNpc);
	void MakeNpcFavorLikeItem(KKSG::NpcFeelingOneNpc& oneNpc);

	double GetAddExpWeight();

	UINT32 MaxNpcCount();


private:
	Switch m_modified;
	Role* m_pRole;

	KKSG::NpcFeelingRecord m_data;

};

#endif