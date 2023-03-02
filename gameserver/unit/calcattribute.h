#ifndef __CALCATTRIBUTE_H__
#define __CALCATTRIBUTE_H__

class Role;
class CombatAttribute;

class CalcRoleAttribute
{
public:
	CalcRoleAttribute();
	~CalcRoleAttribute();

	bool SetupBasicAttribute(int level, int prof, CombatAttribute *combatAttr);
	bool Execute(Role *pRole);
	void CalcAttributeConvert(CombatAttribute *combatAttr, int prof);
	void CalcPowerPoint(CombatAttribute *combatAttr, int SkillPoints, Role* pRole, int prof = 0);
	void RerangeAttribute(CombatAttribute *combatAttr);
};

#endif // __CALCATTRIBUTE_H__