#ifndef _H_Role_H__
#define _H_Role_H__

#include "baselink.h"
#include "commondef.h"
#include "pb/project.pb.h"
#include "commondef.h"

namespace KKSG
{
	class RoleAllInfo;
};

enum ERoleState
{
	ROLE_STATE_INVALID,
	ROLE_STATE_IN_GAME,				//游戏中
	ROLE_STATE_LEAVE_SCENE,			//离开场景中
	ROLE_STATE_ENTER_SCENE,			//进入场景中
};

// 共享内存中的Role信息，必须保证POD
struct CShmRole
{
	SessionType m_sessionID;
	char m_accountID[MAX_ACCOUNT_LEN];		//tencent openid
	char m_strName[MAX_NAME_LEN];
	ERoleState	m_nState;
	UINT64		m_qwID;
	UINT32		m_dwServerId;				// 入口serverid
	UINT32		m_dwStateAdjustBeginTime;

	// 当前场景信息
	UINT32		m_dwGsLine;
	bool		m_isCrossGs;		//是否跨服GS
	UINT32		m_dwMapID;			//所在场景模板ID
	UINT32		m_dwSceneID;		//所在场景实例ID (动态场景有效)
	bool		m_isWatcher;

	// 切场景相关信息
	UINT32		m_dwDestGsLine;
	UINT32		m_dwDestOwnerID;
	UINT32		m_dwDestMapID;		// TemplateID，静态场景，副本使用
	UINT32		m_dwDestSceneID;	// InstanceID, 进入一个指定场景
	bool		m_bDestIsCross;		// DestGsLine是否跨服GS
	bool		m_DestIsWatcher;
	void ClearSwitch()
	{
		m_dwDestGsLine = INVALID_LINE_ID;
		m_dwDestOwnerID = 0;
		m_dwDestMapID = INVALID_MAP_ID;
		m_dwDestSceneID = INVALID_SCENE_ID;
		m_bDestIsCross = false;
		m_DestIsWatcher = false;
	}
};

class CProtocol;
class CRole
{
public:
	CRole();
	~CRole();
	friend class CRoleManager;

	bool Init(const KKSG::RoleAllInfo& roInfo, UINT64 sessionID);
	bool Init(CShmRole* roShmRole);
	void Uninit();


	UINT64 GetID() const				{ return m_poShmData->m_qwID; }
	SessionType GetSessionID() const	{ return m_poShmData->m_sessionID; }
	const std::string& GetAccount() const{ return m_accountID; }
	const std::string &GetName() const	{ return m_strName; }
	ERoleState GetState() const			{ return m_poShmData->m_nState; }
	UINT32 GetGsLine() const			{ return m_poShmData->m_dwGsLine; }
	UINT32 GetGateLine() const			{ return (m_poShmData->m_sessionID >> SessionGateIDShift) & 0xFF; }
	UINT32 GetMapID() const				{ return m_poShmData->m_dwMapID; }
	UINT32 GetSceneID() const			{ return m_poShmData->m_dwSceneID; }
	bool IsWathcer() const				{ return m_poShmData->m_isWatcher; }

	void   SetStateAdjustBeginTime(UINT32 dwTime) { m_poShmData->m_dwStateAdjustBeginTime = dwTime; }
	UINT32 GetStateAdjustBeginTime() const { return m_poShmData->m_dwStateAdjustBeginTime; }

	bool IsInCrossGs()					{ return m_poShmData->m_isCrossGs; }
	void SetInCrossGs(bool cross)		{ m_poShmData->m_isCrossGs = cross; }

	void SetSessionID(SessionType sessionID) { m_poShmData->m_sessionID = sessionID; }
	void SetAccount(const std::string& str)	 { m_accountID = str; }
	void SetRoleID(UINT64 qwID)			{ m_poShmData->m_qwID = qwID; }
	UINT32 GetServerID() { return m_poShmData->m_dwServerId; }
	void SetName(const std::string& str){ m_strName = str; }
	void SetState(ERoleState nState)	{ m_poShmData->m_nState = nState; }
	void SetGsLine(UINT32 dwLine)		{ m_poShmData->m_dwGsLine = dwLine; }
	void SetMapID(UINT32 dwMapID)		{ m_poShmData->m_dwMapID = dwMapID; }
	void SetSceneID(UINT32 dwSceneID)	{ m_poShmData->m_dwSceneID = dwSceneID; }
	void SetIsWatcher(bool value) { m_poShmData->m_isWatcher = value; }

	UINT32 GetDestGsLine() { return m_poShmData->m_dwDestGsLine; }
	UINT32 GetDestOwnerID() { return m_poShmData->m_dwDestOwnerID; }
	UINT32 GetDestMapID() { return m_poShmData->m_dwDestMapID; }
	UINT32 GetDestSceneID() { return m_poShmData->m_dwDestSceneID; }
	bool GetDestIsCross() { return m_poShmData->m_bDestIsCross; }
	bool GetDestIsWatcher() { return m_poShmData->m_DestIsWatcher; }
	KKSG::SceneSwitchData& GetSwitchData() { return m_oSwitchData; }

	void SetDestGsLine(UINT32 value) { m_poShmData->m_dwDestGsLine = value; }
	void SetDestOwnerID(UINT32 value) { m_poShmData->m_dwDestOwnerID = value; }
	void SetDestMapID(UINT32 value) { m_poShmData->m_dwDestMapID = value; }
	void SetDestSceneID(UINT32 value) { m_poShmData->m_dwDestSceneID = value; }
	void SetDestIsCross(bool value) { m_poShmData->m_bDestIsCross = value; }
	void SetDestIsWatcher(bool value) { m_poShmData->m_DestIsWatcher = value; }

	void ClearSwitchInfo() { m_poShmData->ClearSwitch(); }

	void OnChangeScene();
	void OnLogin();
	void OnLogout();

	void Send(const CProtocol &roPtc);
	void SendMsgToGS(const CProtocol& msg);
	void SendMsgToGS(CRpc& msg);
	
	bool CanLoginReconnect();
private:
	CShmRole* m_poShmData;

	std::string m_accountID;		//tencent openid
	std::string	m_strName;

	KKSG::SceneSwitchData m_oSwitchData;
};

#endif