#include "pch.h"
#include "sceneinherit.h"
#include "unit/role.h"
#include "guild/guildinherit.h"
#include "scene/scene.h"

void SceneInherit::OnEnterSceneFinally(Scene *pScene, Role* role)
{
	if (role->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
	{
		CGuildInherit::Instance()->Add(role);
	}
	else
	{
		CGuildInherit::Instance()->Add(role, CGuildInherit::TEACHER_INHERIT);
	}
}

void SceneInherit::OnLeaveScene(Scene *pScene, Role* role)
{
	CGuildInherit::Instance()->DoLeaveScene(role->GetID());
}