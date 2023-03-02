#include "pch.h"
#include <time.h>
#include "singleton.h"
#include "unit/role.h"
#include <fstream>
#include "ApolloVoiceMgr.h"
#include <stdio.h>
#include <iostream>
#include "chat/ptcg2c_joinroomreply.h"
#include "chat/ptcc2g_quitroom.h"
#include "chat/ptcg2c_updatevoiproommemberntf.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"
#include "tinyxml2.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "scene/scenepvp.h"
#include "gamelogic/notice.h"
#include "scene/sceneherobattle.h"
#include "scene/sceneweekend4v4monsterfight.h"
#include "scene/sceneweekend4v4ghostaction.h"
#include "scene/sceneweekend4v4livechallenge.h"
#include "scene/sceneweekend4v4crazybomb.h"
#include "scene/sceneweekend4v4horseracing.h"
#include "scene/scenemobabattle.h"

#include<stdio.h>
#include<stdlib.h>


#ifdef WIN32
#include <windows.h>
#endif

///>2015-12-28 by purperzhang
#define DONT_USE_TSF4G_TLOG
#include <iostream>
#include <string.h>
#include <time.h>
#include <string>
#include <stdlib.h>
#include "api_log.h"
#include "api_log_demo.h"
#include "api_voip.h"
#include "scene/sceneweekend4v4duck.h"

INSTANCE_SINGLETON(ApolloVoiceMgr)

#ifdef WIN32
typedef ns_voip::VoipInterface* __stdcall create_voip_instance_t(ns_voip::VoipEventHandler*,ns_log::MLogInterface*);
create_voip_instance_t *create_voip_instance_p;

#endif


class GetAuthkeyEvent:public ns_ga::get_authkey_event_handler
{
public:
	virtual bool	on_get_authkey_return(const ns_ga::GetAuthkeyResult*pr)
	{
		return false;
	}
};


class VoipEvent: public ns_voip::VoipEventHandler
{
public:
	ns_voip::VoipInterface*pvi;

	virtual bool on_create_voiproom_rsp(const ns_voip::CreateVoipRoomResult*pr)
	{
		if(NULL == pr)
		{
			SSError << "Create Voip Room Failed" << END;
			return false;
		}
		///>SSWarn << "on create voip room response, roomid:" << pr->room_id << ", asynclen:" << pr->async_len << END;	
		ApolloVoiceMgr::Instance()->OnCreateRoomReply(pr->room_id, pr->async_len, pr->async_data);
		return true; 
	}

	virtual bool on_join_voiproom_rsp(const ns_voip::JoinVoipRoomResult* pr)
	{
		if(NULL == pr||pr->user_access.access_ip_count<=0)
			return false; 
		std::string strRoleID = std::string(pr->async_data, pr->async_len);
		UINT64 roleID = convert<UINT64>(strRoleID);
		PtcG2C_JoinRoomReply ptc;
		for (UINT32 i = 0; i < pr->user_access.access_ip_count; i++)
		{
			std::string url = XCommon::ReplaceStr(pr->user_access.access_ip_list[i],"udp://",""); 
			if (i==0)
				ptc.m_Data.set_url1(url);
			else if (i==1)
				ptc.m_Data.set_url2(url);
			else if (i==2)
				ptc.m_Data.set_url3(url);
		}
		LogInfo("SDK Reply, RoleID:%llu,url1=%s,url2=%s,url3=%s", roleID, ptc.m_Data.url1().c_str(),  ptc.m_Data.url2().c_str(), ptc.m_Data.url3().c_str());
		ptc.m_Data.set_roomid(pr->room_id);
		ptc.m_Data.set_roomkey(pr->user_access.room_key);
		ptc.m_Data.set_memberid(pr->user_access.member_id);
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
		if(NULL != pRole)
		{
			pRole->Send(ptc, true);
			LogInfo("SUCESS RET RoleID:%llu,name=%s,url1=%s,url2=%s,url3=%s", roleID, pRole->GetName().c_str(), ptc.m_Data.url1().c_str(), ptc.m_Data.url2().c_str(),ptc.m_Data.url3().c_str());
			ApolloVoiceMgr::Instance()->OnJoinRoomReply(pRole, pr->room_id, pr->user_access.member_id);
		}else
		{
			LogError("ROLE NOT EXIST RoleID:%llu,url1=%s,url2=%s,url3=%s",roleID, ptc.m_Data.url1().c_str(), ptc.m_Data.url2().c_str(),ptc.m_Data.url3().c_str());
		}
		return true; 
	}

