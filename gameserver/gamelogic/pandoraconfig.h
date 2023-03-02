#ifndef __PANDORACONFIG_H__
#define __PANDORACONFIG_H__

#include "PandoraHeart.h"
#include "PandoraHeartReward.h"
#include "PandoraDropList.h"

typedef PandoraHeart::RowData PandoraHeartConf;
typedef PandoraHeartReward::RowData PandoraHeartRewardConf;
typedef PandoraDropList::RowData PandoraDropConf;


struct PandoraDropData
{
	PandoraDropConf* conf;
	std::set<std::pair<UINT32, UINT32>> betterDrop;
	std::vector<std::set<std::pair<UINT32, UINT32>>> bestDrop;
};

struct ItemDesc;
class PandoraConfig
{
	PandoraConfig();
	~PandoraConfig();
	DECLARE_SINGLETON(PandoraConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const PandoraDropData* GetPandoraDropData(UINT32 pandoraDropID, UINT32 profID);
	const PandoraHeartConf* GetPandoraHeartConf(UINT32 pandoraID, UINT32 profID);
	const PandoraHeartRewardConf* GetRewardConf(UINT32 pandoraID, const ItemDesc& item);

	///> 0是找不到，1是common，2是better，3是best
	UINT32 GetItemQuality(UINT32 pandoraDropID, UINT32 profID, const ItemDesc& item);	

	inline UINT64 CombinKey(UINT32 itemid, UINT32 itemcount) { return 1LL * itemid * 10000000LL + itemcount; }

private:
	PandoraDropList m_dropTable;
	std::map<UINT32, std::vector<PandoraDropData>> m_mapDrop;

	PandoraHeart m_heartTable;
	std::map<UINT32, std::vector<PandoraHeartConf*>> m_mapHeart;

	PandoraHeartReward m_oRewardTable;
	std::map<UINT32, std::map<UINT64, PandoraHeartRewardConf*>> m_mapReward;	
};


#endif