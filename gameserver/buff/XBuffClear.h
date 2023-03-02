#ifndef __XBUFFCLEAR_H__
#define __XBUFFCLEAR_H__

#include "XBuff.h"

class XBuffClear : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffClear(const vector<int>&);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual bool CanBuffAdd(BuffTable::RowData * pBuffData);

private:
	XEntity *m_Entity;
	std::set<int> m_Types;
};

#endif // __XBUFFCLEAR_H__