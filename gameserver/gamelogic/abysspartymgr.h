#ifndef __ABYSS_PARTY_MGR_H_
#define __ABYSS_PARTY_MGR_H_

#include "table/AbyssPartyTypeTable.h"


class Role;
class AbyssPartyMgr
{
	DECLARE_SINGLETON(AbyssPartyMgr);
public:
	AbyssPartyMgr();
	~AbyssPartyMgr();
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	void GetNeedItem(UINT32 nDiff,std::vector<UINT32> &itemConf);
	bool CheckLevel(Role *pRole,UINT32 nDiff);
private:

	AbyssPartyTypeTable m_PartyType;
	std::map<UINT32,UINT32> m_mapNeedItem;
};

#endif