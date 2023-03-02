#ifndef __DBCONFIG_H__
#define __DBCONFIG_H__

#include "baseconfig.h"
#include "timer.h"


struct DBInfo {
	std::string m_ip;
	std::string m_user;
	std::string m_pswd;
	std::string m_database;
	std::string m_threads;
	std::string m_port;
};

class DBConfig : public ITimer, public Config
{
	DBConfig();
	~DBConfig();
	DECLARE_SINGLETON(DBConfig)

public:

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	DBInfo     &GetDBInfo();
	DBInfo     &GetOnlineDBInfo();
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	
	virtual int GetSocketSendBufferSize() { return (SOCKET_SEND_BUFF_SIZE * 4); }
	virtual int GetSocketRecvBufferSize() { return SOCKET_RECV_BUFF_SIZE * 4; }

	bool AutoCreateDB() const { return m_bAutoCreateDB; }
	bool IsLoginEnabled() const { return m_bLoginEnabled; }

	UINT32 GetMaxRegisterNum() const { return m_dwMaxRegisterNum; }
	UINT32 GetCurRegisterNum() const { return m_dwCurRegisterNum; }

	void IncCurRegisterNum() { ++m_dwCurRegisterNum; }
	void DesCurRegisterNum() { m_dwCurRegisterNum = (m_dwCurRegisterNum > 0) ? m_dwCurRegisterNum - 1 : m_dwCurRegisterNum; }
	void SetCurRegisterNum(UINT32 dwNum) { m_dwCurRegisterNum = dwNum; }
	bool IsRegisterNumReachMax() const { return m_dwCurRegisterNum >= m_dwMaxRegisterNum; }

	bool ReadLoinLimitFile();
	bool ReLoadLoinLimitFile();
	bool ReLoadMaxRegisterNum();

	bool LoadMysqlReconFile();
	UINT32 GetReconTryInterval() const { return m_dwTryInterval; }
	UINT32 GetReconMaxTryTimes() const { return m_dwMaxTryTimes; }
private:
	DBInfo              m_DBInfo;
	DBInfo				m_OnlineDBInfo;
	bool				m_bAutoCreateDB;
	bool				m_bLoginEnabled;
	UINT32				m_dwMaxRegisterNum;
	UINT32				m_dwCurRegisterNum;

	UINT32				m_dwTryInterval;
	UINT32				m_dwMaxTryTimes;
};

#endif