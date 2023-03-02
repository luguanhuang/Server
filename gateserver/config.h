#ifndef __GSCONFIG_H__
#define __GSCONFIG_H__

#include "baseconfig.h"
#include "version/version.h"
#include "timer.h"

//struct QueryInfo
//{
//	int port;
//	int normal;
//	int full;
//	int isNew;
//};

class GateConfig : public Config
{
	GateConfig();
	~GateConfig();
	DECLARE_SINGLETON(GateConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	//const QueryInfo &GetQueryInfo() { return m_qInfo; }

	// µ•Œª: √Î
	int GetReconnectTimeout() { return m_nReconnectTimeout; }


	UINT32 GetLine() const { return m_dwLine; }
	const std::string& GetGsListenIP() const { return m_strGsListenIP; }
	UINT32 GetGsListenPort() const { return m_dwGsListenPort; }

	//Version& GetVersion() { return m_oVersion; }

	bool	IsCompress() const	{ return m_bCompress; }
	bool	IsEncrypt() const	{ return m_bEncrypt; }
	UINT32	GetMinCompressSize() const { return m_dwMinCompressSize; }

	UINT32 GetNewConnPerSecond() { return m_newConnPerSecond; }
	UINT32 GetTpsPerConn() { return m_tpsPerConn; }
	UINT32 GetMaxConnNum() { return m_maxConnection; }


	void SetAppType(KKSG::GameAppType type){m_nAppType = type;}
	void SetPlatType(KKSG::PlatType type){m_nPlatType = type;}
	KKSG::GameAppType GetAppType(){return m_nAppType;}
	KKSG::PlatType GetPlatType(){return m_nPlatType;}

private:
	//QueryInfo m_qInfo;
	int			m_nReconnectTimeout;
	UINT32		m_dwLine;
	std::string	m_strGsListenIP;
	UINT32		m_dwGsListenPort;

	//Version		m_oVersion;		//set by ms

	KKSG::PlatType		m_nPlatType;
	KKSG::GameAppType	m_nAppType;

	bool		m_bCompress;
	bool		m_bEncrypt;
	UINT32		m_dwMinCompressSize;
	UINT32		m_newConnPerSecond;
	UINT32		m_tpsPerConn;
	UINT32		m_maxConnection;
};

#endif