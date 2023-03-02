#ifndef __XBUFFCHANGEFIGHTGROUP_H__
#define __XBUFFCHANGEFIGHTGROUP_H__

#include "XBuff.h"

class XBuffChangeFightGroup : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffChangeFightGroup(XBuff *pBuff, UINT32 fightgroup);
	~XBuffChangeFightGroup();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

private:
	XBuff *m_pBuff;
	XEntity *m_pEntity;
	UINT32 m_FightGroup;
	UINT32 m_PreservedFightGroup;
};

#endif // __XBUFFCHANGEATTRIBUTE_H__