	virtual bool on_exit_voiproom_rsp(const ns_voip::ExitVoipRoomResult* pr)
	{
		if(NULL == pr)
		{
			SSError << "Exit Room Response From VoipServer Null" << END;
			return false;
		}

		std::string strRoleID = std::string(pr->async_data, pr->async_len);
		UINT64 roleID = convert<UINT64>(strRoleID);
		ApolloVoiceMgr::Instance()->OnExitRoomReply(roleID, pr->room_id);

		///>房间剩余人数为0，关闭房间
		if(0 == pr->member_count_after_exit)
		{
			///>SSWarn << "RoomMember Count After Exit Zero, Try To Close Room, RoomID:" << pr->room_id << ", MemCount:" << pr->member_count_after_exit << END;
			ApolloVoiceMgr::Instance()->CloseVoipRoom(pr->room_id, 1);			
		}

		return true; 
	}

	virtual bool on_close_voiproom_rsp(const ns_voip::CloseVoipRoomResult* pr)
	{
		if(NULL == pr)
		{
			return false; 
		}
		SSInfo << "recv close voip room response from voip server, roomid:" << pr->room_id << ", datalen:" << pr->async_len << END;
		ApolloVoiceMgr::Instance()->OnCloseRoomReply(pr->room_id);
		return true; 
	}

	virtual bool on_recv_error(const ns_voip::VoipReturnError* pr)
	{
		if(NULL == pr)
		{
			return false; 
		}
		UINT32 cmd = pr->req_cmd;
		UINT32 dataLen = pr->async_len;
		UINT32 code = pr->error_code;
		std::string errdesc = pr->error_desc;
		SSWarn << "on recv error, cmd:" << cmd << ", dataLen:" << dataLen << ", code:" << code << ", errdesc:" << errdesc << END;
		switch(pr->req_cmd)
		{
		case ns_voip::VOIP_CMD_CREATE_ROOM:
			break;
		case ns_voip::VOIP_CMD_JOIN_ROOM:
			break;
		case ns_voip::VOIP_CMD_EXIT_ROOM:
			break;
		case ns_voip::VOIP_CMD_CLOSE_ROOM:
			break;
		default:
			SSInfo << "unknown command, cmd:" << pr->req_cmd << END;
		}
		if(pr->req_cmd == ns_voip::VOIP_CMD_CREATE_ROOM)
		{
			SSWarn << "Recv Voip SDK Error, cmd=VOIP_CMD_CREATE_ROOM" << ", Code:" << code << ", ErrorDesc:" << errdesc << END;
			ApolloVoiceMgr::Instance()->OnRecvErrorCreateRoom(pr->async_data, pr->async_len);
			return true; 
		}
		if(pr->req_cmd == ns_voip::VOIP_CMD_JOIN_ROOM)
		{
			SSWarn << "Recv Voip SDK Error, cmd=VOIP_CMD_JOIN_ROOM" << ", Code:" << code << ", ErrorDesc:" << errdesc << END;
			ApolloVoiceMgr::Instance()->OnRecvErrorJoinRoom(pr->async_data, pr->async_len);
			return true; 
		}
		if(pr->req_cmd == ns_voip::VOIP_CMD_EXIT_ROOM)
		{
			SSWarn << "Recv Voip SDK Error, cmd=VOIP_CMD_EXIT_ROOM" << ", Code:" << code << ", ErrorDesc:" << errdesc << END;
			ApolloVoiceMgr::Instance()->OnRecvErrorExitRoom(pr->async_data, pr->async_len);
			return true; 
		}
		if(pr->req_cmd == ns_voip::VOIP_CMD_CLOSE_ROOM)
		{
			SSWarn << "Recv Voip SDK Error, cmd=VOIP_CMD_CLOSE_ROOM" << ", Code:" << code << ", ErrorDesc:" << errdesc << END;
			ApolloVoiceMgr::Instance()->OnRecvErrorCloseRoom(pr->async_data, pr->async_len);
			return true; 
		}

		SSError << "Recv Voip SDK Error, cmd Unknown" << ", Code:" << code << ", ErrorDesc:" << errdesc << END;
		return true;		
	}
};

