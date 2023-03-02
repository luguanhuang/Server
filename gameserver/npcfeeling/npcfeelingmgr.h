#ifndef __NPCFEELINGMGR_H__
#define __NPCFEELINGMGR_H__

#include "util/utilsingleton.h"
#include "timer.h"
#include "table/NpcFeeling.h"
#include "table/NpcFeelingAttr.h"
#include "table/NpcUniteAttr.h"
#include "table/NpcFeelingSpecial.h"

class Role;
class CNpcFeelingRecord;

class CNpcFeelingMgr : public Singleton<CNpcFeelingMgr>, public ITimer
{
public:
	CNpcFeelingMgr();
	~CNpcFeelingMgr();

	bool Init();
	void Uninit();
	bool LoadFile();
	bool CheckConfig();
	void ClearConfig();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	KKSG::ErrorCode HandleGiveNpcItem(Role* pRole, const KKSG::NpcFlArgM2G& m2gArg, KKSG::NpcFlResM2G& m2gRes);
	KKSG::ErrorCode HandleUpNpcLevel(Role* pRole, UINT32 npcID);
	KKSG::ErrorCode HandleNpcUinteLevel(Role* pRole, UINT32 uniteID);
	KKSG::ErrorCode HandleExchange(Role* pRole, UINT32 npcID, const KKSG::ItemBrief& role2npc, const KKSG::ItemBrief& npc2role);
	KKSG::ErrorCode HandleBuyGiveGiftCount(Role* pRole);
	void FillBaseData(Role* pRole, KKSG::NpcFlRes& roRes);

	void OnLevelUp(Role* pRole);
	void OnSystemOpen(Role* pRole);
	void OnLogin(Role* pRole);
	
	void SyncData(const KKSG::NpcFlGlobal& data);

	void InfoPrint(Role* pRole);
	void InfoPrint(KKSG::NpcFlGlobal& data);
	void InfoPrintTable();

private:
	void CheckGiveNewNpc(Role* pRole);
	void CheckReturnMail();

	bool IsSameItem(const KKSG::ItemBrief& item1, const KKSG::ItemBrief& item2);
	bool IsSameItem(const KKSG::NpcLikeItem& item1, const KKSG::NpcLikeItem& item2);

	double GetTopAvExp();

	UINT32 GetLevelUpNeedExp(UINT32 npcID, UINT32 tolevel);
	UINT32 GetUniteNextLevel(UINT32 uniteID, UINT32 curLevel);
	bool IsNpcMaxLevel(UINT32 npcID, UINT32 level);

	void CheckTriggerExchange(Role* pRole, UINT32 npcID, KKSG::NpcFlRes& roRes);
	void CheckTriggerReturn(Role* pRole, UINT32 npcID);
	void CheckTriggerFavor(Role* pRole, UINT32 npcID);
	bool RandomPro(UINT32 pro);

	void FillNpc2Role(KKSG::NpcFlRes& roRes);

	void Sequence2NpcItem(const Sequence<UINT32,3>& seq, KKSG::NpcLikeItem& litem, KKSG::NpcFlItemType type, double addWeight);

	UINT32 GetMaxLevelLimit();

	NpcFeeling::RowData* GetConf(UINT32 npcID, UINT32 level = 0);
	NpcFeelingAttr::RowData* GetNpcAttrConf(UINT32 npcID, UINT32 level);
	NpcUniteAttr::RowData* GetUniteAttrConf(UINT32 uniteID, UINT32 level);
	NpcFeelingSpecial::RowData* GetNpcSpecial(UINT32 npcID);

	void BuildNpcLevel2Index();
	void BuildUniteLevel2Index();

private:
	HTIMER m_handler;

	NpcFeeling m_npcTable;	
	NpcFeelingAttr m_npcAttr;
	NpcUniteAttr m_unitAttr;
	NpcFeelingSpecial m_npcSpecial;

	typedef std::pair<UINT32,UINT32> NpcAttrKey;
	//< <npcid, level>, index>
	std::map<NpcAttrKey, UINT32> m_npclevel2index;
	std::map<NpcAttrKey, UINT32> m_unitelevel2index;

	KKSG::NpcFlGlobal m_globalData;

	friend class CNpcFeelingRecord;
};

#endif