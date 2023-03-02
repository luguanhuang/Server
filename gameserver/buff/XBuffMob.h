#ifndef __XBUFFMOB_H__
#define __XBUFFMOB_H__

#include "XBuff.h"

class XBuffMob : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffMob(uint templateID, XBuff* pBuff);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);
	virtual void OnUpdate();
	void _Mob();
private:
	XEntity *m_pEntity;
	uint m_TemplateID;
	XBuff* m_pBuff;

	UINT64 m_MobID;

	bool m_bUpdated;
};

#endif // __XBUFFCLEAR_H__