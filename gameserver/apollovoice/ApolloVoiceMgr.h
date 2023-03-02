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


///>VOIP����: teamID&scendIDΨһ��ʶһ��room
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
	///>�û��б�key��roleID  value��memberID
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

	///>������ҵ�����
	void HandleJoinRoom( Role* pRole );
	void HandleExitRoom( Role* pRole,  UINT64 roomID, UINT32 memberID, std::string reason );
	void HandleSetState(Role* pRole,UINT32 nState);

	///>����SDK��Ӧ
	void OnCreateRoomReply( UINT64 roomID, UINT32 dataLen, const char* data );
	void OnJoinRoomReply( Role* pRole, UINT64 roomID, UINT32 memberID );
	void OnExitRoomReply( UINT64 roleID, UINT64 roomID );
	void OnCloseRoomReply( UINT64 roomID );


	///>GMָ� ��������
	void GmCreateVoipRoom( Role* pRole );

	///>GMָ���ʾvoipʵ��
	void ShowVoipInfo();

	///>�����û�����ID��memberID
	bool FindRoleVoipInfo( Role* pRole, UINT64& roomID, UINT32& memberID );

	///>ˢ�·����û���Ϣ�б�
	void UpdateVoipRoomMember( UINT64 roomID );

	///>��������xml�����ļ�
	void LoadVoipXML();

	///>���û���ӵ��ȴ��б�
	void AddWaitRoleID( UINT64 roleID, stVoipRoomKey& rkey );

	///>�����뿪���������: ��exitroom����
	void HandleLeaveScene( UINT64 qwRoleID );

	///>����ʧ��
	void OnRecvErrorCreateRoom( const char* data, const UINT32 dataLen );
	void RetryCreateApolloRoom(stRetry *pRetry);
	void Retry();
	void OnRecvErrorJoinRoom( const char* data, const UINT32 dataLen );
	void OnRecvErrorExitRoom( const char* data, const UINT32 dataLen );
	void OnRecvErrorCloseRoom( const char* data, const UINT32 dataLen );

private:
	///>voipʵ��
	ns_voip::VoipInterface* m_pVoip;

	
	//�����õ�

	///>�����б�
	std::map<stVoipRoomKey, stVoicRoom> m_mapVoipRoomV2;

	///>roomid--voiproomkey
	std::map<UINT64, stVoipRoomKey> m_mapRoomIDRoomKey;

	///>�ȴ����������б� key:VoipRoomKey value: roleID �б�
	std::map<stVoipRoomKey, std::set<UINT64> > m_mapWaitList;

	///>���͹�joinroom��Ϣ����û�յ�joinroomreply���û�ID�б�: �ȴ�joinroomȷ�ϵ��û��б�
	std::set<UINT64> m_setWaitJoinRoomReplyRoleID;

	///>xml�ֶ�ֵ
	std::vector<std::string> m_vecVoipSvrURL;
	std::string m_strBusinessName;
	std::string m_strBusinessKey;
	std::string m_strWindowsDllName;
	std::string m_strLinuxAName;


	std::vector<stRetry> m_vecRetry;
};

#endif
