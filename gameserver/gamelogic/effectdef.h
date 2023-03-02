#ifndef __EFFECTDEF_H__
#define __EFFECTDEF_H__


enum CombatEffectType
{
	CET_INVALID = 0,
	CET_Buff_ChangeAttribute = 1,		///> 填乘以1000后的值
	CET_Buff_HP = 2,					///> HP变化绝对值
	CET_Buff_Duration = 3,				///> 持续时间 毫秒
	CET_Buff_TriggerRate = 4,			///> 触发概率 * 1000
	CET_Buff_AuraRadius = 5,			///> 光环半径 * 1000
	CET_Buff_ReduceCD = 6,				///> 减CD参数 同BuffList格式
	CET_Buff_ChangeSkillDamage = 7,		///> 伤害改变比率 * 1000
	CET_Buff_ChangeDamage_Cast = 8,		///> 攻击比率 * 1000
	CET_Buff_ChangeDamage_Receive = 9,	///> 受击比率 * 1000
	CET_Buff_DOTorHOT = 10,             ///> DOT or HOT * 1000, BuffDOTValueFromCaster 有值时，也替换该列的值

	CET_Skill_Damage = 1001,			///> 直接改该技能伤害，填倍数 * 1000
	CET_Skill_CD = 1002,				///> 直接改该技能CD比率，填0~1 * 1000
	CET_Skill_AddBuff = 1003,			///> 技能start时加buff，格式同SkillList StartBuff，^分割
	CET_Skill_AddMobBuff = 1004,		///> 召唤物添加buff，id^level
};


enum EffectIDType
{
	EffectIDType_Buff = 1,
	EffectIDType_Skill = 2,
};



#endif