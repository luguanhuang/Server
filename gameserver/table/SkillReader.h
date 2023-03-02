#ifndef __SKILLREADER_H__
#define __SKILLREADER_H__

#include <vector>
#include <string>
#include <limits.h>
#include <float.h>
#include "define.h"

using namespace std;

struct XBaseData 
{
	int Index;
	XBaseData()
	{
		Index = 0;
	}
};

enum XResultAffectDirection
{
	AttackDir, ChargeDir
};

enum XResultBulletType
{
	Sphere, Plane, Satellite, Ring
};

struct XLongAttackResultData
{
	bool Valid;
	XResultBulletType Type;
	bool WithCollision;
	bool Follow;
	float Runningtime;
	float Stickytime;
	float Velocity;
	float Radius;
	float Palstance;
	float RingRadius;
	float RingVelocity;
	bool RingFull;
	bool TriggerOnce;
	bool TriggerAtEnd;
	float TriggerAtEnd_Cycle;
	int TriggerAtEnd_Count;
	bool FlyWithTerrain;
	bool AimTargetCenter;
	bool Reinforce;
	int FireAngle;
	float At_X;
	float At_Y;
	float At_Z;
	float Refine_Cycle;
	int Refine_Count;
	bool AutoRefine_at_Half;
	bool IsPingPong;

	bool StaticCollider;
	bool DynamicCollider;

	bool Manipulation;
	float ManipulationRadius;
	float ManipulationForce;

	XLongAttackResultData()
	{
		Valid = false;
		WithCollision = true;
		Follow = false;
		Runningtime = 0;
		Stickytime = 0;
		Velocity = 0;
		Radius = 0;
		Palstance = 0;
		RingRadius = 0;
		RingVelocity = 0;
		RingFull = false;
		TriggerOnce = true;
		TriggerAtEnd = false;
		TriggerAtEnd_Cycle = 0;
		TriggerAtEnd_Count = 0;
		FlyWithTerrain = true;
		AimTargetCenter = true;
		FireAngle = 0;
		At_X = 0;
		At_Y = 0;
		At_Z = 0;
		Type = Sphere;
		Refine_Cycle = 0;
		Refine_Count = 0;
		IsPingPong = false;
		Reinforce = false;
		AutoRefine_at_Half = false;
		StaticCollider = true;
		DynamicCollider = false;
		Manipulation = false;
		ManipulationRadius = 0;
		ManipulationForce = 0;
	}
};

struct XResultData : public XBaseData
{
	bool LongAttackEffect;
	float At;
	float Range;
	float Low_Range;
	float Scope;
	bool Loop;
	bool Group;
	float Cycle;
	int Deviation_Angle;
	int Angle_Step;
	float Time_Step;
	int Group_Count;
	int Loop_Count;
	int Token;
	bool Clockwise;
	bool Warning;
	int Warning_Idx;
	bool Sector_Type;
	bool Rect_HalfEffect;
	int None_Sector_Angle_Shift;
	bool Attack_All;
	bool Mobs_Inclusived;
	bool Attack_Only_Target;
	float Offset_X;
	float Offset_Z;
	XResultAffectDirection Affect_Direction;

	XLongAttackResultData LongAttackData;

	XResultData()
	{
		LongAttackEffect = false;
		At = 0;
		Range = 0;
		Low_Range = 0;
		Scope = 0;
		Loop = false;
		Group = false;
		Cycle = 0.0f;
		Deviation_Angle = 0;
		Angle_Step = 0;
		Time_Step = 0.0f;
		Group_Count = 0;
		Loop_Count = 0;
		Token = 0;
		Clockwise = false;
		Warning = false;
		Warning_Idx = 0;
		Sector_Type = true;
		Rect_HalfEffect = false;
		None_Sector_Angle_Shift = 0;
		Attack_All = false;
		Mobs_Inclusived = false;
		Attack_Only_Target = false;
		Offset_X = 0;
		Offset_Z = 0;
		Affect_Direction = AttackDir;
	}
};

struct XChargeData : public XBaseData
{
	float At;
	float End;
	float Offset;
	float Height;
	float Velocity;
	float Rotation_Speed;
	bool  Using_Curve;
	bool  Using_Up;
	string Curve_Forward;
	string Curve_Side;
	string Curve_Up;
	bool StandOnAtEnd;
	bool Control_Towards;
	bool AimTarget;

