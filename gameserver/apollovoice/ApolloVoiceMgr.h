#ifndef __APOLLO_VOICE_MGR_H__
#define __APOLLO_VOICE_MGR_H__

#include <set>
#include "api_voip.h"
#include "api_large_room.h"
#include "api_get_authkey.h"


struct stVoipRoomKey
{
	UINT64 sceneID;
	UINT64 teamLeaderID;

	bool operator == ( const stVoipRoomKey& obj) const
	{
		if( sceneID == obj.sceneID && teamLeaderID == obj.teamLeaderID )
		{
			return true; 
		}
		return false; 
	}

	bool operator < ( const stVoipRoomKey& obj ) const
	{
		if( sceneID < obj.sceneID )
		{
			return true; 
		}
		if( sceneID == obj.sceneID && teamLeaderID < obj.teamLeaderID )
		{
			return true; 
		}
		return false;
	}

};

struct stRetry
{
	UINT32 nTime;
	UINT32 nRetry;
	UINT64 sceneID;
	UINT64 leaderID;
};


///>VOIP房间: teamID&scendID唯一标识一个room
struct stVoicRoom
{
	UINT64 roomID;
	UINT64 roomKEY;
	UINT64 sceneID; 
	UINT64 teamLeaderID;
	struct stMemberState
	{
		stMemberState()
		{
			nMemberID = 0;
			nState	  = 0;
		}
		UINT32 nMemberID;
		UINT32 nState;
	};
	///>用户列表：key：roleID  value：memberID
	std::map<UINT64, stVoicRoom::stMemberState> mapRoleIDMemberID;
	stVoicRoom( )
	{
		roomID = 0; 
		roomKEY = 0; 
		sceneID = 0; 
		teamLeaderID = 0; 
		mapRoleIDMemberID.clear();
	}

	void AddRole( UINT64 roleID, UINT32 memberID )
	{
		mapRoleIDMemberID[roleID].nMemberID = memberID;
		mapRoleIDMemberID[roleID].nState	= 0;
	}

	void SetState( UINT64 nRoleID, UINT32 nState)
	{
		mapRoleIDMemberID[nRoleID].nState	= nState;
	}

	void  RemoveRole( UINT64 roleID )
	{
		mapRoleIDMemberID.erase( roleID );
	}

	UINT64 GetRoomID()
	{
		return roomID;
	}

	bool GetMemberID( UINT64 roleID,stMemberState &stState)
	{
		auto itFind = mapRoleIDMemberID.find( roleID );
		if( itFind == mapRoleIDMemberID.end() )
		{
			return false; 
		}
		stState.nMemberID = stState.nMemberID;
		stState.nState	  = stState.nState;
		return true;
	}

	UINT32 GetMemberCount()
	{
		UINT32 count = mapRoleIDMemberID.size();
		return count;
	}
};

class Role;
class VoipInterface;
class VoipEvent;




class ApolloVoiceMgr 
{
	ApolloVoiceMgr();
	~ApolloVoiceMgr();
	DECLARE_SINGLETON(ApolloVoiceMgr)


public:
	bool Init();
	void Uninit();
	void Update();

	void CreateVoipRoom(Role* pRole, UINT64 nSceneID, UINT64 nLeaderID);
	void CloseVoipRoom( UINT64 roomID, UINT32 closeType );		

	///>处理玩家的请求
	void HandleJoinRoom( Role* pRole );
	void HandleExitRoom( Role* pRole,  UINT64 roomID, UINT32 memberID, std::string reason );
	void HandleSetState(Role* pRole,UINT32 nState);

	///>处理SDK回应
	void OnCreateRoomReply( UINT64 roomID, UINT32 dataLen, const char* data );
	void OnJoinRoomReply( Role* pRole, UINT64 roomID, UINT32 memberID );
	void OnExitRoomReply( UINT64 roleID, UINT64 roomID );
	void OnCloseRoomReply( UINT64 roomID );


	///>GM指令： 创建房间
	void GmCreateVoipRoom( Role* pRole );

	///>GM指令：显示voip实例
	void ShowVoipInfo();

	///>查找用户房间ID和memberID
	bool FindRoleVoipInfo( Role* pRole, UINT64& roomID, UINT32& memberID );

	///>刷新房间用户信息列表
	void UpdateVoipRoomMember( UINT64 roomID );

	///>加载语音xml配置文件
	void LoadVoipXML();

	///>将用户添加到等待列表
	void AddWaitRoleID( UINT64 roleID, stVoipRoomKey& rkey );

	///>处理离开场景的情况: 走exitroom流程
	void HandleLeaveScene( UINT64 qwRoleID );

	///>处理失败
	void OnRecvErrorCreateRoom( const char* data, const UINT32 dataLen );
	void RetryCreateApolloRoom(stRetry *pRetry);
	void Retry();
	void OnRecvErrorJoinRoom( const char* data, const UINT32 dataLen );
	void OnRecvErrorExitRoom( const char* data, const UINT32 dataLen );
	void OnRecvErrorCloseRoom( const char* data, const UINT32 dataLen );

private:
	///>voip实例
	ns_voip::VoipInterface* m_pVoip;

	
	//测试用的

	///>房间列表
	std::map<stVoipRoomKey, stVoicRoom> m_mapVoipRoomV2;

	///>roomid--voiproomkey
	std::map<UINT64, stVoipRoomKey> m_mapRoomIDRoomKey;

	///>等待加入的玩家列表 key:VoipRoomKey value: roleID 列表
	std::map<stVoipRoomKey, std::set<UINT64> > m_mapWaitList;

	///>发送过joinroom消息，还没收到joinroomreply的用户ID列表: 等待joinroom确认的用户列表
	std::set<UINT64> m_setWaitJoinRoomReplyRoleID;

	///>xml字段值
	std::vector<std::string> m_vecVoipSvrURL;
	std::string m_strBusinessName;
	std::string m_strBusinessKey;
	std::string m_strWindowsDllName;
	std::string m_strLinuxAName;


	std::vector<stRetry> m_vecRetry;
};

#endif
