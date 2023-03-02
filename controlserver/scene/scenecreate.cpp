#include "pch.h"
#include "scenecreate.h"
#include "sceneconfig.h"
#include "config.h"
#include "network/gslink.h"
//#include "scene/rpcm2g_createbattle.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"

INSTANCE_SINGLETON(CSceneCreator)


CSceneCreator::CSceneCreator()
:m_qwUID(0)
{
}

CSceneCreator::~CSceneCreator()
{
}

bool CSceneCreator::Init()
{
	return true;
}

void CSceneCreator::Uninit()
{
}

bool CSceneCreator::CreateBattleScene(UINT32 dwSceneTemplateID, ICreateBattleListener* poListener, const KKSG::CreateBattleParam& roParam)
{
	const SceneConf *pConf = CSceneConfig::Instance()->GetSceneConf(dwSceneTemplateID);
	if(pConf == NULL)
	{
		LogError("Invalid scene template id [%u]", dwSceneTemplateID);
		return false;
	}

	UINT32 dwDestLine = CGsManager::Instance()->GetMinimalLoadGsLine();
	if(dwDestLine == INVALID_LINE_ID)
	{
		LogError("Can't find the minimal load gs line");
		return false;
	}

	UINT64 qwNewUID = NewID();
	m_oMap[qwNewUID] = poListener;

	UINT32 dwNewSceneUID = CSceneManager::Instance()->NewID();
	/*RpcM2G_CreateBattle* rpc = RpcM2G_CreateBattle::CreateRpc();
	rpc->m_oArg.set_uid(qwNewUID);
	rpc->m_oArg.set_rpcid(0);
	rpc->m_oArg.set_mapid(dwSceneTemplateID);
	rpc->m_oArg.set_sceneid(dwNewSceneUID);
	rpc->m_oArg.set_line(dwDestLine);
	rpc->m_oArg.mutable_param()->CopyFrom(roParam);
	GSLink::Instance()->SendToLine(dwDestLine, *rpc);*/
	return true;
}

void CSceneCreator::OnCreateBattleReply(UINT64 qwUid, CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	CListenerMap::iterator it = m_oMap.find(qwUid);
	if(it == m_oMap.end())
	{
		return;
	}

	if(it->second != NULL)
	{
		it->second->OnCreateReply(scene, roParam, res);
	}
	m_oMap.erase(it);
}

UINT64 CSceneCreator::NewID()
{
	++m_qwUID;
	if(m_qwUID == 0)
	{
		++m_qwUID;
	}

	return m_qwUID;
}