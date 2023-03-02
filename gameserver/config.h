#ifndef __GSCONFIG_H__
#define __GSCONFIG_H__

#include "baseconfig.h"
#include "commondef.h"
#include "pb/enum.pb.h"
#include "DBInfo.h"

enum
{
	Version_None,
	Version_iOS,
	Version_Android,
	Version_PC,
	Version_All
};

struct Version
{
	std::string ios;
	std::string android;
};

class IMysqlConn;

class GSConfig : public Config
{
	GSConfig();
	~GSConfig();
	DECLARE_SINGLETON(GSConfig)

	virtual int GetSocketSendBufferSize() { return (SOCKET_SEND_BUFF_SIZE * 4); }
	virtual int GetSocketRecvBufferSize() { return SOCKET_RECV_BUFF_SIZE * 4; }

	int GetGameServerOpenDays();
	time_t GetGameServerOpenTimeStamp() { return m_OpenTimeStamp; }
	void SetGameServerOpenTimeStamp(std::string str);
	void SetGameServerOpenTimeStamp(UINT32 dwTime) { m_OpenTimeStamp = dwTime; }

	const std::string &GetServerInfo() { return m_ServerInfo; }
	virtual bool ReadSettings(tinyxml2::XMLElement *element);

	bool InitFromDB();
	bool LoadServerIds(IMysqlConn* pMysql);
	
	bool IsDebug() { return m_isdebug; }
	void SetDebug(bool isdebug) { m_isdebug = isdebug; }

	bool IsCrossGS() { return m_isCross; }
	
	float GetFrameTime() { return m_frametime; }

	bool IsHallGS() { return m_dwLine == MAIN_HALL_GS_LINE && !IsCrossGS(); }
	UINT32 GetLine() const { return m_dwLine; }
	const std::string& GetLocalIP() const { return m_strLocalIP; }
	const std::string& GetSdkUrl() const { return m_strSdkUrl; }
	const std::string& GetAntiAddictionUrl() const { return m_strAntiAddictionUrl; }
	const std::string& GetDataMoreUrl() const { return m_strDataMoreUrl;}
	const std::string& GetGameWeiXinUrl() const { return  m_strGameWeiXinUrl;}

	DBInfo&		GetDBInfo() { return m_DBInfo; }

	void SetAppType(KKSG::GameAppType nType){ m_nAppType = nType; }
	KKSG::GameAppType	GetAppType()  const { return m_nAppType; }
	void SetPlatType(KKSG::PlatType nType){ m_nPlatType = nType; }
	KKSG::PlatType GetPlatType() const { return m_nPlatType; }

	void SetCombineServerTime(UINT32 dwTime) { m_combineServerTime = dwTime; }
	time_t GetCombineServerTime() const { return m_combineServerTime; }

	bool IsBackFlowServer() { return m_isBackFlowServer; }
	UINT32 GetBackFlowLevel() { return m_backFlowLevel; }
	void SetBackFlowLevel(UINT32 level);

	bool CanDoBackFlowInit(UINT32 roleNum);

	bool IsServerIDIn(UINT32 serverID);

private:
	float m_frametime;
	bool m_isdebug;
	bool m_isCross;			// 是否跨服区GS
	Version m_version;
	time_t m_OpenTimeStamp;				//Ms传入
	UINT32		m_dwLine;
	std::string m_ServerInfo;
	std::string	m_strLocalIP;
	DBInfo      m_DBInfo;
	KKSG::GameAppType	m_nAppType;		//Ms传入
	std::string m_strSdkUrl;
	std::string m_strAntiAddictionUrl;//防沉迷上报url;
	KKSG::PlatType m_nPlatType;//ms传入;
	std::string m_strDataMoreUrl;
	std::string m_strGameWeiXinUrl;
	time_t m_combineServerTime;
	std::set<UINT32> m_combineServerIDs;

	bool m_isBackFlowServer;				// 是否回流服务器
	UINT32 m_backFlowLevel;					// 回流等级
};


//检查是否是HALL_GS
#define CHECK_HALL_GS if(!GSConfig::Instance()->IsHallGS()) return;
#define CHECK_HALL_GS_RETURN(returnValue) if(!GSConfig::Instance()->IsHallGS()) return returnValue;


#endif