	XChargeData()
	{
		At = 0;
		End = 0;
		Offset = 0;
		Height = 0;
		Velocity = 0;
		Rotation_Speed = 0;
		Using_Curve = false;
		Using_Up = false;
		StandOnAtEnd = true;
		AimTarget = false;
		Control_Towards = false;

		Curve_Forward = "";
		Curve_Side = "";
		Curve_Up = "";
	}
};

struct XJAData : public XBaseData
{
	string Next_Name;
	string Name;
	float At;
	float End;
	float Point;

	XJAData()
	{
		At = 0;
		End = 0;
		Point = 0;

		Next_Name = "";
		Name = "";
	}
};

enum XBeHitState
{
	Hit_Back, Hit_Fly, Hit_Roll, Hit_Freezed, Hit_Free
};

enum XBeHitState_Animation
{
	Hit_Back_Front = 0,
	Hit_Back_Left,
	Hit_Back_Right
};

enum XBeHitPhase
{
	Hit_Present,
	Hit_Landing,
	Hit_Hard,
	Hit_GetUp
};

struct XHitData : public XBaseData
{
	float Time_Present_Straight;
	float Time_Hard_Straight;
	float Offset;
	float Height;
	XBeHitState State;
	XBeHitState_Animation State_Animation;
	float FreezeDuration;
	float Random_Range;
	bool CurveUsing;
	bool Additional_Using_Default;
	float Additional_Hit_Time_Present_Straight;
	float Additional_Hit_Time_Hard_Straight;
	float Additional_Hit_Offset;
	float Additional_Hit_Height;

	XHitData()
	{
		Time_Present_Straight = 0;
		Time_Hard_Straight = 0;
		Offset = 0;
		Height = 0;
		State = Hit_Back;
		State_Animation = Hit_Back_Front;
		Random_Range = 0;
		CurveUsing = false;
		FreezeDuration = 0;
		Additional_Using_Default = true;
		Additional_Hit_Time_Present_Straight = 0;
		Additional_Hit_Time_Hard_Straight = 0;
		Additional_Hit_Offset = 0;
		Additional_Hit_Height = 0;
	}
};

enum XStrickenResponse
{
	XSR_Invincible,
	XSR_Half_Endure,
	XSR_Cease,
	XSR_Full_Endure
};

struct XMobUnitData : public XBaseData
{
	float At;
	int TemplateID;
	bool LifewithinSkill;
	bool Shield;

	float Offset_At_X;
	float Offset_At_Y;
	float Offset_At_Z;

	XMobUnitData()
	{
		At = 0;
		TemplateID = 0;
		LifewithinSkill = false;
		Shield = false;

		Offset_At_X = 0;
		Offset_At_Y = 0;
		Offset_At_Z = 0;
	}
};

struct XCastChain : public XBaseData
{
	float At;
	int TemplateID;
	string Name;

	XCastChain()
	{
		At = 0;
		TemplateID = 0;
		Name = "";
	}
};

struct XQTEData
{
	int QTE;
	float At;
	float End;

	XQTEData()
	{
		QTE = 0;
		At = 0;
		End = 0;
	}
};

struct XLogicalData : public XBaseData
{
	XStrickenResponse StrickenMask;
	int CanReplacedby;    //All
	float Not_Move_At;
	float Not_Move_End;
	float Not_Dash_At;
	float Not_Dash_End;
	float Rotate_At;
	float Rotate_End;
	float Rotate_Speed;
	float CanCancelAt;  //not canceled
	int CanCastAt_QTE;
	bool MoveType;
	bool AttackOnHitDown;
	bool Association;
	string Association_Skill;
	string Syntonic_CoolDown_Skill;
	int PreservedStrength;
	float PreservedAt;
	float PreservedEndAt;
	string Exstring;
	float Exstring_At;

	vector<XQTEData> QTEData;

	XLogicalData()
	{
		StrickenMask = XSR_Cease;
		CanReplacedby = 0;
		Not_Move_At = 0;
		Not_Move_End = 0;
		Not_Dash_At = 0;
		Not_Dash_End = 0;
		Rotate_At = 0;
		Rotate_End = 0;
		Rotate_Speed = 0;
		CanCancelAt = 0;
		CanCastAt_QTE = 0;
		MoveType = false;
		AttackOnHitDown = true;
		Association = false;
		PreservedStrength = 0;
		PreservedAt = 0;
		PreservedEndAt = 0;
		Association_Skill = "";
		Syntonic_CoolDown_Skill = "";
		Exstring = "";
		Exstring_At = 0;
	}
};

