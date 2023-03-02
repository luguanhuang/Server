#ifndef __XBUFFAURACHECK_H__
#define __XBUFFAURACHECK_H__

#include "XBuff.h"
#include "timer.h"

class XEntity;

class XBuffAuraCheck : public BuffEffect
{
public:
	static bool TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff);

	XBuffAuraCheck(XBuff *pBuff);
	virtual ~XBuffAuraCheck();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

	bool OnTimer(IArgs*, void*);

	enum ShapeType
	{
		BACT_CIRCLE = 0,
		BACT_RECTANGLE = 1,
	};

	UINT64 m_UnitID;

	ShapeType m_shapeType;

	float m_startDelay;
	float m_interval;
	float m_sqrRadius;
	float m_sqrexclusiveRadius;

	float m_halfWidth;
	float m_halfHeight;
	float m_halfExclusiveWidth;
	float m_halfExclusiveHeight;

	HTIMER m_handler;
	bool m_effectexcept;
	bool m_effectexceptself;
	std::set<UINT32> m_fightgroups;
	XBuff *m_pBuff;
private:
	bool _CanAttack(UINT32 fightgroup);
	float _GetParam(int index);
	void _KillTimer();
};

#endif // __XBUFFAURACHECK_H__