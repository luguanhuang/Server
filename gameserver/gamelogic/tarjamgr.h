#ifndef __TARJA_MGR_H_
#define __TARJA_MGR_H_

#include "table/PveProfessionTable.h"

class Unit;
class Role;

class TarjaMgr
{
	DECLARE_SINGLETON(TarjaMgr);
public:
	TarjaMgr();
	~TarjaMgr();
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	double SetPveAttribute(Unit* unit, Scene *pScene);
	PveProfessionTable::RowData* GetConfig(Role *pRole);
	void GiveTarjaAward(Role *pRole,Scene *pScene);
private:
	UINT32 m_nMaxAward;
	PveProfessionTable m_Table;
	std::map<UINT64,PveProfessionTable::RowData*> m_mapPveAttr;
};

#endif