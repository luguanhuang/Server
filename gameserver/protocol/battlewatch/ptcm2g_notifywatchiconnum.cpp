#include "pch.h"
#include "battlewatch/ptcm2g_notifywatchiconnum.h"
#include "battlewatch/ptcg2c_notifywatchiconnum2client.h"
#include "table/globalconfig.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/8/2 10:22:47

void PtcM2G_NotifyWatchIconNum::Process(UINT32 dwConnID)
{
	Scene* scene = SceneManager::Instance()->FindStaticScene( GetGlobalConfig().InitialReturnSceneID);
	if (scene)
	{
		PtcG2C_NotifyWatchIconNum2Client send;
		send.m_Data.set_num(m_Data.num());
		scene->Broadcast(send);
	}

	LiveManager::Instance()->SetMainHallLiveNum(m_Data.num());
}
