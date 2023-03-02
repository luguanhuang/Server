#ifndef __ATTRIBUTEWATCHER_H__
#define __ATTRIBUTEWATCHER_H__

#include "combatattrdef.h"

class Unit;


// ������Եı仯��֪ͨ���ͻ���
class AttributeWatcher
{
public:
	struct AttrPair
	{
		CombatAttrDef attrID;
		double        attrValue;
	};

	AttributeWatcher(Unit *pUnit);

	void AddWatchedAttribute(CombatAttrDef attrID);
	void SetAttr(CombatAttrDef attrDef, double value);
	void AddAttr(CombatAttrDef attrDef, double value);
	void CheckChangedAttributeAndNotify(bool broadcast, bool showHUD = false, UINT64 casterID = 0);

private:

	std::unordered_map<int, double> m_AttrWatch;
	Unit *m_pUnit;
};

#endif // __ATTRIBUTEWATCHER_H__