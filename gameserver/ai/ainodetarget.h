#ifndef __AINODETARGET_H__
#define __AINODETARGET_H__

#include "ainode.h"
#include "ainodeaction.h"
#include "util/XCommon.h"
#include "ainodefactory.h"

class ResetTargets : public AINodeAction
{
public:
	ResetTargets(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class FindTargetByDistance:public AINodeAction
{
public:
	FindTargetByDistance(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	std::string m_sharedDistanceName;
	UINT32 m_distanceIndex;
	float m_distance;
	bool m_filterImmortal;
	float m_angle;
	float m_delta;
	int m_targetType;  // 0: all 1: player  2: enemy
};

class FindTargetByHitLevel:public AINodeAction
{
public:
	FindTargetByHitLevel(const tinyxml2::XMLElement *elem);

	virtual bool Update(Unit* unit);

private:
	bool m_filterImmortal;
};

class TargetByHatredList:public AINodeAction
{
public:
	TargetByHatredList(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	std::string m_sharedDistanceName;
	bool m_filterImmortal;
};

class DoSelectByUID:public AINodeAction
{
public:
	DoSelectByUID(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	int m_UIDIndex;
	string m_UIDName;
};

class DoSelectRoleByIndex:public AINodeAction
{
public:
	DoSelectRoleByIndex(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	int m_IndexIndex;
	int m_Index;
};

class SelectNonHartedList : public AINodeAction
{
public:
	SelectNonHartedList(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};

class FindNavPath : public AINodeAction
{
public:
	FindNavPath(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};

class DoSelectNearest : public AINodeAction
{
public:
	DoSelectNearest(const tinyxml2::XMLElement* elem){m_targetDisIndex = AINodeFactory::CreateIndexByName("target_distance");};
	virtual bool Update(Unit* unit);

public:
	UINT32 m_targetDisIndex;
};

class DoSelectFarthest : public AINodeAction
{
public:
	DoSelectFarthest(const tinyxml2::XMLElement* elem){m_targetDisIndex = AINodeFactory::CreateIndexByName("target_distance");};
	virtual bool Update(Unit* unit);

public:
	UINT32 m_targetDisIndex;
};

class DoSelectRandomTarget : public AINodeAction
{
public:
	DoSelectRandomTarget(const tinyxml2::XMLElement* elem) { }
	virtual bool Update(Unit* unit);
};

class DetectEnimyInSight:public AINodeAction
{
public:
	DetectEnimyInSight(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class SelectMoveTargetById : public AINodeAction
{
public:
	SelectMoveTargetById(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string MoveTarget;
	UINT32 TargetIndex;
	int ObjectId;
};

class SelectBuffTarget : public AINodeAction
{
public:
	SelectBuffTarget(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
private:
	std::string BuffTarget;
	UINT32 BuffTargetIndex;
};

class SelectItemTarget : public AINodeAction
{
public:
	SelectItemTarget(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
private:
	std::string ItemTarget;
	UINT32 ItemTargetIndex;
};

class SelectTargetBySkillCircle : public AINodeAction
{
public:
	SelectTargetBySkillCircle(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit){return true;};
};

class ResetHartedList : public AINodeAction
{
public: 
	ResetHartedList(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};


class CalDistance : public AINodeAction
{
public:
	CalDistance(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	Transform TargetObject;
	int TargetIndex;
	float Distance;
	int DistanceIndex;
	Vector3 DestPoint;
	int DestPointIndex;

};

#endif