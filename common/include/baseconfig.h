#ifndef __BASECONFIG_H__
#define __BASECONFIG_H__

#include <string>
#include <list>
#include "tinyxml2.h"
#include "define.h"
#include "network.h"


class BaseLink;
class INetWork;

class Config
{
public:
	Config();
	virtual ~Config();

	bool        Init(const char *config);
	void        Uninit();

	void        ProcessNetMessage();
	BaseLink   *FindLinkHandler(const std::string &linkName);
	
	std::string GetFilePath(const char *filePath);
	std::string ReadFileContent(const char *filePath, const char *mode="rb");

	void SetServerTag(const char *serverTag) { m_ServerTag = serverTag; }

	UINT32			   GetZoneID();
	UINT32             GetServerID();
	const std::string& GetServerName();
	const std::string& GetConfFile() const { return m_strConfFile; }

	virtual int GetTimerWarningInterval() { return 10; }
	virtual int GetSocketSendBufferSize() { return SOCKET_SEND_BUFF_SIZE; }
	virtual int GetSocketRecvBufferSize() { return SOCKET_RECV_BUFF_SIZE; }
	virtual int GetProtocolWarningSize()  { return 1048576; }
	virtual int GetCompressThredhold() { return 5000; }
	virtual int GetReconnectInterval() { return 2000; }

	static Config *GetConfig();

protected:
	bool        InitNetwork();
	void        AddLinkHandler(BaseLink *link);
	bool        LoadServerNode(tinyxml2::XMLElement *element);
	virtual     bool ReadSettings(tinyxml2::XMLElement *element);
	tinyxml2::XMLElement * FindServerNode(tinyxml2::XMLDocument &doc, const char *config);

private:

	std::list<BaseLink*> m_LinkHandler;
	INetWork            *m_Network;
	std::string          m_RootPath;
	std::string          m_ServerName;
	UINT32               m_ServerID;
	UINT32               m_ZoneID;
	const char *         m_ServerTag;
	std::string			 m_strConfFile;
};

#endif