#ifdef WIN32
void* load_voip_library(const char *dll,const char *fname)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("voip.dll");

	if (!hGetProcIDDLL) 
	{
		UINT32 errorcode = GetLastError();
		std::cout << "could not load the dynamic library"<<GetLastError() << std::endl;
		return NULL;
	}
	void* funci = GetProcAddress(hGetProcIDDLL, fname);
	if (!funci) 
	{
		std::cout << "could not locate the function" << std::endl;
		return NULL;
	}
	printf("LoadLibrary[%s=>%s]=(%p)",dll,fname,funci);
	return funci;
}

#endif

ApolloVoiceMgr::ApolloVoiceMgr()
{
}

ApolloVoiceMgr::~ApolloVoiceMgr()
{
}


///>windows平台初始化
#ifdef WIN32

bool ApolloVoiceMgr::Init()
{
	LoadVoipXML();
	m_mapRoomIDRoomKey.clear();
	m_mapVoipRoomV2.clear();
	std::string dllname = "voip.dll";
	static VoipEvent ve;
	ns_log::MLog* pmlog = new ns_log::MLog();
	create_voip_instance_p = (create_voip_instance_t *)load_voip_library(dllname.c_str(), "create_voip_instance");
	if(!create_voip_instance_p)
	{
		SSError << "create voip instance pointer failed" << END;
		return false;
	}
	m_pVoip = create_voip_instance_p(&ve, pmlog);
	if(!m_pVoip)
	{
		SSError << "failed to get method addr error" << END;
		return false; 
	}
	ns_voip::VoipConfig config;
	memset(config.dir_svr_url[0], 0, sizeof(config.dir_svr_url[0]));
	memset(config.business_key, 0, sizeof(config.business_key));
	memset(config.business_name, 0, sizeof(config.business_name));


	for (UINT32 i = 0; i < m_vecVoipSvrURL.size() && i<3; i++)
	{
		strncpy(config.dir_svr_url[i],  m_vecVoipSvrURL[i].c_str(),  m_vecVoipSvrURL[i].length()+1);
		config.dir_svr_url_count++;
	}

	strncpy(config.business_name, m_strBusinessName.c_str(), m_strBusinessName.size());
	strncpy(config.business_key, m_strBusinessKey.c_str(), m_strBusinessKey.size());
	if(!m_pVoip->init(config))
	{
		SSError << "pvi init error" << END;
		return false; 
	}
	ve.pvi = m_pVoip;
	return true;
}
#endif

///>linux平台初始化
#ifndef WIN32
bool ApolloVoiceMgr::Init()
{
	LoadVoipXML();
	m_mapVoipRoomV2.clear();
	m_mapRoomIDRoomKey.clear();
	static VoipEvent ve;
	ns_log::MLog* pmlog = new ns_log::MLog();
	m_pVoip = create_voip_instance(&ve, pmlog);
	if(!m_pVoip)
	{
		SSError << "failed to get method addr error" << END;
		return false; 
	}
	ns_voip::VoipConfig config;
	memset(config.business_key, 0, sizeof(config.business_key));
	memset(config.business_name, 0, sizeof(config.business_name));
	memset(config.dir_svr_url[0], 0, sizeof(config.dir_svr_url[0]));

	for (UINT32 i = 0; i < m_vecVoipSvrURL.size(); i++)
	{
		strncpy(config.dir_svr_url[i],  m_vecVoipSvrURL[i].c_str(),  m_vecVoipSvrURL[i].length()+1);
		config.dir_svr_url_count++;
	}
	strncpy(config.business_name, m_strBusinessName.c_str(), m_strBusinessName.size());
	strncpy(config.business_key, m_strBusinessKey.c_str(), m_strBusinessKey.size());
	if(!m_pVoip->init(config))
	{
		SSError << "pvi init error" << END;
		return false; 
	}
	ve.pvi = m_pVoip;
	///>SSWarn << "init apollo voice manager success, url:" << m_strVoipSvrURL << ", businessname:" << m_strBusinessName << ", businesskey:" << m_strBusinessKey << END;
	return true;
}
#endif

void ApolloVoiceMgr::Uninit()
{
	
}

void ApolloVoiceMgr::Update()
{
	Retry();
	m_pVoip->update();

}


