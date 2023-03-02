#ifndef __GSCONFIG_H__
#define __GSCONFIG_H__

#include "baseconfig.h"
#include "version/version.h"
#include "pb/enum.pb.h"
#include "DBInfo.h"

enum DataClearType
{
	DataClear_Summary,
	DataClear_Friend,
	DataClear_Garden,
	DataClear_Dummy,
};


class IMysqlConn;
class MSConfig : public Config
{
	MSConfig();
	~MSConfig();
	DECLARE_SINGLETON(MSConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *root);

	DBInfo&		GetDBInfo() { return m_DBInfo; }
	UINT32		GetGameServerOpenTimeStamp() const { return (UINT32)m_OpenTimeStamp; }
	void SetGameServerOpenTimeStamp(time_t time) { m_OpenTimeStamp = time; }

	bool IsDebug() { return m_bDebug; }
	void SetDebug(bool isDebug) { m_bDebug = isDebug; }


	Version* GetVersion() { return &m_oVersion; }
	const std::string &GetServerInfo() { return m_ServerInfo; }
	const std::string& GetName() { return m_serverName; }
	void SetName(const std::string& name) { m_serverName = name; }
	const std::string& GetZoneName() { return m_zoneName; }
	void SetZoneName(const std::string& name) { m_zoneName = name; }

	bool LoadVersionFile();
	bool HotLoadVersionFile();
	static void SyncVersion2Control();

	KKSG::PlatType		GetPlatType() const { return m_nPlatType; }
	KKSG::GameAppType	GetAppType()  const { return m_nAppType; }
	KKSG::ServerTag		GetServerTag()  const { return m_nServerTag; }
	bool GetFMFlag(){ return m_bFM;}
	const std::set<UINT32>& GetServerIds() {return m_serverIds;}
	bool HasServerId(UINT32 id) { return m_serverIds.find(id) != m_serverIds.end(); }


	bool Reload();
	const std::string& GetMsdkUrl(){return m_strMsdkUrl;}
	bool GetPayDebug() { return m_bPayDebug;}
	UINT32 GetPayErrNum() { return m_PayNum;}
	bool GetPayDelayTest() { return m_bPayDelayTest;}
	std::string GetMidasPayUrl(bool isMaster=true);
	const std::string& GetBuyGoodsUrl() { return m_strBuyGoodsUrl; }
	const std::string& GetXingeUrl() { return m_xingeUrl; }
	UINT32 GetShmKeyOffset(const std::string& name);
	bool IsDataClear(DataClearType type);

	bool IsBackFlowServer() { return m_isBackFlowServer; }
	UINT32 GetBackFlowLevel() { return m_backFlowLevel; }
	void SetBackFlowLevel(UINT32 level);

private:
	void _UpdateVersionType(Version& tempVersion);
	KKSG::GameAppType _ReadAppType(const char* pcApp);
	KKSG::PlatType _ReadPlatType(const char* pcPlat);
	void _AfterReloadMSConfig();
	void LoadShmKeyOffset();

	bool InitFromDB();
	bool LoadServerIds(IMysqlConn* pMysql);
	bool LoadBackFlow(IMysqlConn* pMysql);
	void LoadDataClear();

private:
	DBInfo     m_DBInfo;
	time_t	   m_OpenTimeStamp;

	bool		m_bDebug;
	Version		m_oVersion;
	std::string m_ServerInfo;
	std::string m_serverName;
	std::string m_zoneName;

	KKSG::PlatType		m_nPlatType;
	KKSG::GameAppType	m_nAppType;
	KKSG::ServerTag		m_nServerTag;

	std::string m_strPayUrl;
	std::string m_strPaySlaveUrl;
	std::string m_strMsdkUrl;
	std::string m_strBuyGoodsUrl;
	std::string m_strBuyGoodsSlaveUrl;
	bool m_bPayDelayTest;//充值延迟到账测试;
	UINT32 m_uPollID;//轮询ID;
	bool m_bFM;
	bool m_bPayDebug;//是否为沙箱或者正式环境;
	INT32 m_PayNum;//与midas连接异常连续次数;

	std::string m_xingeUrl;
	std::map<std::string, UINT32> m_shmOffset;
	std::set<UINT32>			m_serverIds;

	bool m_iscleardata[DataClear_Dummy];

	bool m_isBackFlowServer;				// 是否回流服务器
	UINT32 m_backFlowLevel;					// 回流等级
};

#endif