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
	///> �Ƿ���ϵͳ
	optional bool issystem = 1;
	///> ����id
	optional uint32 configid = 2;
	///> ��������
	optional CustomBattleGroupType grouptype = 3;
	optional CustomBattleType type = 4;
	///> ���ѣ����ᣬ���
	optional CustomBattleScale scale = 5;
	///> ����
	optional string name = 6;
	optional string desc = 7;
	///> ����
	optional string password = 8;
	///> �ɲμ�����
	optional uint32 canjoincount = 9;
	///> ʱ��
	optional uint32 readytime = 10;
	optional uint32 battletime = 11;
	///> ��ƽģʽ
	optional bool isfair = 12;
	///> Ψһ��ʶ
	optional string token = 13;
}

message CustomBattleRank{
	optional uint64 roleid = 1;
	optional string name = 2;
	optional uint32 point = 3;
}
***************************************/

#endif