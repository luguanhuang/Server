#ifndef __XBUFFMANIPULATE_H__
#define __XBUFFMANIPULATE_H__

#include "XBuff.h"

struct XManipulationData;
class XBuffManipulate : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffManipulate(XBuff* pBuff);
	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

private:
	float _GetParam(int index);
	XEntity *m_pEntity;

	XBuff* m_pBuff;

	int m_Token;
	XManipulationData* m_pData;
};

#endif // __XBUFFCLEAR_H__