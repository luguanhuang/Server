#ifndef __XBUFFKILL_H__
#define __XBUFFKILL_H__

#include "XBuff.h"

class XBuffKill : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffKill(XBuff* pBuff);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

private:
	XEntity* m_pEntity;
	XBuff* m_pBuff;
};

#endif // __XBUFFCLEAR_H__