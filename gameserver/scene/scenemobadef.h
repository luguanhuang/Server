#ifndef __SCENEMOBADEF_H__
#define __SCENEMOBADEF_H__

#define MOBA_GROUP_1 11
#define MOBA_GROUP_2 12

enum MobaHint
{
	MobaHint_Begin10 = 1001,			///> 开始游戏10秒：10秒后小兵开始进攻敌方
	MobaHint_Begin20 = 1002,			///> 开始游戏20秒：全军出击，摧毁敌方防御塔和基地

	MobaHint_SelfLevel20 = 1003,		///> 己方小兵等级20级时：我方兵线20级，即将派出高级兵种
	MobaHint_OtherLevel20 = 1004,		///> 敌方小兵等级20级时：我方兵线20级，即将派出高级兵种
	MobaHint_SelfLevel50 = 1005,		///> 己方小兵等级50级时：我方兵线50级，即将派出终级兵种
	MobaHint_OtherLevel50 = 1006,		///> 敌方小兵等级50级时：我方兵线50级，即将派出终级兵种

	MobaHint_SelfBeAttack = 1007,		///> 我方基地被攻击时（CD1分钟）：我方基地正遭受攻击，请迅速回防

	MobaHint_SelfDestoryed = 1008,		///> 我方防御塔被摧毁时：我方防御塔被摧毁
	MobaHint_OtherDestoryed = 1009,		///> 敌方防御塔被摧毁时：摧毁敌方防御塔

	MobaHint_SelfKill = 1010,			///> 我方击杀狮蝎：成功击杀狮蝎，我方将派出中立生物支援
	MobaHint_OtherSkill = 1011,			///> 敌方击杀狮蝎：敌方击杀了狮蝎，将派出中立生物进攻我方防御塔
};

#endif