void  ApolloVoiceMgr::CreateVoipRoom(Role* pRole, UINT64 nSceneID, UINT64 nLeaderID)
{
	stVoipRoomKey rkey;
	rkey.sceneID = nSceneID;
	rkey.teamLeaderID = nLeaderID;
	auto itFind = m_mapVoipRoomV2.find(rkey);
	if(itFind != m_mapVoipRoomV2.end())
		return; 
	KKSG::ApolloRoomKey kv;
	kv.set_key(rkey.sceneID);
	kv.set_value(rkey.teamLeaderID);
	kv.set_retry(0);
	std::string strKV = "";
	if(!kv.SerializeToString(&strKV))
	{
		SSError << "Pack Protobuf Failed, SceneID:" << itFind->first.sceneID << ", TeamID:" << itFind->first.teamLeaderID << ", RoleID:" << pRole->GetRoleID() << ", Name:" << pRole->GetName() << END; 
		return; 
	}
	ns_voip::CreateVoipRoomPara param;
	memset(param.async_data, 0, sizeof(param.async_data));
	if(strKV.size() + 1 > sizeof(param.async_data))
	{
		SSError << "Param Too Big, SceneID:" << itFind->first.sceneID << ", TeamID:" << itFind->first.teamLeaderID << ", RoleID:" << pRole->GetRoleID() << ", Name:" << pRole->GetName() << ", ParamSize:" << strKV.size() << ", MaxLenLimit:" << sizeof(param.async_data) << END;
		return; 
	}
	memcpy(param.async_data, strKV.c_str(), strKV.size());
	param.async_len = strKV.size(); 
	if(!m_pVoip->send_create_voiproom_req(&param))
	{
		SSError << "send_create_voiproom_req error, roleID:" << pRole->GetRoleID() << ", name:" << pRole->GetName() << END;
		return;
	}
}


void ApolloVoiceMgr::CloseVoipRoom(UINT64 roomID, UINT32 closeType)
{
	ns_voip::CloseVoipRoomPara param;
	param.room_id = roomID;
	param.close_type = closeType;
	param.async_data[0] = '\0';
	param.async_len = 0; 
	if(!m_pVoip->send_close_voiproom_req(&param))
	{
		SSError << "send close voip room request error, roomid:" << roomID << ", closeType:" << closeType << END;
		return; 
	}
}



