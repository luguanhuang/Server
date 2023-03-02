#ifndef __COMBATATTRIBUTE_H__
#define __COMBATATTRIBUTE_H__

#include "combatattrdef.h"
#include "roleserilizeinterface.h"

namespace KKSG 
{
	class Attribute;
}

struct AttrDiff
{
	CombatAttrDef id;
	double        oldValue;
	double        newValue;
};

enum AttrType
{
	AT_NONE,
	AT_BASIC,
	AT_PERCENT,
	AT_TOTAL,
};

class Role;


class CombatAttribute : public RoleSerializeInterface
{
	friend class RoleManager;
	friend struct AttrHelper; 
public:
	CombatAttribute();
	~CombatAttribute();

	void Init();
	void Uninit();
	void Clear();
	bool IsLoad();

	double GetAttr(CombatAttrDef attrDef);
	void SetAttr(CombatAttrDef attrDef, double value);
	double AddAttr(CombatAttrDef attrDef, double value);

	virtual bool AutoDestory() { return false; }
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	std::vector<AttrDiff> Diff(CombatAttribute *pOldAttr);
	
	bool LoadFromAttributes(const KKSG::Attribute &attr);
	void SaveToAttributes(KKSG::Attribute &attr);
	void SaveToBriefAttributes(KKSG::Attribute &attr);
	void operator=(const CombatAttribute &attr);

	void Scale(float scale);

private:
	double CheckAttributeRange(CombatAttrDef attrDef, CombatAttrDef maxAttrDef);

	bool m_IsLoad;
	std::vector<double> m_BasicAttr;
	std::vector<double> m_PercentAttr;
};


class CombatAttributeSnapshot
{
public:
	CombatAttributeSnapshot(CombatAttribute *pAttr);
	void NotifyAttributeChange(CombatAttribute *pNewAttr, Role *pRole);

private:
	CombatAttribute m_savedAttribute;
};

#endif // __COMBATATTRIBUTE_H__