#ifndef _OH_MY_GOD_MGR_H_
#define _OH_MY_GOD_MGR_H_

#include "util/utilsingleton.h"
#include "table/GMAddItem.h"


class OhMyGodMgr : public Singleton<OhMyGodMgr>
{
public:
	bool Init();
	void Uninit();

	GMAddItem& GetGMItem() { return m_items; }

private:
	GMAddItem m_items;
};


#endif