void ApolloVoiceMgr::HandleJoinRoom(Role* pRole)
{
	if(!pRole||pRole->IsWatcher())
		return; 
	Scene* pScene = pRole->GetCurrScene();
	if(!pScene)
	{
		pScene = pRole->GetLoadingScene();
		if (!pScene)
		{
			SSError << "Cannot Find Role Current Scene Pointer, RoleID:" << pRole->GetRoleID() << ", Name:" << pRole->GetName() << END;
			return;
		}
	}

	UINT32 sceneID		= pScene->GetSceneID();
	UINT32 sceneType	= pScene->GetSceneType();
	UINT64 leaderID		= 0; 

	if(KKSG::SCENE_PVP == sceneType)
	{
		ScenePvp* pPvpScene = pScene->GetPvpHandler();
		if (!pPvpScene)
			return;
		leaderID = pPvpScene->GetGroup(pRole->GetRoleID());
	}else if(KKSG::SCENE_HEROBATTLE == sceneType)
	{
		SceneHeroBattle* pHeroScene = pScene->GetHeroBattleHandler();
		if (!pHeroScene)
			return;
		leaderID = pHeroScene->GetGroup(pRole->GetRoleID());
	}
	else if (KKSG::SKYCITY_FIGHTING == sceneType)
	{
		TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetRoleID());
		if(!pTeam)
			return; 
		leaderID = pTeam->GetTeamID();
	}else if (KKSG::SCENE_RESWAR_PVP == sceneType)
	{
		TeamCopy* pTeam = TeamCopyMgr::Instance()->GetTeamByRoleID(pRole->GetRoleID());
		if(!pTeam)
			return; 
		leaderID = pTeam->GetTeamID();
	}else if (KKSG::SCENE_WEEKEND4V4_MONSTERFIGHT  == sceneType)
	{
		SceneWeekEnd4v4MonsterFight* pHandler = pScene->GetMonsterFightHandler();
		if(pHandler==NULL)
			return;
		leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
		if (leaderID == (UINT64)-1)
			return;

	}else if (KKSG::SCENE_WEEKEND4V4_LIVECHALLENGE  == sceneType)
	{
		 SceneWeekEnd4v4LiveChallenge* pHandler = pScene->GetLiveChallengeHandler();
		 if(pHandler==NULL)
			 return;
		 leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
		 if (leaderID == (UINT64)-1)
			 return;

	}else if (KKSG::SCENE_WEEKEND4V4_CRAZYBOMB  == sceneType)
	{
		SceneWeekEnd4v4CrazyBomb* pHandler = pScene->GetCrazyBombHandler();
		if(pHandler==NULL)
			return;
		leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
		if (leaderID == (UINT64)-1)
			return;

	}else if (KKSG::SCENE_WEEKEND4V4_HORSERACING == sceneType)
	{
		 SceneWeekEnd4v4HorseRacing* pHandler = pScene->GetWeekEnd4v4HorseRacingHandler();
		 if(pHandler==NULL)
			 return;
		 leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
		 if (leaderID == (UINT64)-1)
			 return;
	}else if (KKSG::SCENE_WEEKEND4V4_GHOSTACTION==sceneType)
	{
		SceneWeekEnd4v4GhostAction* pHandler = pScene->GetGhostActionHandler();
		if(pHandler==NULL)
			return;
		leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
		if (leaderID == (UINT64)-1)
			return;
	}else if (KKSG::SCENE_MOBA==sceneType)
	{
		SceneMobaBattle *pHandle = pScene->GetMobaBattleHandler();
		if (pHandle==NULL)
			return;
		leaderID = pHandle->GetGroup(pRole->GetRoleID());
	}
    else if (KKSG::SCENE_WEEKEND4V4_DUCK == sceneType)
    {
        SceneWeekEnd4v4Duck* pHandler = pScene->GetDuckHandler();
        if(pHandler==NULL)
            return;
        leaderID = pHandler->GetVoipRoomID(pRole->GetRoleID());
        if (leaderID == (UINT64)-1)
            return;
    }

	stVoipRoomKey rkey; 
	rkey.sceneID = sceneID;
	rkey.teamLeaderID = leaderID;
	LogInfo("joinroom roleid=%llu sceneid=%u scenetype=%u leaderid=%llu",pRole->GetRoleID() ,sceneID,sceneType,leaderID);
	auto itFind = m_mapVoipRoomV2.find(rkey);
	if(itFind == m_mapVoipRoomV2.end())
	{
		AddWaitRoleID(pRole->GetRoleID(), rkey);
		CreateVoipRoom(pRole, sceneID, leaderID);
		return; 
	}

	ns_voip::JoinVoipRoomPara para;
	para.room_id = itFind->second.roomID;
	strncpy(para.user.open_id, pRole->GetAccount().c_str(), sizeof(para.user.open_id));
	strncpy(para.user.client_ip,  pRole->GetClientConf().ip().c_str(), sizeof(para.user.client_ip));
	para.user.net_type=ns_voip::VOIP_NET_TYPE_DEFAULT;
	memset(para.async_data, 0, sizeof(para.async_data));
	para.async_len = snprintf(para.async_data, sizeof(para.async_data), "%llu", pRole->GetRoleID());
	if(!m_pVoip->send_join_voiproom_req(&para))
	{
		SSError << "send join voip room request error, roleid:" << pRole->GetRoleID() << ", name:" << pRole->GetName() << END;
		return;
	}
}


void ApolloVoiceMgr::HandleExitRoom(Role* pRole,  UINT64 roomID, UINT32 memberID, std::string reason)
{
	if(!pRole||pRole->IsWatcher())
		return; 
	ns_voip::ExitVoipRoomPara param;
	param.room_id	= roomID;
	param.member_id = memberID;
	param.exit_type = 0; 
	param.async_len = snprintf(param.async_data, sizeof(param.async_data), "%llu", pRole->GetRoleID());
	memset(param.user_open_id, 0, sizeof(param.user_open_id));

	if(!m_pVoip->send_exit_voiproom_req(&param))
	{
		SSError << "Send Exit Voip Room Failed, RoleID:" << pRole->GetRoleID() << ", Name:" << pRole->GetName() << ", RoomID:" << roomID << ", MemberID:" << memberID << END;
		return; 
	}
}

void ApolloVoiceMgr::HandleSetState(Role* pRole,UINT32 nState)
{
	Notice notice(ApolloVice_Notice);
	notice.Add(pRole);
	for (auto iter = m_mapVoipRoomV2.begin(); iter != m_mapVoipRoomV2.end(); iter++)
	{
		stVoicRoom &room = iter->second;
		auto itFind = room.mapRoleIDMemberID.find(pRole->GetRoleID());
		if(itFind != room.mapRoleIDMemberID.end())
		{
			auto &itMemberID = itFind->second;
			itMemberID.nState  = nState;
			UpdateVoipRoomMember(room.roomID);
			if (nState == 2)
			{
				for(auto siter = room.mapRoleIDMemberID.begin(); siter!= room.mapRoleIDMemberID.end(); siter++)
				{
					Role *pRoomRole = RoleManager::Instance()->FindByRoleID(siter->first);
					if (pRoomRole)
					{
						notice.SendSelf(pRoomRole);
					}
				}
			}
			break;
		}
	}
}

