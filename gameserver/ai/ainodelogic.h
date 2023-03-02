#ifndef __AINODELOGIC_H__
#define __AINODELOGIC_H__

#include "ainode.h"

class LogicNode:public AINodeBase 
{
public:
	LogicNode(){m_type = NODE_TYPE_LOGIC;}
	virtual ~LogicNode();
	virtual void AddChild(AINodeBase* child);

	virtual void Print();

protected:
	std::vector<AINodeBase*> m_list;
};

class RandomIndex
{
public:
	void Rand();
	void Append(){ m_IndexList.push_back(m_IndexList.size());}
	int operator[] (int index);
protected:
	std::vector<int> m_IndexList;
};

class SelectorNode:public LogicNode
{
public:
	virtual bool Update(Unit* unit);
};

class SequenceNode:public LogicNode
{
public:
	virtual bool Update(Unit* unit);
};

class RandomSelectorNode:public LogicNode
{
public:
	virtual void AddChild(AINodeBase* child);
	virtual bool Update(Unit* unit);
protected:
	RandomIndex m_RandomIndex;
};

class RandomSequenceNode:public LogicNode
{
public:
	virtual void AddChild(AINodeBase* child);
	virtual bool Update(Unit* unit);
protected:
	RandomIndex m_RandomIndex;
};

class DecorationNode : public AINodeBase
{
public:
	DecorationNode():m_childNode(NULL) 
	{ 
		m_type = NODE_TYPE_DECORATION;
	}
	virtual ~DecorationNode();
	virtual void AddChild(AINodeBase* child);
	virtual void Print();

protected:
	AINodeBase* m_childNode;
};

class Inverter : public DecorationNode
{
public:
	virtual bool Update(Unit* unit);
	
};

class ReturnSuccess:public DecorationNode
{
public:
	virtual bool Update(Unit* unit);
};

class ReturnFailure:public DecorationNode
{
public:
	virtual bool Update(Unit* unit);
};

class ConditionalEvaluator : public DecorationNode
{
public:
	ConditionalEvaluator(const tinyxml2::XMLElement* elem );
	virtual ~ConditionalEvaluator();
	virtual bool Update(Unit* unit);

	std::string GetConditionNodeName();
	bool AddConditionNode(AINodeBase * conditionNode);


private:

	std::string m_conditionNodeName;

	AINodeBase* m_conditionNode;
};

#endif