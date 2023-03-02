#include "pch.h"
#include "RandomTaskCondition.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenestatistics.h"

bool RandomTaskStarCountGreater::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return pScene->GetRoleRank(pRole->GetID()) >= pConf->TaskParam;
	return false;
}

bool RandomTaskComboCountGreater::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return pRole->GetStatistics().GetMaxCombo() >= pConf->TaskParam;
}

bool RandomTaskBeHitLess::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return pRole->GetStatistics().GetBehit() < pConf->TaskParam;
}

bool RandomTaskPassTimeLess::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return (pScene->CostTime() != 0) && pScene->CostTime() < pConf->TaskParam;
}

bool RandomTaskTeamDamageGreater::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return NULL != pScene->Statistics() ? pScene->Statistics()->m_TotalDamage >= pConf->TaskParam : false; 
}

bool RandomTaskTeamBeHitLess::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return NULL != pScene->Statistics() ? pScene->Statistics()->m_TotalBehit < pConf->TaskParam : false;
}

bool RandomTaskOpenChestCount::IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene)
{
	return pRole->Statistics().GetOpenChestCount() >= pConf->TaskParam;
}