///>处理SDK回应: 创建房间
void ApolloVoiceMgr::OnCreateRoomReply(UINT64 roomID, UINT32 dataLen, const char* data)
{
	if(dataLen==0||!data)
	{
		SSError << "dataLen:" << dataLen << ", should be sceneid" << END;
		return; 
	}
	std::string strSceneID = std::string(data, dataLen);
	KKSG::ApolloRoomKey kv; 
	if(!kv.ParseFromString(strSceneID))
	{
		SSError << "parse error" <<END;
		return;
	}
	UINT64 sceneID		= kv.key();
	UINT64 leaderID		= kv.value();
	stVoipRoomKey rkey; 
	rkey.sceneID		= sceneID;
	rkey.teamLeaderID	= leaderID;
	auto itFind = m_mapVoipRoomV2.find(rkey);
	if(itFind != m_mapVoipRoomV2.end())
	{
		SSWarn << "RoomID Already Exist, RoomID:" << roomID << END;
		return; 
	}
	stVoicRoom room;
	room.roomID			= roomID;
	room.roomKEY		= 0; 
	room.sceneID		= sceneID;
	room.teamLeaderID	= leaderID;
	room.mapRoleIDMemberID.clear();
	m_mapVoipRoomV2.insert(std::make_pair(rkey, room));
	m_mapRoomIDRoomKey.insert(std::make_pair(room.roomID, rkey));
	SSInfo << "MARK1, Create Voip Room Success, RoomID:" << roomID << ", SceneID:" << sceneID << END;
	///>检查是否有在等待的用户列表
	auto waitFind = m_mapWaitList.find(rkey);
	if(waitFind == m_mapWaitList.end())
		return; 
	for(auto iter = waitFind->second.begin(); iter != waitFind->second.end(); iter++)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(*iter);
		if(pRole)
			HandleJoinRoom(pRole);
	}
	///>将玩家从等待列表中删除
	m_mapWaitList.erase(rkey);
	SSInfo << "MARK2, Over, Create Voip Room Success, RoomID:" << roomID << ", SceneID:" << sceneID << END;
}

///>处理SDK回应: 加入房间
void ApolloVoiceMgr::OnJoinRoomReply(Role* pRole, UINT64 roomID, UINT32 memberID)
{
	if(NULL == pRole)
	{
		return; 
	}
	auto itFind = m_mapRoomIDRoomKey.find(roomID);
	if(itFind == m_mapRoomIDRoomKey.end())
	{
		LogInfo("Cannot Find Scene For Role, RoleID:%llu,Name:%s",pRole->GetRoleID(),pRole->GetName().c_str());
		return; 
	}
	auto itRoomFind = m_mapVoipRoomV2.find(itFind->second);
	if(itRoomFind == m_mapVoipRoomV2.end())
	{
		LogInfo("dd Role To Room Failed, Cannot Find Room, RoleID:%llu,Name:%s",pRole->GetRoleID(),pRole->GetName().c_str());
		return; 
	}
	itRoomFind->second.AddRole(pRole->GetRoleID(), memberID);
	///>更新房间用户列表
	UpdateVoipRoomMember(roomID);
}

///>处理SDK回应: 退出房间
void ApolloVoiceMgr::OnExitRoomReply(UINT64 roleID, UINT64 roomID)
{
	auto itFind = m_mapRoomIDRoomKey.find(roomID);
	if(itFind == m_mapRoomIDRoomKey.end())
	{
		SSError << "Cannot Find Scene For Room, RoleID:" << roleID << ", RoomID:" << roomID << END;
		return; 
	}
	auto roomFind = m_mapVoipRoomV2.find(itFind->second);
	if(roomFind == m_mapVoipRoomV2.end())
	{
		SSError << "Cannot Find Voip Room For SceneID:" << itFind->second.sceneID <<  ", LeaderID:"  << itFind->second.teamLeaderID << END;
		return; 
	}
	roomFind->second.RemoveRole(roleID);
	SSInfo << "Player Exit Voip Room Success, RoleID:" << roleID << ", RoomID:" << roomID << END;

	///>刷新成员列表
	UpdateVoipRoomMember(roomID);

	///>如果剩余成员数量为0，则关闭房间
	UINT32 memberCount = roomFind->second.mapRoleIDMemberID.size();
	if(0 == memberCount)
	{
		SSWarn << "Voip Room Member Zero, Close Room, RoomID:" << roomID << ", RoleID:" << roleID << END;
		CloseVoipRoom(roomID, 1);
	}
}