struct XManipulationData : public XBaseData
{
	float At;
	float End;
	float OffsetX;
	float OffsetZ;
	float Radius;
	float Force;
	float Degree;
	bool TargetIsOpponent;

	XManipulationData()
	{
		At = 0;
		End = 0;
		OffsetX = 0;
		OffsetZ = 0;
		Radius = 0;
		Force = 0;
		Degree = 0;
		TargetIsOpponent = true;
	}
};

enum XWarningType
{
	Warning_None,
	Warning_Target,
	Warning_Multiple,
	Warning_All
};

struct XWarningData : public XBaseData
{
	XWarningType Type;
	float At;

	float OffsetX;
	float OffsetY;
	float OffsetZ;

	bool Mobs_Inclusived;

	int MaxRandomTarget;
	bool RandomWarningPos;
	float PosRandomRange;
	int PosRandomCount;

	XWarningData()
	{
		Type = Warning_None;
		At = 0;

		OffsetX = 0;
		OffsetY = 0;
		OffsetZ = 0;

		Mobs_Inclusived = false;

		MaxRandomTarget = 0;
		RandomWarningPos = false;
		PosRandomRange = 0;
		PosRandomCount = 0;
	}
};

struct XCombinedData : public XBaseData
{
	string Name;
	float At;
	float End;

	XCombinedData()
	{
		Name = "";
		At = 0;
		End = 0;
	}
};

struct XScriptData: public XBaseData
{
	string Start_Name;
	string Update_Name;
	string Stop_Name;
	string Result_Name;

	XScriptData()
	{
		Start_Name = "";
		Update_Name =  "";
		Stop_Name = "";
		Result_Name = "";
	}
};

class XSkillData
{
	friend class SkillReader;

public:
	inline bool IsDirty(){ return _dirty; }
	inline bool IsNoneReferenced(){ return 0 == _reference; }

	void Release();

public:
	string Prefix;

	string Name;
	int    TypeToken;
	bool   NeedTarget;
	bool   OnceOnly;
	bool   ForCombinedOnly;
	float  CoolDown;
	float  Time;
	bool   Cast_Range_Rect;
	float  Cast_Offset_X;
	float  Cast_Offset_Z;
	float  Cast_Range_Upper;
	float  Cast_Range_Lower;
	float  Cast_Scope;
	float  Cast_Scope_Shift;
	bool IgnoreCollision;
	bool MultipleAttackSupported;
	float BackTowardsDecline;
	string PVP_Script_Name;

	vector<XResultData> Result;
	vector<XChargeData> Charge;
	vector<XJAData> Ja;
	vector<XHitData> Hit;
	vector<XManipulationData> Manipulation;
	vector<XWarningData> Warning;
	vector<XMobUnitData> Mobs;
	vector<XCombinedData> Combined;

	XLogicalData Logical;
	XScriptData Script;
	XCastChain Chain;
	
	XSkillData()
		:_dirty(false),
		 _reference(0)
	{
		Prefix = "";

		Name = "";
		PVP_Script_Name = "";
		TypeToken = 1;
		NeedTarget = true;
		OnceOnly = false;
		ForCombinedOnly = false;
		CoolDown = 1;
		Time = 0;
		Cast_Range_Rect = false;
		Cast_Offset_X = 0;
		Cast_Offset_Z = 0;
		Cast_Range_Upper = 0;
		Cast_Range_Lower = 0;
		Cast_Scope = 0;
		Cast_Scope_Shift = 0;
		MultipleAttackSupported = false;
		BackTowardsDecline = 0.75f;
		IgnoreCollision = false;
	}

private:
	inline void Referenced(){ ++_reference; }
	inline void Dirty(){ _dirty = true; }

private:
	volatile bool _dirty;
	volatile int _reference;
};

class SkillReader
{
public:
	static XSkillData *LoadSkill(const char *fileName);
	static void HotPatchSkill(const char *fileName);
};

#endif // __SKILLREADER_H__