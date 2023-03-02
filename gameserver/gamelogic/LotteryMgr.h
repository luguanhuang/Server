#ifndef __LOTTERYMGR_H__
#define __LOTTERYMGR_H__

#include "item.h"
#include "table/ShareRandomGift.h"

typedef ShareRandomGift::RowData ShareRandomGiftConf;

class Role;

// ³é½±·½·¨
class IDrawHandler
{
public:
	virtual ~IDrawHandler() {}
	virtual int  GetDrawType() = 0;
	virtual bool TakeCost(Role *pRole) = 0;
	virtual std::vector<ItemDesc> DrawARaffle(Role *pRole) = 0;
};

class LotteryMgr
{
	LotteryMgr();
	~LotteryMgr();
	DECLARE_SINGLETON(LotteryMgr)

public:

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	
	const ShareRandomGiftConf* GetShareGiftConf(UINT32 id) { return m_oShareGift.GetByID(id); };

	void GiveShareRandomGift(Role* pRole, UINT32 sceneType);

	void RegistDrawHandler(IDrawHandler *handler);

	bool TakeCost(int type, Role *pRole);
	std::vector<ItemDesc> DoDrawARaffle(int type, Role *pRole, std::vector<UINT32>& ppt);
	
private:
	IDrawHandler * FindHandler(int type);

	std::list<IDrawHandler *> m_Handlers;

	ShareRandomGift m_oShareGift;
	std::unordered_map<UINT32, std::vector<ShareRandomGiftConf*>> m_shareGiftMap;
};

#endif // __LOTTERYMGR_H__