///>处理SDK回应: 关闭房间
void ApolloVoiceMgr::OnCloseRoomReply(UINT64 roomID)
{
	auto itFind = m_mapRoomIDRoomKey.find(roomID);
	if(itFind == m_mapRoomIDRoomKey.end())
	{
		SSError << "Cannot Find RoomID, RoomID:" << roomID << END;
		return; 
	}
	m_mapVoipRoomV2.erase(itFind->second);
	m_mapRoomIDRoomKey.erase(roomID);
	SSInfo << "Close Voip Room Success, RoomID:" << roomID << END;
}

///>GM测试指令
#ifdef WIN32
void ApolloVoiceMgr::GmCreateVoipRoom(Role* pRole)
{
}
#endif

#ifdef linux
void ApolloVoiceMgr::GmCreateVoipRoom(Role* pRole)
{
}
#endif


///>GM指令：显示voip实例
void ApolloVoiceMgr::ShowVoipInfo()
{
}

///>查找用户房间ID和memberID
bool ApolloVoiceMgr::FindRoleVoipInfo(Role* pRole, UINT64& roomID, UINT32& memberID)
{
	if(!pRole)
		return false;
	for(auto i = m_mapVoipRoomV2.begin(); i!= m_mapVoipRoomV2.end(); i++)
	{
		auto itFind = i->second.mapRoleIDMemberID.find(pRole->GetRoleID());
		if(itFind != i->second.mapRoleIDMemberID.end())
		{
			roomID		= i->second.roomID;
			memberID	= itFind->second.nMemberID;
			return true;
		}
	}
	return false;
}

///>刷新房间用户信息列表
void ApolloVoiceMgr::UpdateVoipRoomMember(UINT64 roomID)
{
	auto keyFind = m_mapRoomIDRoomKey.find(roomID);
	if(keyFind == m_mapRoomIDRoomKey.end())
		return; 
	auto roomFind = m_mapVoipRoomV2.find(keyFind->second);
	if(roomFind == m_mapVoipRoomV2.end())
		return; 
	PtcG2C_UpdateVoipRoomMemberNtf ntf;

	for(auto iter= roomFind->second.mapRoleIDMemberID.begin(); iter != roomFind->second.mapRoleIDMemberID.end(); iter++)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(iter->first);
		if(pRole)
		{
			KKSG::VoipRoomMember* member = ntf.m_Data.add_datalist();
			member->set_memberid(roomFind->second.mapRoleIDMemberID[iter->first].nMemberID);
			member->set_state(roomFind->second.mapRoleIDMemberID[iter->first].nState);
			member->set_roleid(pRole->GetRoleID());
			member->set_name(pRole->GetName());
		}
	}
	for(auto iter= roomFind->second.mapRoleIDMemberID.begin(); iter != roomFind->second.mapRoleIDMemberID.end(); iter++)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(iter->first);
		if(pRole)
			pRole->Send(ntf, true);	
	}	
}

///>加载语音xml配置文件
void ApolloVoiceMgr::LoadVoipXML()
{
	///>xml路径
	std::string voipFilePath = "gsconf/serveronly/";

	///>配置文件路径
	std::string voipParamFile = voipFilePath + "voipparam.xml";

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(voipParamFile.c_str());
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("Parser Voip Param File %s get XMLError code: %d", voipParamFile.c_str(), error);
		return;
	}
	tinyxml2::XMLElement* rootElem = doc.RootElement();
	if(NULL == rootElem)
	{
		LogError("XML Root Null, FileName: %s", voipParamFile.c_str());
		return; 
	}
	tinyxml2::XMLElement *paramvalue = rootElem->FirstChildElement("ParamValue");  
	if(NULL == paramvalue)
	{
		LogError("Param Value Node Null, FileName: %s", voipParamFile.c_str());
		return; 
	}	
	

	m_vecVoipSvrURL = Split(paramvalue->Attribute("dir_svr_url"), '|');
	m_strBusinessName = paramvalue->Attribute("business_name");
	m_strBusinessKey = paramvalue->Attribute("business_key");

	tinyxml2::XMLElement* libNode = rootElem->FirstChildElement("LibName");
	if(NULL == libNode)
	{
		LogError("LibName Node Null, FileName: %s", voipParamFile.c_str());
		return; 
	}
	m_strWindowsDllName = libNode->Attribute("windowsdll");
	m_strLinuxAName = libNode->Attribute("linuxa");
	return; 
}

