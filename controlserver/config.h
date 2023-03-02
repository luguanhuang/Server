#ifndef __CONTROL_CONFIG_H__
#define __CONTROL_CONFIG_H__

#include "baseconfig.h"
#include "pb/enum.pb.h"
#include "DBInfo.h"
#include "version/version.h"


class IMysqlConn;
class ControlConfig : public Config
{
	ControlConfig();
	~ControlConfig();
	DECLARE_SINGLETON(ControlConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *root);


	DBInfo&		GetDBInfo() { return m_DBInfo; }
	Version&	GetVersion()	{return m_oVersion;}

	bool UseWhiteList() const { return m_bUseWhiteList; }
	void SetUseWhiteList(bool bUse) { m_bUseWhiteList = bUse; }

	KKSG::PlatType		GetPlatType() const { return m_nPlatType; }
	KKSG::GameAppType	GetAppType()  const { return m_nAppType; }
	UINT32 GetChunkNum(const std::string& name, UINT32 defaultValue = 20000);
	UINT32 GetShmKeyOffset(const std::string& name);
	std::set<UINT32>& GetServerIds() {return m_serverIds;}
	bool IsValidServerId(UINT32 serverId);
	bool IsBackFlowServer() { return m_isBackFlowServer; }
	UINT32 GetBackFlowLevel() { return m_backFlowLevel; }
	bool CanLoginReconnect() { return m_enableReconnect; }

	bool LoadUseWhiteList();
	void LoadShmKeyOffset();
	void LoadReconnect();

	bool InitFromDB();
	bool LoadServerIds(IMysqlConn* pMysql);
	bool LoadBackFlow(IMysqlConn* pMysql);

private:
	DBInfo		m_DBInfo;

	Version		m_oVersion;

	bool		m_bUseWhiteList;
	bool		m_enableReconnect;

	KKSG::PlatType		m_nPlatType;
	KKSG::GameAppType	m_nAppType;
	std::map<std::string, UINT32> m_chunkNum;
	std::map<std::string, UINT32> m_shmOffset;
	std::set<UINT32>			m_serverIds;

	bool m_isBackFlowServer;				// 是否回流服务器
	UINT32 m_backFlowLevel;					// 回流等级
};

#endif