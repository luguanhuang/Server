#include "pch.h"
#include <typeinfo>
#include "ainode.h"

#include "ainodelogic.h"
#include "aionlinedebugmgr.h"
#include "util/XRandom.h"

void LogicNode::AddChild( AINodeBase* child )
{
	m_list.push_back(child);
}

LogicNode::~LogicNode()
{
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		delete (*it);
	}
	m_list.clear();
}

void LogicNode::Print()
{
	AINodeBase::Print();
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		AINodeBase* node = (*it);
		if(node)
		{
			node->Print();
		}
	}
}

void RandomIndex::Rand()
{
	for (int i = m_IndexList.size() - 1; i > 0; --i)
	{
		int j = XRandom::randInt(0, i + 1);
		if (j == i)
			continue;
		int temp = m_IndexList[j];
		m_IndexList[j] = m_IndexList[i];
		m_IndexList[i] = temp;
	}
}

int RandomIndex::operator[](int index)
{
	if (index < 0 || index >= m_IndexList.size())
	{
		SSError << "Random index out of range, try get " << index << " but length = " << m_IndexList.size() << END;
	}

	return m_IndexList[index];
}

bool SelectorNode::Update(Unit* unit)
{
	AINODESTART("SelectorNode");
	
	DEFINE_AI_DEBUG_HELPER(this, unit);
#ifdef AIDEBUG
	int i = 0;
#endif
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		AINodeBase* base = (*it);
		if(base)
		{
			if(base->Update(unit))
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << i << "] " <<base->GetFriendlyName() << " Success; ");

				AINODEEND("SelectorNode");
				AIRETURN("SelectorNode", true);
			}
		}
#ifdef AIDEBUG
		++i;
#endif
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Failed; ");

	AIRETURN("SelectorNode", false);
}

bool SequenceNode::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

#ifdef AIDEBUG
	int i = 0;
#endif
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
	{
		AINodeBase* base = (*it);
		if(base)
		{
			if(!base->Update(unit))
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << i << "] " << base->GetFriendlyName() << " Failed; ");

				AIRETURN("SequenceNode", false);
			}
		}
#ifdef AIDEBUG
		++i;
#endif
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Success; ");

	AIRETURN("SequenceNode", true);
}

bool Inverter::Update(Unit* unit)
{
	if(m_childNode)
	{
		AI_DEBUG_RETURN("Inverter", !m_childNode->Update(unit));
	}

	AI_DEBUG_RETURN("Inverter", false);
}

DecorationNode::~DecorationNode()
{
	delete m_childNode;
	m_childNode = NULL;
}

void DecorationNode::Print()
{
	AINodeBase::Print();
	if(m_childNode)
	{
		m_childNode->Print();
	}
}

void DecorationNode::AddChild( AINodeBase* child )
{
	m_childNode = child;
}




bool ReturnSuccess::Update( Unit* unit )
{
	if(m_childNode)
	{
		m_childNode->Update(unit);
	}
	AI_DEBUG_RETURN("ReturnSuccess", true);
}

bool ReturnFailure::Update( Unit* unit )
{
	if(m_childNode)
	{
		m_childNode->Update(unit);
	}
	AI_DEBUG_RETURN("ReturnFailure", false);
}

ConditionalEvaluator::ConditionalEvaluator( const tinyxml2::XMLElement* elem )
{
	m_conditionNode = NULL;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("ConditionalTask");
		if(attr)
		{
			m_conditionNodeName = attr->Value();
		}
	}
}

ConditionalEvaluator::~ConditionalEvaluator()
{
	delete m_conditionNode;
	m_conditionNode = NULL;
}

bool ConditionalEvaluator::Update( Unit* unit )
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	if(m_conditionNode)
	{
		if(!m_conditionNode->Update(unit))
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Condition Failed;");

			AIRETURN("ConditionalEvaluator", false);
		}
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Condition, Failed;");

		AIRETURN("ConditionalEvaluator", false);
	}

	if(m_childNode)
	{
		bool bRes = m_childNode->Update(unit);

		AI_DEBUG_STATEMENT(m_DebugInfo << (bRes ? "Success; " : "Failed;"));

		AIRETURN("ConditionalEvaluator", bRes);
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Failed;");

	AIRETURN("ConditionalEvaluator", false);
}

std::string ConditionalEvaluator::GetConditionNodeName()
{
	return m_conditionNodeName;
}

bool ConditionalEvaluator::AddConditionNode(AINodeBase * conditionNode)
{
	if(m_conditionNode)
	{
		SSError << "add condition node repeat = " << m_conditionNodeName << END;
		return false;
	}

	m_conditionNode = conditionNode;

	return true;
}

void RandomSelectorNode::AddChild(AINodeBase* child)
{
	LogicNode::AddChild(child);
	m_RandomIndex.Append();
}

bool RandomSelectorNode::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	m_RandomIndex.Rand();
	for(int i = 0; i < m_list.size(); ++i)
	{
		int curIndex = m_RandomIndex[i];
		AI_DEBUG_STATEMENT(m_DebugInfo << curIndex << " ")
		AINodeBase* base = m_list[curIndex];
		if(base)
		{
			if(base->Update(unit))
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << curIndex << "] " <<base->GetFriendlyName() << " Success; ");

				AIRETURN("RandomSelectorNode", true);
			}
		}
	}

	AI_DEBUG_RETURN("RandomSelectorNode", true);
}

void RandomSequenceNode::AddChild(AINodeBase* child)
{
	LogicNode::AddChild(child);
	m_RandomIndex.Append();
}

bool RandomSequenceNode::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	m_RandomIndex.Rand();
	for(int i = 0; i < m_list.size(); ++i)
	{
		int curIndex = m_RandomIndex[i];
		AI_DEBUG_STATEMENT(m_DebugInfo << curIndex << " ")
		AINodeBase* base = m_list[curIndex];
		if(base)
		{
			if(!base->Update(unit))
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << curIndex << "] " << base->GetFriendlyName() << " Failed; ");

				AIRETURN("RandomSequenceNode", false);
			}
		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("RandomSequenceNode", true);
}
