#include "pch.h"
#include "achievementhandler.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "AchivementDef.h"
#include "rolefashion.h"

AChievementHandler AChievementHandler::GlobalHandler;

void AChievementHandler::OnLevelup(Role *poRole)
{
	// 完成成就
	AchivementLevelupEvent event(poRole, poRole->GetLevel());
	event.Emit();
}

void AChievementHandler::OnSetProfession(Role *poRole)
{
	AchivementPromoteEvent event(poRole);
	event.Emit();
}

void AChievementHandler::OnCompleteScene(Role *poRole, Scene *poScene)
{
	// 完成关卡成就
	AchivementStageCompleteEvent event(poRole, poScene->GetSceneType(), poScene->GetSceneTemplateID(), poScene->GetRoleRank(poRole->GetID()));
	event.Emit();
}

void AChievementHandler::OnSweepScene(Role *poRole, SceneInfo *pSceneInfo)
{
	AchivementStageCompleteEvent event(poRole, pSceneInfo->m_pConf->type, pSceneInfo->m_pConf->id, 3);
	event.Emit();
}

void AChievementHandler::OnChangeAttr(Role* pRole)
{
	AchivementPowerPointEvent event(pRole, pRole->GetAttr(TOTAL_POWERPOINT));
	event.Emit();
}

void AChievementHandler::OnLogin(Role* pRole)
{
	AchivementLoginEvent event(pRole);
	event.Emit();
}

void AChievementHandler::OnAddFashion(Role *pRole)
{
	AchivementFashionEvent event(pRole, pRole->Get<RoleFashion>()->GetFashionNum());
	event.Emit();
}
