#ifndef __XBUFFSKILLSREPLACE_H__
#define __XBUFFSKILLSREPLACE_H__

#include "XBuff.h"

class XEntity;
class XBuffSkillsReplace : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffSkillsReplace(XBuff* pBuff);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

private:
	XBuff* m_pBuff;
	XEntity *m_pEntity;
};

#endif // __XBUFFCLEAR_H__