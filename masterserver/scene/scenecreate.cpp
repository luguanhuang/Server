#include "pch.h"
#include "scenecreate.h"
#include "sceneconfig.h"
#include "config.h"
#include "network/gslink.h"
#include "scene/rpcm2n_createbattlescene.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "gmf/guildmatchfightmgr.h"
#include "skycity/skycitymgr.h"
#include "reswar/reswarmgr.h"
#include "loghelper/tlogr.h"
#include "network/controllink.h"
#include "invfight/invfightmgr.h"
#include "herobattle/herobattlemgr.h"
#include "guildcastle/guildcastlebase.h"
#include "guildcastle/guildcastlemgr.h"
#include "pk/pkmgr.h"
#include "mobabattle/mobabattlemgr.h"
#include "battlefield/battlefieldmgr.h"

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

bool CSceneCreator::CreateBattleScene(UINT32 dwSceneTemplateID, ICreateBattleListener* poListener, const KKSG::CreateBattleParam& roParam, UINT32 destline, UINT32 createType)
{
	const SceneConf *pConf = CSceneConfig::Instance()->GetSceneConf(dwSceneTemplateID);
	if(pConf == NULL)
	{
		LogError("Invalid scene template id [%u]", dwSceneTemplateID);
		return false;
	}

	UINT64 qwNewUID = NewID();
	m_oMap[qwNewUID] = poListener;

	RpcM2N_CreateBattleScene* rpc = RpcM2N_CreateBattleScene::CreateRpc();
	rpc->m_oArg.set_uid(qwNewUID);
	rpc->m_oArg.set_mapid(dwSceneTemplateID);
	rpc->m_oArg.set_destline(destline);
	rpc->m_oArg.set_createtype(createType);
	rpc->m_oArg.mutable_param()->CopyFrom(roParam);
	ControlLink::Instance()->SendTo(*rpc);

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

void ICreateGMFBattleListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if(0 != nErrCode)
	{
		SSError << " on create gmf battle err = " << nErrCode << END;
		return ;
	}
	if(scene && roParam.has_gmfpara())
	{
		const auto& data  = roParam.gmfpara().roomdata();
		GuildMatchFightMgr::Instance()->CreateSceneEnd(data.uid(), scene->GetID());
	}
}

ICreateGMFBattleListener ICreateGMFBattleListener::GlobalCreateGMFBattleListener;





void ICreateSkyCityListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if(0 != nErrCode)
	{
		SSError << " on create skycity battle err = " << nErrCode << END;
		return ;
	}
	if(scene)
	{
		const auto& data  = roParam.skycity();
		LogDebug("ICreateSkyCityListener==ngroupid=%d nfloor=%d sceneid==%d", data.ngroupid(), data.nfloor(), scene->GetID());
		SkyCityMgr::Instance()->CreateFightSceneEnd(data.ngroupid(), data.nfloor(), scene->GetID());
	}
}


ICreateSkyCityListener ICreateSkyCityListener::GlobalCreateSkyCityListener;



void ICreateSkyCityWaitListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if(0 != nErrCode)
	{
		SSError << " on create skycitywait  err = " << nErrCode << END;
		return ;
	}
	SkyCityMgr::Instance()->CreateSceneWaitEnd(0, 0, scene->GetID());
}

ICreateSkyCityWaitListener ICreateSkyCityWaitListener::GlobalCreateSkyCityWaitListener;

void ICreateTestListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if(scene)
	{
		GuildMatchFightMgr::Instance()->TestEnd(scene->GetID());//test
	}
}

ICreateTestListener ICreateTestListener::GlobalCreateTestListener;



void ICreateResWarListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if(0 != nErrCode)
	{
		SSError << " on create reswar battle err = " << nErrCode << END;
		return ;
	}
	if(scene)
	{
		const auto& data  = roParam.reswar();
		if(data.mineid())
		{
			LogDebug("ICreateResWarListener==ngroupid=%d sceneid==%d mineid=%d", data.groupid(), scene->GetID(), data.mineid());
			ResWarMgr::Instance()->CreatePVESceneEnd(data.groupid(), scene->GetID());
		}else
		{
			LogDebug("ICreateResWarListener==ngroupid=%d sceneid==%d", data.groupid(), scene->GetID());
			ResWarMgr::Instance()->CreatePVPSceneEnd(data.groupid(), scene->GetID());
		}

	}
}


ICreateResWarListener ICreateResWarListener::GlobalCreateResWarListener;


void ICreateInvFightListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if(NULL == scene)
	{
		return ;
	}

	const KKSG::InvFightBattleInfo& info = roParam.invfightinfo();
	if(info.inunit_size() < 2)
	{
		return ;
	}

	InvFightMgr::Instance()->CreateSceneEnd(scene->GetID(), info.inunit(0).roleid(), info.inunit(1).roleid());

}

ICreateInvFightListener ICreateInvFightListener::GlobalCreateInvFightListener;


void ICreateHeroBattleListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	bool issucceed = true;
	if (res.result() != KKSG::ERR_SUCCESS)
	{
		LogError("Create HeroBattle error, errorcode:%d", res.result());
		issucceed = false;
	}
	else if (scene == NULL)
	{
		LogError("Create HeroBattle error, scene is NULL");
		issucceed = false;
	}
	if (issucceed)
	{
		HeroBattleMgr::Instance()->CreateSceneSucceed(roParam.herobattle().uid(), scene->GetID());
	}
	else
	{
		HeroBattleMgr::Instance()->CreateSceneFailed(roParam.herobattle().uid());
	}
}

ICreateHeroBattleListener ICreateHeroBattleListener::GlobalCreateHeroBattleListener;

void ICreateGuildCastleListner::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if(NULL == scene)
	{
		return ;
	}
	const KKSG::GCFCreateBattleInfo& data = roParam.guildcastle();
	auto pOneFight = GuildCastleMgr::Instance()->GetOneFight(data.uid());
	if(NULL == pOneFight)
	{
		SSError << " find one fight failed uid = " << data.uid() << END;
		return ;
	}
	pOneFight->CreateSceneEnd(scene->GetMapID(), scene->GetID());
}

ICreateGuildCastleListner ICreateGuildCastleListner::GlobalCreateGuildCastleListener;

void ICreatePkListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	if(NULL == scene)
	{
		return ;
	}
	PkMgr::Instance()->CreateSceneEnd(roParam.pkinfo(), scene->GetID());
}

ICreatePkListener ICreatePkListener::GlobalCreatePkListener;


void ICreateMobaBattleListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	bool issucceed = true;
	if (res.result() != KKSG::ERR_SUCCESS)
	{
		LogError("Create MobaBattle error, errorcode:%d", res.result());
		issucceed = false;
	}
	else if (scene == NULL)
	{
		LogError("Create MobaBattle error, scene is NULL");
		issucceed = false;
	}
	if (issucceed)
	{
		MobaBattleMgr::Instance()->CreateSceneSucceed(roParam.mobabattle().uid(), scene->GetID());
	}
	else
	{
		MobaBattleMgr::Instance()->CreateSceneFailed(roParam.mobabattle().uid());
	}
}

ICreateMobaBattleListener ICreateMobaBattleListener::GlobalCreateMobaBattleListener;



void ICreateBattleFieldReadyListener::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 nErrCode = res.result();
	if(0 != nErrCode)
	{
		SSError << " on create battlefield ready  err = " << nErrCode << END;
		return ;
	}
	BattleFieldMgr::Instance()->CreateSceneWaitEnd(0, scene->GetLine(), scene->GetID());
}

ICreateBattleFieldReadyListener ICreateBattleFieldReadyListener::GlobalCreateBattleFieldReadyListener;