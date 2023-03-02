#ifndef __SPRITE_CONFIG_MGR_H__
#define __SPRITE_CONFIG_MGR_H__

#include "table/SpriteTable.h"

typedef SpriteTable::RowData SpriteConf;
class CSpriteConfigMgr
{
	CSpriteConfigMgr();
	~CSpriteConfigMgr();
	DECLARE_SINGLETON(CSpriteConfigMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const SpriteConf* GetSpriteConf(UINT32 spriteID){ return m_oSpriteTable.GetBySpriteID(spriteID); }

private:
	SpriteTable m_oSpriteTable;
};
#endif
