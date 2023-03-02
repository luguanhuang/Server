#ifndef __AINODEMOVE_H__
#define __AINODEMOVE_H__

#include "ainode.h"

#include "ainodeaction.h"
#include "ainodecondition.h"

class ActionMove : public AINodeAction
{
public:
	ActionMove(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	std::string m_sharedMoveDir;
	std::string m_sharedMoveDest;
	std::string m_sharedMoveSpeed;
	UINT32 m_moveDirIndex;
	UINT32 m_moveDestIndex;
	UINT32 m_moveSpeedIndex;
};

class SetDest : public AINodeAction
{
	enum SetDestWay
	{
		Target = 1,
		BornPos = 2,
		NavPos = 3,
	};

	enum AdjustDirection
	{
		TargetDir = 1,
		TargetFace = 2,
		SelfFace = 3,
	};

public:
	SetDest(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string m_sharedFinalDest;
	std::string m_sharedTarget;
	std::string m_sharedNav;
	std::string m_sharedBornPos;
	std::string m_sharedTickCount;
	float m_RandomMax;
	float m_AdjustAngle;
	std::string m_sharedAdjustLengthName;
	float m_sharedAdjustLength;
	int m_AdjustDir;
	int m_AdjustSetDestType;
	Vector3 m_BornPos;

	UINT32 m_finalDestIndex;
	UINT32 m_targetIndex;
	UINT32 m_navIndex;
	UINT32 m_bornPosIndex;
	UINT32 m_tickCountIndex;
	UINT32 m_adjustLengthIndex;

	//Vector3 mFinalDest;
	//Transform mTarget;
	//Transform mNav;
	//Vector3 mBornPos;
	//int mTickCount;
	//float mRandomMax;
	//float mAdjustAngle;
	//float mAdjustLength;
	//Vector3 mAdjustDir;
	//int mAdjustSetDestType;

};


class NavToTarget:public AINodeAction
{
public:
	NavToTarget(const tinyxml2::XMLElement *elem);

	virtual bool Update(Unit* unit);
	virtual void Print();
private:
	bool _Do(Unit* pUnit, const Vector3 &des);

	std::string m_sharedTargetName;
	UINT32 m_targetIndex;
	std::string m_sharedNavTargetName;
	UINT32 m_navTargetIndex;
	std::string m_sharedNavPosName;
	UINT32 m_navPosIndex;
	Vector3 m_navPos;
};

class ActionRotate : public AINodeAction
{
public:
	ActionRotate(const tinyxml2::XMLElement *elem);

	virtual bool Update(Unit* unit);

private:
	float m_RotDegree;
	float m_RotSpeed;
	int m_RotType;
};

class RotateToTarget : public AINodeAction
{
public:
	RotateToTarget(const tinyxml2::XMLElement *elem){};

	virtual bool Update(Unit* unit);
};

class ConditionCanReach : public AINodeCondition
{
public:
	ConditionCanReach(const tinyxml2::XMLElement *elem);

	virtual bool Update(Unit* unit);

private:
	int m_TemplateId;
	int m_TemplateIdIndex;
	Vector3 m_DestPos;
	int m_DestPosIndex;
};

class Navigation : public AINodeAction
{
public:
	Navigation(const tinyxml2::XMLElement *elem);
	virtual bool Update(Unit* unit);

private:
	int m_MoveDirOld;

	int m_MoveDirIndex;
	std::string m_MoveDirName;

	float m_PatrolPointRadius;
	bool m_IsGoForward;
};

class MoveStratage : public AINodeAction
{
public:
	MoveStratage(const tinyxml2::XMLElement *elem);
	virtual bool Update(Unit* unit);

private:
	int TargetIndex;
	int DestIndex;
	int Stratage;
};

#endif