#ifndef __COMBATATTRDEF_H__
#define __COMBATATTRDEF_H__

enum CombatAttrRange
{
	// ����������
	CA_ATTRIBUTE_COUNT = 301,

	// ��������
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
	DefAttr(Strength, 1, "����"),\
	DefAttr(Agility,  2, "����"),\
	DefAttr(Intelligence, 3, "����"),\
	DefAttr(Vitality, 4, "����"),\
	DefAttr(CombatSocre, 5, "ս����ƽ"),\
	DefAttr(PhysicalAtk, 11, "��������"),\
	DefAttr(PhysicalDef, 12, "���������"),\
	DefAttr(MaxHP, 13, "����ֵ����"),\
	DefAttr(CurrentHP, 14, "��ǰ����ֵ"),\
	DefAttr(HPRecovery, 15, "�����ظ��ٶ�"),\
	DefAttr(PhysicalAtkMod, 16, "������������ֵ"),\
	DefAttr(PhysicalDefMod, 17, "�������������ֵ"),\
	DefAttr(MaxSuperArmor, 18, "��󳬼�����"),\
	DefAttr(CurrentSuperArmor, 19, "��ǰ��������"),\
	DefAttr(SuperArmorRecovery,20, "�������׻ظ� - Boss�ƽ������״̬�Ļָ��ٶ�"),\
	DefAttr(MagicAtk, 21, "ħ��������"),\
	DefAttr(MagicDef, 22, "ħ��������"),\
	DefAttr(MaxMP, 23, "���ħ��ֵ"),\
	DefAttr(CurrentMP, 24, "��ǰħ��ֵ"),\
	DefAttr(MPRecovery, 25, "ħ���ظ��ٶ�"),\
	DefAttr(MagicAtkMod, 26, "ħ������������ֵ"),\
	DefAttr(MagicDefMod, 27, "ħ������������ֵ"),\
	DefAttr(Critical, 31, "����һ��"),\
	DefAttr(CritResist, 32, "����һ���ֿ�"),\
	DefAttr(Paralyze, 33, "Ӳֱ"),\
	DefAttr(ParaResist,  34, "Ӳֱ�ֿ�"),\
	DefAttr(Stun, 35, "ѣ��"),\
	DefAttr(StunResist,  36, "ѣ�εֿ�"),\
	DefAttr(HurtInc,     50, "�˺�����"),\
	DefAttr(CurrentXULI,        51, "������"),\
	DefAttr(SuperArmorAtk,  52, "�������׹�"),\
	DefAttr(SuperArmorDef,  53, "�������׷�"),\
	DefAttr(SuperArmorReg,  54, "�������ظ� - һ������µ�ͨ�ûָ��ٶȣ��ͻ�Ѫ������һ��"),\
	DefAttr(XULI,        61, "��������"),\
	DefAttr(CritDamage, 111, "����һ���˺�"),\
	DefAttr(FinalDamage, 112, "�����˺�"),\
	DefAttr(TrueDamage, 113, "��ʵ�˺�"),\
	DefAttr(FireAtk, 121, "�𹥻�"),\
	DefAttr(FireDef, 122, "�����"),\
	DefAttr(WaterAtk, 123, "ˮ����"),\
	DefAttr(WaterDef, 124, "ˮ����"),\
	DefAttr(LightAtk, 125, "�⹥��"),\
	DefAttr(LightDef, 126, "�����"),\
	DefAttr(DarkAtk, 127, "������"),\
	DefAttr(DarkDef,  128, "������"),\
	DefAttr(VoidAtk, 129, "�����Թ���"),\
	DefAttr(VoidDef,  130, "�����Է���"),\
	DefAttr(RUN_SPEED,  201, "�ܲ��ٶ�"),\
	DefAttr(WALK_SPEED,  202, "��·�ٶ�"),\
	DefAttr(DASH_SPEED,  203, "�����ٶ�"),\
	DefAttr(ROTATION_SPEED,  204, "ת���ٶ�"),\
	DefAttr(AUTOROTATION_SPEED, 205, "�Զ�ת���ٶ�"),\
	DefAttr(ATTACK_SPEED, 206, "�����ٶ�"),\
	DefAttr(SKILL_CD, 207, "����CD"),\
	DefAttr(ENMITY, 208, "���Ӱ����"),\
	DefAttr(POWERPOINT, 300, "ս����")


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