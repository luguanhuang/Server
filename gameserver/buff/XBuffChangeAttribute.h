#ifndef __XBUFFCHANGEATTRIBUTE_H__
#define __XBUFFCHANGEATTRIBUTE_H__

#include "XBuff.h"
#include "unit/attributewatcher.h"

class Unit;
class CombatEffectHelper;

class PercentWatcher
{
public:
	PercentWatcher(Unit* pUnit, CombatAttrDef attr, double targetDeltaPercent, AttributeWatcher& watcher);
	void Check();
	inline double GetTargetDelta() { return m_TargetDeltaBasic; }
	inline CombatAttrDef GetBasicAttr() { return m_BasicAttr; }
	inline bool IsValid() { return m_bValid; }
private:
	void _ChangeBasic();

	bool m_bValid;
	double m_TargetDeltaPercent;
	double m_TargetDeltaBasic;
	//double m_OldBasic;
	//CombatAttrDef m_PercentAttr;
	CombatAttrDef m_BasicAttr;
	Unit* m_pUnit;
	AttributeWatcher* m_Watcher;
};

class XBuffChangeAttribute : public BuffEffect
{
public:
	static bool TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff);

	XBuffChangeAttribute(XBuff *pBuff);
	~XBuffChangeAttribute();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);
	virtual void OnAttributeChanged(CombatAttrDef attr);
	virtual void OnBattleEnd();
	void ChangeAttr(Unit* pUnit, CombatAttrDef attrKey, double attrValue, AttributeWatcher& attrWatcher);
private:
	void _Convert(Unit* pUnit, CombatAttrDef attrDef, double deltaValue);

private:
	XBuff *m_pBuff;
	bool m_bChanging;

	std::set<CombatAttrDef> m_SetConvertor;
	std::map<int, double> m_ConvertorDeltaValue;

	typedef std::pair<CombatAttrDef, double> AttrPair;
	///> ԭʼ���ݴ���� ð����������
	std::vector<AttrPair> m_AdditionalAttrs;

	///> ԭʼ���ԣ���������Effect��ġ�ֻ�е���Effectʱ�������������ݣ�����ֱ����Table����������
	std::vector<Sequence<float, 3>> m_OriginalAttrs;
	bool m_bOnlyFromTable;
};

#endif // __XBUFFCHANGEATTRIBUTE_H__