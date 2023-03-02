#ifndef __CUSTOMBATTLEDEF_H__
#define __CUSTOMBATTLEDEF_H__

#include "pb/enum.pb.h"
#include "pb/project.pb.h"

#define CustomBattlePk_SceneId 62
#define CustomBattlePkTwo_SceneId 66

struct CustomBattleMatchSceneInfo
{
	bool issystem;
	bool isfaire;
	UINT64 battleid;
	UINT32 mapid;

	CustomBattleMatchSceneInfo()
	{
		issystem = false;
		isfaire = true;
		battleid = 0;
		mapid = 0;
	}
};


/**************************************
message CustomBattleDataRole{
	optional CustomBattleData data = 1;
	optional CustomBattleRole role = 2;
}

message CustomBattleRole{
	optional uint64 uid = 1;
	optional uint64 creator = 2;
	optional uint32 lose = 3;
	optional uint32 win = 4;
	optional uint32 point = 5;
	optional CustomBattleRoleState state = 6;
}

message CustomBattleData{
	optional uint64 uid = 1;
	optional uint64 creator = 2;
	optional CustomBattleConfig config = 3;
	repeated CustomBattleRank rank = 4;
}

message CustomBattleConfig{
	///> 是否是系统
	optional bool issystem = 1;
	///> 配置id
	optional uint32 configid = 2;
	///> 比赛类型
	optional CustomBattleGroupType grouptype = 3;
	optional CustomBattleType type = 4;
	///> 好友，公会，跨服
	optional CustomBattleScale scale = 5;
	///> 描述
	optional string name = 6;
	optional string desc = 7;
	///> 密码
	optional string password = 8;
	///> 可参加人数
	optional uint32 canjoincount = 9;
	///> 时间
	optional uint32 readytime = 10;
	optional uint32 battletime = 11;
	///> 公平模式
	optional bool isfair = 12;
	///> 唯一标识
	optional string token = 13;
}

message CustomBattleRank{
	optional uint64 roleid = 1;
	optional string name = 2;
	optional uint32 point = 3;
}
***************************************/

#endif