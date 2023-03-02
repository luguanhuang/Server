#ifndef __SYSNODE_H__
#define __SYSNODE_H__
#include "ainodeaction.h"
#include "ainodecondition.h"

class RandomFloat : public AINodeAction
{
public:
	RandomFloat(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	float GetMinValue(Unit* unit);
	float GetMaxValue(Unit* unit);

	std::string m_sharedMinName;
	UINT32 m_MinIndex;
	float m_minValue;

	std::string m_sharedMaxName;
	UINT32 m_MaxIndex;
	float m_maxValue;

	bool m_inclusive;
	
	std::string m_sharedStoredResultName;
	UINT32 m_ResultIndex;
};

enum FloatOperatorType
{
	FOTYPE_ADD = 0,
	FOTYPE_SUBTRACT = 1,
	FOTYPE_MULTIPLY = 2,
	FOTYPE_DIVIDE = 3,
	FOTYPE_MIN = 4,
	FOTYPE_MAX = 5,
};
class FloatOperator : public AINodeAction
{
public:
	FloatOperator(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	float GetFloat1(Unit* unit);
	float GetFloat2(Unit* unit);

	FloatOperatorType m_type;

	std::string m_sharedFloat1Name;
	UINT32 m_FloatIndex1;
	float m_float1Value;

	std::string m_sharedFloat2Name;
	UINT32 m_FloatIndex2;
	float m_float2Value;

	std::string m_sharedStoredResultName;
	UINT32 m_ResultIndex;
	
};

class IntOperator : public AINodeAction
{
public:
	IntOperator(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int GetInt1(SharedData* data);
	int GetInt2(SharedData* data);

	FloatOperatorType m_type;

	std::string m_sharedInt1Name;
	UINT32 m_IntIndex1;
	int m_int1Value;

	std::string m_sharedInt2Name;
	UINT32 m_IntIndex2;
	int m_int2Value;

	std::string m_sharedStoredResultName;
	UINT32 m_StoreValue;
};

enum ComparisonType
{
	FCTYPE_LESS_THAN = 0,
	FCTYPE_LESS_OR_EQUAL_TO = 1,
	FCTYPE_EQUAL_TO = 2,
	FCTYPE_NOT_EQUAL_TO = 3,
	FCTYPE_GREATER_THAN_OR_EQUAL_TO = 4,
	FCTYPE_GREATER_THAN = 5,
};

class FloatComparison :public AINodeCondition
{
public:
	FloatComparison(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);
	

private:
	float GetFloat1(SharedData* data);
	float GetFloat2(SharedData* data);

	ComparisonType m_type;

	std::string m_sharedFloat1Name;
	UINT32 m_floatIndex1;
	float m_float1Value;

	std::string m_sharedFloat2Name;
	float m_float2Value;
	UINT32 m_floatIndex2;
};

class IntComparison :public AINodeCondition
{
public:
	IntComparison(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	int GetInt1(SharedData* data);
	int GetInt2(SharedData* data);

	ComparisonType m_type;

	std::string m_sharedInt1Name;
	UINT32 m_intIndex1;
	int m_int1Value;

	std::string m_sharedInt2Name;
	UINT32 m_intIndex2;
	int m_int2Value;

};

class SetFloat : public AINodeAction
{
public:
	SetFloat(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	float GetValue(SharedData* data);

	std::string m_sharedValueName;
	UINT32 m_valueIndex;
	float m_value;

	std::string m_sharedStoredResultName;
	UINT32 m_resultIndex;
};

class Log : public AINodeAction
{
public:
	Log(const tinyxml2::XMLElement* elem){};
};

class SetBool : public AINodeAction
{
public:
	SetBool(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string m_sharedValueName;
	UINT32 m_valueIndex;
	bool m_value;

	std::string m_sharedStoredResultName;
	UINT32 m_resultIndex;
};

class BoolComparison : public AINodeCondition
{
public:
	BoolComparison(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	
	std::string m_sharedBool1Name;
	UINT32 m_boolIndex1;
	bool m_bool1Value;

	std::string m_sharedBool2Name;
	UINT32 m_boolIndex2;
	bool m_bool2Value;
};

class CompareTo : public AINodeAction
{
public:
	CompareTo(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* uint);

private:

	std::string m_sharedString1Name;
	UINT32 m_stringIndex1;
	std::string m_string1Value;

	std::string m_sharedString2Name;
	UINT32 m_stringIndex2;
	std::string m_string2Value;

	std::string m_sharedResultName;
	UINT32 m_resultIndex;
};

class SetInt: public AINodeAction
{
public:
	SetInt(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string m_sharedValueName;
	UINT32 m_intIndex;
	int m_value;

	std::string m_sharedStoredResultName;
	UINT32 m_resultIndex;
};

class GetRealtimeSinceStartup : public AINodeAction
{
public:
	GetRealtimeSinceStartup(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string m_sharedValueName;
	int m_nameIndex;
};

#endif