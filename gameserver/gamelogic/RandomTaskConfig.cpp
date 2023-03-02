#include "pch.h"
#include "RandomTaskConfig.h"
#include "RandomTaskCondition.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "foreach.h"
#include "stagemgr.h"



INSTANCE_SINGLETON(RandomTaskConfig)

RandomTaskConfig::RandomTaskConfig()
{

}

RandomTaskConfig::~RandomTaskConfig()
{

}

bool RandomTaskConfig::Init()
{
	if (!m_oTable.LoadFile("table/randomtask.txt"))
	{
		LogWarn("Load file table/randomtask.txt failed!");
		return false;
	}

	for (size_t i = 0; i < m_oTable.Table.size(); ++i)
	{
		m_oTaskPool[m_oTable.Table[i]->TaskType].push_back(m_oTable.Table[i]->TaskID);
	}

	m_oTaskCondition[1] = new RandomTaskStarCountGreater;   // 副本3星
	m_oTaskCondition[2] = new RandomTaskComboCountGreater;  // Combo数量达到多少
	m_oTaskCondition[3] = new RandomTaskBeHitLess;          // 被击中次数少于
	m_oTaskCondition[4] = new RandomTaskPassTimeLess;       // 通关耗时少于
	m_oTaskCondition[5] = new RandomTaskTeamDamageGreater;  // 队伍伤害大于
	m_oTaskCondition[6] = new RandomTaskTeamBeHitLess;      // 队伍被击中次数少于
	m_oTaskCondition[7] = new RandomTaskOpenChestCount;     // 打开箱子数量大于等于
	return true;
}

void RandomTaskConfig::Uninit()
{
	foreach(i in m_oTaskCondition)
	{
		delete i->second;
	}

	m_oTaskCondition.clear();
}

//bool RandomTaskConfig::CheckTaskCondition(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
//{
//	auto it = m_oTaskCondition.find(pConf->TaskCondition);
//	if (it == m_oTaskCondition.end())
//	{
//		return false;
//	}
//
//	return it->second->IsTaskComplete(pConf, pRole, pScene);
//}
//
//RandomTaskTable::RowData * RandomTaskConfig::GetConf(int taskID)
//{
//	return m_oTable.GetByTaskID(taskID);
//}
//
//void RandomTaskConfig::GiveRandomTask(Role *pRole, Scene *pScene)
//{
//	TaskStatus *pTaskStatus = pRole->Get<TaskStatus>();
//	int taskcount = pTaskStatus->GetTodayActiveRandomTask();
//	if (taskcount >= 10)
//	{
//		return;
//	}
//
//	auto *pSceneConf = pScene->GetSceneInfo()->m_pConf;
//	if (pSceneConf->RandomTaskSpecify.empty())
//	{
//		return;
//	}
//
//	// 第一次进这个场景必然接到任务
//	if (pRole->Get<StageMgr>()->GetRank(pSceneConf->id) <= 0)
//	{
//		int n = XRandom::randInt(0, pSceneConf->RandomTaskSpecify.size());
//		pTaskStatus->SetRandomTask(pSceneConf->RandomTaskSpecify[n]);
//		pTaskStatus->SetTodayActiveRandomTask(taskcount+1);
//		return;
//	}
//
//	int randVal = (int)XRandom::randInt(0, 100);
//	int Prob =  GetGlobalConfig().GetInt("RandomTaskProb", 10);
//	if (randVal < Prob)
//	{
//		int n = XRandom::randInt(0, pSceneConf->RandomTaskSpecify.size());
//		pTaskStatus->SetRandomTask(pSceneConf->RandomTaskSpecify[n]);
//		pTaskStatus->SetTodayActiveRandomTask(taskcount+1);
//		return;
//	}
//}
//
