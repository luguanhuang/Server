#ifndef __SPACTIVITYDEF_H__
#define __SPACTIVITYDEF_H__
#include "define/spactivityiddef.h"

enum SpActDateType
{
	SpActDateType_OpenServerDay = 1,
	SpActDateType_DefiniteDate	= 2,
	SpActDateType_EveryWeek		= 3,
	SpActDateType_Other			= 4,
};

enum SpActTaskEnum
{
	SpActTask_PassOneScene					= 1,
	SpActTask_JoinSceneType					= 2,
	SpActTask_HitWorldBoss					= 3,
	SpActTask_HitGuildBoss					= 4,
	SpActTask_Login							= 5,
	SpActTask_VipLogin						= 6,
	SpActTask_GuildDare						= 7,
	SpActTask_GuildQA						= 8,
	SpActTask_EnhanceMaster					= 9,
	SpActTask_GetJade						= 10,
	SpActTask_SendFriendGift				= 11,
	SpActTask_LevelUp						= 12,
	SpActTask_Collect						= 13,
	SpActTask_CompleteSceneTypeWithPartner	= 14,
	SpActTask_CompleteSceneType				= 15,
	SpActTask_GetActivityPoint				= 16,
	SpActTask_HelperSceneType				= 17,
	SpActTask_HelperSceneID					= 18,
	SpActTask_CompleteSceneID				= 19,
	SpActTask_JoinGuild						= 20,
	SpActTask_FriendNum						= 21,
	SpActTask_GuildDonate					= 22,
	SpActTask_SubTaskComplete				= 23,
	SpActTask_CompleteSmallDragonNest		= 24,
};

enum SpActTaskStateEnum
{
	SpActTaskState_NotComplete = 0,
	SpActTaskState_Completed	= 1,
	SpActTaskState_AlreadyGet	= 2,
};

#endif