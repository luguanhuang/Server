#ifndef _AI_BEHAVIOR_TREE_
#define _AI_BEHAVIOR_TREE_

#include "ainode.h"

class AIBehaviorTree
{
	float m_TickInterval;
	RootNode* m_Root;
	Unit* m_Host;
	std::string m_TreeName;
public:
	AIBehaviorTree();
	~AIBehaviorTree();

public:
	void Tick();

	SharedData& Data();

	bool Init(const std::string& aiTreeName);//对AITree 进行绑定
	void Print();//输出树的信息
	void SetHost(Unit* unit) {m_Host = unit;};
	std::string& GetTreeName() {return m_TreeName;}
	void CopyData(SharedData* data);

private:

	SharedData m_myData;//

};

#endif

