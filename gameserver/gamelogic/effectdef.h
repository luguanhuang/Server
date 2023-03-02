#ifndef __EFFECTDEF_H__
#define __EFFECTDEF_H__


enum CombatEffectType
{
	CET_INVALID = 0,
	CET_Buff_ChangeAttribute = 1,		///> �����1000���ֵ
	CET_Buff_HP = 2,					///> HP�仯����ֵ
	CET_Buff_Duration = 3,				///> ����ʱ�� ����
	CET_Buff_TriggerRate = 4,			///> �������� * 1000
	CET_Buff_AuraRadius = 5,			///> �⻷�뾶 * 1000
	CET_Buff_ReduceCD = 6,				///> ��CD���� ͬBuffList��ʽ
	CET_Buff_ChangeSkillDamage = 7,		///> �˺��ı���� * 1000
	CET_Buff_ChangeDamage_Cast = 8,		///> �������� * 1000
	CET_Buff_ChangeDamage_Receive = 9,	///> �ܻ����� * 1000
	CET_Buff_DOTorHOT = 10,             ///> DOT or HOT * 1000, BuffDOTValueFromCaster ��ֵʱ��Ҳ�滻���е�ֵ

	CET_Skill_Damage = 1001,			///> ֱ�Ӹĸü����˺������ * 1000
	CET_Skill_CD = 1002,				///> ֱ�Ӹĸü���CD���ʣ���0~1 * 1000
	CET_Skill_AddBuff = 1003,			///> ����startʱ��buff����ʽͬSkillList StartBuff��^�ָ�
	CET_Skill_AddMobBuff = 1004,		///> �ٻ������buff��id^level
};


enum EffectIDType
{
	EffectIDType_Buff = 1,
	EffectIDType_Skill = 2,
};



#endif