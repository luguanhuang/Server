#ifndef __AINODEACTION_H__
#define __AINODEACTION_H__

#include "ainode.h"
#include "scene/scene.h"

class AINodeAction:public AINodeBase
{
public:
	AINodeAction(){m_type = NODE_TYPE_ACTION;}
	virtual ~AINodeAction(){};
};

class ReceiveAIEvent : public AINodeAction
{
public:
	ReceiveAIEvent(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	bool mDeprecate;
	int mAIArgMsgType;
	std::string mAIArgMsgStr;
	std::string mAIArgTypeIdName;
	int mAIArgTypeId;
	std::string mAIArgPos;
	std::string mAIArgFloatArg;
	float mAIArgFloat;

	UINT32 mStrIndex;
	UINT32 mTypeIdIndex;
	UINT32 mPosIndex;
	UINT32 mWaveIdIndex;
	UINT32 mUnitIdIndex;
	UINT32 mSkillIndex;
	UINT32 mFloatArgIndex;
	UINT32 mSenderUIDIndex;
};

class SendAIEvent : public AINodeAction
{
public:
	SendAIEvent(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	int mAIArgMsgTo;
	int mAIArgMsgType;
	int mAIArgEntityTypeId;
	std::string mAIArgMsgStr;
	float mAIArgDelayTime;
	int mWaveIdIndex;
	int mUnitIdIndex;
	int mPosIndex;
	Vector3 mPos;
	float mFloatArg;
};

class CallMonsterArgs : public IArgs
{
public:
	int MonsterTemplateId;
	int CopyMonsterId;
	Vector3 MonsterPos;
	UINT64 MonsterUnitId;
	UINT32 SceneID;
	int WaveId;
	UINT64 OldMonsterUnitId;
	bool EnableDebug;
	float FaceDir;
	float HPPercent;
};

class KillMonsterArgs : public IArgs
{
public: 
	int MonsterTemplateId;
	UINT32 SceneID;
};

class CallScriptArgs : public IArgs
{
public:
	UINT32 SceneID;
	std::string CmdString;
};

class AIDoodadArgs : public IArgs
{
public:
	UINT32 DoodadID;
	UINT32 WaveID;
	Vector3 DoodadPos;
	UINT32 SceneId;
};

class CallMonster : public AINodeAction
{
public:
	CallMonster(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
	bool KillSpawn(IArgs*, void*);
	bool LateSpawn(IArgs*, void*);

private:
	std::string DistName;
	float DistValue;
	std::string ArgAngleName;
	float AngleValue;
	int MonsterId;
	int MonsterIDIndex;
	string MonsterIDName;

	int CopyMonsterId;
	int MaxMonsterNum;
	float MonsterLifeTime;
	float MonsterDelayTime;
	std::string EventPosName;
	Vector3 EventPos;
	int BornPosType; // 0: eventpos, 1: randompos
	Vector3 pos[4];
	Vector3 FinalPos;
	UINT32 SpawnMonsterId;
	bool ForcePlace;
	float DeltaArg;
	float HPPercent;

	UINT32 mWaveIdIndex;
	UINT32 mUnitIdIndex;
	UINT32 mEventPosIndex;
	UINT32 mAngleIndex;
};

class MixMonsterPos : public AINodeAction
{
public:
	MixMonsterPos(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int MonsterTemplate0;
	int MonsterTemplate1;
	int MonsterTemplate2;
};

class KillMonster : public AINodeAction
{
public:
	KillMonster(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
	bool DelayKillMonster(IArgs*, void*);

private:
	int MonsterId;
	float DelayTime;
};

class AddBuff : public AINodeAction
{
public:
	AddBuff(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int MonsterId;
	string MonsterIdName;
	int MonsterNameIndex;
	int BuffId;
	string BuffIdName;
	int BuffIdIndex;
	int BuffId2;
	string BuffId2Name;
	int AddBuffTarget; // 0: monster  1: player
	string AddBuffTargetName;
	int AddBuffWay;  // 0: all 1: random one 2; prof
	string AddBuffWayName;
	int PlayerProfId; // player proffesion id
	string PlayerProfIdName;
};

class RemoveBuff : public AINodeAction
{
public:
	RemoveBuff(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int MonsterId;
	int BuffId;
};

class CallScript : public AINodeAction
{
public:
	CallScript(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
	bool LateCall(IArgs*, void*);

private:
	std::string FuncName;
	float DelayTime;
};

class DetectEnemyInRange : public AINodeAction
{
public:
	DetectEnemyInRange(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int RangeType;
	float Radius;
	float Angle;
	float Length;
	float Width;
	float OffsetAngle;
	float OffsetLength;
	UINT32 MonsterType;
	int FightGroupType;

};

class XHashFunc : public AINodeAction
{
public:
	XHashFunc(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	string HashName;
	string ResName;
	UINT32 ResNameIndex;
	int ResValue;
};

class AIDoodad : public AINodeAction
{
public:
	AIDoodad(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
	bool DelayDoodad(IArgs*, void*);

private:
	int doodadIndex;
	int doodadId;
	int waveid;
	int posIndex;
	Vector3 doodadPos;
	float randompos;
	float delaytime;
};

class RemoveSceneBuff : public AINodeAction
{
public:
	RemoveSceneBuff(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int doodadid;
};

class RandomEntityPos : public AINodeAction
{
public:
	RandomEntityPos(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int TemplateId;
	float Radius;
	Vector3 CenterPos;
	Vector3 FinalPos;
	int NearPlayerTemplateId;
};

class SelectPlayerFromList : public AINodeAction
{
public:
	SelectPlayerFromList(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int SelectType;
	int StartIndex;
	int EndIndex;
	UINT32 TargetIndex;
};

class GetEntityPos : public AINodeAction
{
public: 
	GetEntityPos(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int IsPlayer;
	int TemplateId;
	int StorePosIndex;
};

class GetEntityCount : public AINodeAction
{
public:
	GetEntityCount(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
private:
	int Way;
	int StoreCountIndex;
};

class SetEnmity : public AINodeAction
{
public: 
	SetEnmity(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

	enum Type
	{
		Invalid = 0,
		HateValue = 1,
		Provoke,
		ProvokeIfNotProvoked,
		ClearAll
	};
public:
	Type OperateType;
	float OperateValue; ///> when type == 1: add or reduce value; when type == 2 or 3: set provoke when > 0, remove provoke when < 0, value means time
	int OperateValueIndex;
};

class GetMonsterID : public AINodeAction
{
public:
	GetMonsterID(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int MonsterType;
	int StoreIDIndex;
};

class GetUID : public AINodeAction
{
public:
	GetUID(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int StoreIDIndex;
};

class AddGroupLevel : public AINodeAction
{
public:
	AddGroupLevel(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int FightGroupIndex;
	int FightGroup;
	int Level;
};

class NotifyTarget : public AINodeAction
{
public:
	NotifyTarget(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
};

class GetFightGroup : public AINodeAction
{
public:
	GetFightGroup(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

public:
	int UIDIndex;
	int StoreFightGroupIndex;
};

#endif