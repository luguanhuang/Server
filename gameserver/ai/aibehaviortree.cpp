#include "pch.h"
#include "aibehaviortree.h"
#include "aitreemgr.h"
#include "aionlinedebugmgr.h"

AIBehaviorTree::AIBehaviorTree()
{
	m_Root = NULL;
	m_Host = NULL;
	m_TickInterval = 1.0f;
	m_TreeName = "";
}

AIBehaviorTree::~AIBehaviorTree()
{
	if (m_TreeName != "")
		AITreeMgr::Instance()->DecTreeRef(m_TreeName, m_Root);
}

void AIBehaviorTree::Tick()
{
	if (m_Root && m_Host)
	{
		AITREESTART(m_TreeName.c_str());
		m_Root->Update(m_Host);
		AITREEEND(m_TreeName.c_str());
	}

#ifdef AIDEBUG
	AIDebugHelper helper(m_Root, m_Host);
	helper.TryReport();
#endif
}

SharedData& AIBehaviorTree::Data()
{
	return m_myData;
}

bool AIBehaviorTree::Init(const std::string& aiTreeName )
{
	RootNode* tmpRoot = AITreeMgr::Instance()->GetAITreeByName(aiTreeName);
	if(!tmpRoot)
	{
		SSError << "not find aitree name = " + aiTreeName << END;
		return false;
	}

	SharedData* data = tmpRoot->GetSharedData();
	if(!data)
	{
		SSError << "aitree data null name = " + aiTreeName << END;
		return false;
	}
	m_Root = tmpRoot;
	//m_myData = (*data);//数据初始化
	CopyData(data);
	m_TreeName = aiTreeName;

	AITreeMgr::Instance()->IncTreeRef(aiTreeName, m_Root);

	return true;
}

void AIBehaviorTree::CopyData(SharedData* data)
{
	for (UINT32 i=0; i<MAX_COUNT_NUM; i++)
	{
		m_myData.SetBoolByIndex(i, data->GetBoolByIndex(i));
		m_myData.SetFloatByIndex(i, data->GetFloatByIndex(i));
		m_myData.SetIntByIndex(i, data->GetIntByIndex(i));
		m_myData.SetTransformByIndex(i, data->GetTransformByIndex(i));
		///> 为了效率优化，移出目前没有初始值的类型的拷贝
		//m_myData.SetStringByIndex(i, data->GetStringByIndex(i));
	}
}

void AIBehaviorTree::Print()
{
	if(m_Root)
	{
		m_myData.Print();
		m_Root->PrintTree();
	}
}



