#include "pch.h"
#include "scene/rpcc2g_enterleisurescene.h"
#include "scene/scenemanager.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/sceneleisureconfig.h"
#include "scene/sceneconfig.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/10/17 11:08:28

RPC_SERVER_IMPLEMETION(RpcC2G_EnterLeisureScene, EnterLeisureSceneArg, EnterLeisureSceneRes)

void RpcC2G_EnterLeisureScene::OnCall(const EnterLeisureSceneArg &roArg, EnterLeisureSceneRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Scene* scene = role->GetCurrScene();
	UINT32 currentsceneid = (NULL == scene) ? 0 : scene->GetSceneTemplateID();
	UINT32 sceneid = SceneLeisureConfig::Instance()->GetSceneId(roArg.index(), currentsceneid);
	if ((UINT32(-1) == sceneid))
	{
		SSWarn<<"role:"<<role->GetID()<<" request index:"<<roArg.index()<<" invalid sceneid:"<<sceneid<<END;
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	if (0 == sceneid)
	{
		SSWarn<<"role:"<<role->GetID()<<" request index:"<<roArg.index()<<" invalid sceneid:"<<sceneid<<END;
		roRes.set_errorcode(KKSG::ERR_CHANGE_SAMESCENE);
		return;
	}

	SceneInfo* sceneinfo = SceneConfig::Instance()->FindSceneInfo(sceneid);
	if (NULL == sceneinfo)
	{
		SSWarn<<"role:"<<role->GetID()<<" request index:"<<roArg.index()<<" invalid sceneid:"<<sceneid<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	SceneManager::Instance()->ChangeScene(role, sceneid);

	SSInfo<<"role:"<<role->GetID()<<" request index:"<<roArg.index()<<" sceneid:"<<sceneid<<END;
}

void RpcC2G_EnterLeisureScene::OnDelayReplyRpc(const EnterLeisureSceneArg &roArg, EnterLeisureSceneRes &roRes, const CUserData &roUserData)
{
}
