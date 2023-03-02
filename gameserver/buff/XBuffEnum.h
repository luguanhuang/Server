#ifndef __XBUFFENUM_H__
#define __XBUFFENUM_H__

enum XBuffMergeType
{
	XBuffMergeType_Replace = 0,
	XBuffMergeType_ExtendTime = 1,
	XBuffMergeType_Stacking = 2,
	XBuffMergeType_Reset = 3
};

enum XBuffType
{
	XBuffType_Immortal  = 1,    // 无敌
	XBuffType_Bati      = 2,    // 霸体
	XBuffType_Freeze    = 3,    // 定身
	XBuffType_CantDie	= 4,	// 一滴血
	XBuffType_Shield	= 5,	// 保护盾
	XBuffType_Trapped	= 6,	// 缠绕
	XBuffType_Silencing	= 7,	// 沉默
	XBuffType_Puzzled	= 8,	// 混乱
	XBuffType_Transform = 9,	// 变身
	XBuffType_Stealth	= 10,	// 隐身
	XBuffType_LockFoot	= 11,	// 无法移动
	XBuffType_Mark		= 12,	// 蜜汁标记
	XBuffType_Scale		= 13,	// 缩放
	XBuffType_Max
};

enum XBuffEffectPrioriy
{
	XBuffEffectPrioriy_START	= 0,
	XBuffEffectPrioriy_Default	= XBuffEffectPrioriy_START,
	XBuffEffectPrioriy_SpecialState_Trapped,
	XBuffEffectPrioriy_TargetLifeAddAttack,
	XBuffEffectPrioriy_TriggerByHit,
	XBuffEffectPrioriy_ReduceDamage,
	XBuffEffectPrioriy_SpecialState_Immortal,
	XBuffEffectPrioriy_SpecialState_Shield,
	XBuffEffectPrioriy_SpecialState_CantDie,
	XBuffEffectPrioriy_TriggerByBeHit,
	XBuffEffectPrioriy_DamageReflection,
	XBuffEffectPrioriy_TriggerByHit_Death,
	XBuffEffectPrioriy_END
};

enum XBuffTag
{
	XBuffTag_SpriteEffect = 1,		// 精灵Buff触发后的最终效果
};

enum XBuffRemoveReason
{
	XBuffRemoveReason_Replace	= 0,
	XBuffRemoveReason_Timeout,
	XBuffRemoveReason_Manual
};

#define BUFF_INFINITY_TIME (-1)

struct ChangeAttributeArgs
{
	int AttrID;
	double AttrValue;
};

#endif // __XBUFFENUM_H__