///>将用户添加到等待列表
void ApolloVoiceMgr::AddWaitRoleID(UINT64 roleID, stVoipRoomKey& rkey)
{
	m_mapWaitList[rkey].insert(roleID);
}

///>处理离开场景的情况: 走exitroom流程
void ApolloVoiceMgr::HandleLeaveScene(UINT64 qwRoleID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleID);
	if(!pRole)
		return; 
	UINT64 roomID	= 0; 
	UINT32 memberID = 0; 
	bool bRet = FindRoleVoipInfo(pRole, roomID, memberID);
	if(0 == roomID ||bRet== false)
	{
		return; 
	}
	HandleExitRoom(pRole, roomID, memberID, "PlayerLeaveScene");
}

///>处理失败: SDK文档介绍，实时语音大房间和小房间的所有创建房间请求，无论返回什么错误码，均可重试。而加入退出及关闭房间不要重试。
void ApolloVoiceMgr::OnRecvErrorCreateRoom(const char* data, const UINT32 dataLen)
{
	if(NULL == data || 0 == dataLen)
	{
		return; 
	}
	ns_voip::CreateVoipRoomPara createroomparam;
	if(dataLen + 1 > sizeof(createroomparam.async_data))
	{
		SSError << "Create Voip Room Param Size Too Long" << ", SizeOf Param: " << sizeof(createroomparam.async_data) << ", DataLen:" << dataLen << END;
		return; 
	}
	std::string strSceneID = std::string(data, dataLen);
	KKSG::ApolloRoomKey kv; 
	if(!kv.ParseFromString(strSceneID))
		SSError << "parse error" << END;
	UINT64 sceneID		= kv.key();
	UINT64 leaderID		= kv.value();
	UINT32 nRetry        = kv.retry();
	if (kv.retry()>=1)
	{
		SSError << "Retry 1 Times Ignore" << "sceneid =" <<sceneID << "leaderID"<<leaderID <<END;
		return;
	}
	stRetry retry; 
	retry.nRetry = kv.retry()+1;
	retry.sceneID = kv.key();
	retry.leaderID = kv.value();
	retry.nTime = TimeUtil::GetTime() + 2;
	m_vecRetry.push_back(retry);
}


void ApolloVoiceMgr::RetryCreateApolloRoom(stRetry *pRetry)
{
	std::string strKV = "";
	KKSG::ApolloRoomKey kv;
	kv.set_key(pRetry->sceneID);
	kv.set_value(pRetry->leaderID);
	kv.set_retry(pRetry->nRetry);
	bool bRet = kv.SerializeToString(&strKV);
	ns_voip::CreateVoipRoomPara param;
	memset(param.async_data, 0, sizeof(param.async_data));
	memcpy(param.async_data, strKV.c_str(), strKV.size());
	param.async_len = strKV.size(); 
	m_pVoip->send_create_voiproom_req(&param);
}


void ApolloVoiceMgr::Retry()
{
	UINT32 nTime = TimeUtil::GetTime();
	for (auto iter = m_vecRetry.begin(); iter!=m_vecRetry.end();)
	{
		if (nTime >= (*iter).nTime)
		{
			RetryCreateApolloRoom(&(*iter));
			iter= m_vecRetry.erase(iter);
		}else
		{
			iter++;
		}
	}
}



void ApolloVoiceMgr::OnRecvErrorJoinRoom(const char* data, const UINT32 dataLen)
{
	if(NULL == data || 0 == dataLen)
	{
		return; 
	}
}

void ApolloVoiceMgr::OnRecvErrorExitRoom(const char* data, const UINT32 dataLen)
{
	if(NULL == data || 0 == dataLen)
	{
		return; 
	}
}

void ApolloVoiceMgr::OnRecvErrorCloseRoom(const char* data, const UINT32 dataLen)
{
	if(NULL == data || 0 == dataLen)
	{
		return; 
	}
}
