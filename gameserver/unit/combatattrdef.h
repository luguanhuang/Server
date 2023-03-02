#ifndef __COMBATATTRDEF_H__
#define __COMBATATTRDEF_H__

enum CombatAttrRange
{
	// 属性总数量
	CA_ATTRIBUTE_COUNT = 301,

	// 属性区段
	CA_BASIC_START   = 0,
	CA_BASIC_END     = CA_BASIC_START + CA_ATTRIBUTE_COUNT,
	CA_PERCENT_START = 1000,
	CA_PERCENT_END   = CA_PERCENT_START + CA_ATTRIBUTE_COUNT,
	CA_TOTAL_START   = 2000,
	CA_TOTAL_END     = CA_TOTAL_START + CA_ATTRIBUTE_COUNT,
};

#define DefAttr(X, NUM, Comment) BASIC_##X = CA_BASIC_START + NUM, \
	PERCENT_##X = CA_PERCENT_START + NUM, \
	TOTAL_##X = CA_TOTAL_START + NUM

#define AttributeDef \
	DefAttr(Strength, 1, "力量"),\
	DefAttr(Agility,  2, "敏捷"),\
	DefAttr(Intelligence, 3, "智力"),\
	DefAttr(Vitality, 4, "体质"),\
	DefAttr(CombatSocre, 5, "战力配平"),\
	DefAttr(PhysicalAtk, 11, "物理攻击力"),\
	DefAttr(PhysicalDef, 12, "物理防御力"),\
	DefAttr(MaxHP, 13, "生命值上限"),\
	DefAttr(CurrentHP, 14, "当前生命值"),\
	DefAttr(HPRecovery, 15, "生命回复速度"),\
	DefAttr(PhysicalAtkMod, 16, "物理攻击力修正值"),\
	DefAttr(PhysicalDefMod, 17, "物理防御力修正值"),\
	DefAttr(MaxSuperArmor, 18, "最大超级盔甲"),\
	DefAttr(CurrentSuperArmor, 19, "当前超级盔甲"),\
	DefAttr(SuperArmorRecovery,20, "超级盔甲回复 - Boss破解后虚弱状态的恢复速度"),\
	DefAttr(MagicAtk, 21, "魔法攻击力"),\
	DefAttr(MagicDef, 22, "魔法防御力"),\
	DefAttr(MaxMP, 23, "最大魔法值"),\
	DefAttr(CurrentMP, 24, "当前魔法值"),\
	DefAttr(MPRecovery, 25, "魔法回复速度"),\
	DefAttr(MagicAtkMod, 26, "魔法攻击力修正值"),\
	DefAttr(MagicDefMod, 27, "魔法防御力修正值"),\
	DefAttr(Critical, 31, "致命一击"),\
	DefAttr(CritResist, 32, "致命一击抵抗"),\
	DefAttr(Paralyze, 33, "硬直"),\
	DefAttr(ParaResist,  34, "硬直抵抗"),\
	DefAttr(Stun, 35, "眩晕"),\
	DefAttr(StunResist,  36, "眩晕抵抗"),\
	DefAttr(HurtInc,     50, "伤害加深"),\
	DefAttr(CurrentXULI,        51, "蓄力条"),\
	DefAttr(SuperArmorAtk,  52, "超级盔甲攻"),\
	DefAttr(SuperArmorDef,  53, "超级盔甲防"),\
	DefAttr(SuperArmorReg,  54, "超级盔回复 - 一般情况下的通用恢复速度，和回血，回蓝一样"),\
	DefAttr(XULI,        61, "蓄力能力"),\
	DefAttr(CritDamage, 111, "致命一击伤害"),\
	DefAttr(FinalDamage, 112, "最终伤害"),\
	DefAttr(TrueDamage, 113, "真实伤害"),\
	DefAttr(FireAtk, 121, "火攻击"),\
	DefAttr(FireDef, 122, "火防御"),\
	DefAttr(WaterAtk, 123, "水攻击"),\
	DefAttr(WaterDef, 124, "水防御"),\
	DefAttr(LightAtk, 125, "光攻击"),\
	DefAttr(LightDef, 126, "光防御"),\
	DefAttr(DarkAtk, 127, "暗攻击"),\
	DefAttr(DarkDef,  128, "暗防御"),\
	DefAttr(VoidAtk, 129, "无属性攻击"),\
	DefAttr(VoidDef,  130, "无属性防御"),\
	DefAttr(RUN_SPEED,  201, "跑步速度"),\
	DefAttr(WALK_SPEED,  202, "走路速度"),\
	DefAttr(DASH_SPEED,  203, "闪避速度"),\
	DefAttr(ROTATION_SPEED,  204, "转身速度"),\
	DefAttr(AUTOROTATION_SPEED, 205, "自动转身速度"),\
	DefAttr(ATTACK_SPEED, 206, "攻击速度"),\
	DefAttr(SKILL_CD, 207, "技能CD"),\
	DefAttr(ENMITY, 208, "仇恨影响力"),\
	DefAttr(POWERPOINT, 300, "战斗力")


enum CombatAttrDef
{
	Invalid = -1,
	AttributeDef
};

#undef DefAttr

enum RecalAttrReason
{
	RecalAttr_EnterScene = 0,
	RecalAttr_LevelUp = 1,
	RecalAttr_SetProfession = 2,
	RecalAttr_IDIPUpdateExp = 3,
	RecalAttr_GMSetLevel = 4,

	RecalAttr_WearItem = 5,
	RecalAttr_Jade = 6,
	RecalAttr_Enchant = 7,
	RecalAttr_Enhance = 8,
	RecalAttr_Emblem = 9,
	RecalAttr_Smelting = 10,

	RecalAttr_Fashion = 11,
	RecalAttr_Designation = 12,
	RecalAttr_Atlas = 13,
	RecalAttr_RandomAttr = 14,
	RecalAttr_Skill = 15,
	RecalAttr_GuildSkill = 16,
	RecalAttr_Sprite = 17,
	RecalAttr_Title = 18,
	RecalAttr_Pet = 19,

	RecalAttr_ChangeSkillSet = 20,

	RecalAttr_ChangeProfession = 21,

	RecalAttr_GMAttr = 22,

	RecalAttr_UpgradeEquip = 23,
	
	RecalAttr_FuseEquip = 24,
	RecalAttr_Artifact = 25,
};

class AttributeDefInfo
{
public:
	static int GetAttributeOffset(int attrID);
	static const char *GetAttributeName(CombatAttrDef attr);
	static bool IsBasicAttr(CombatAttrDef attrDef);
	static bool IsPercentAttr(CombatAttrDef attrDef);
	static bool IsTotalAttr(CombatAttrDef attrDef);
	static bool _InitFirstLevel();
	static CombatAttrDef UsedAttributeArray[];
	static int UsedAttributeCount;

	static CombatAttrDef GetBasicAttr(CombatAttrDef attrDef) { return CombatAttrDef(attrDef % 1000); }
	static bool IsFirstLevelAttr(CombatAttrDef attrDef);
	static CombatAttrDef GetAttrCurAttr(CombatAttrDef limitAttr);
};

#endif // __COMBATATTRDEF_H__