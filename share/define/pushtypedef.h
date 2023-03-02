#ifndef _PUSH_TYPE_DEFINE_H_
#define _PUSH_TYPE_DEFINE_H_

enum PushType
{
	Push_Stamina = 1,		// 体力
	Push_GuildBoss = 2,		// 公会boss
	Push_WorldBoss = 3,		// 世界boss
	Push_LevelSeal = 4,		// 等级封印
	Push_SuperRisk = 5,		// 大冒险
};

enum SuperRiskPushType
{
	SuperRiskPush_Dice = 1,		// 骰子
	SuperRiskPush_Box = 2,		// 箱子
};

#endif