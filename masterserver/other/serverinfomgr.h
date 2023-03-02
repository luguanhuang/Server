#ifndef __SERVERINFOMGR_H__
#define __SERVERINFOMGR_H__

#include "pb/project.pb.h"
#include "cmdline.h"

//整个大区的所有服
class ZServerInfoMgr
{
	ZServerInfoMgr();
	~ZServerInfoMgr();
	DECLARE_SINGLETON(ZServerInfoMgr)

public:
	bool Init();
	void Uninit();

	void AddServerInfo(const KKSG::ServerInfo& data);	
	const KKSG::ServerInfo* GetServerInfo(UINT32 serverID);

	std::string GetServerName(UINT32 serverID);
	std::string GetZoneName(UINT32 serverID);
	std::string GetCurServerName();

	void TestPrint();

private:

	void Print(const KKSG::ServerInfo& data);

	std::unordered_map<UINT32, KKSG::ServerInfo> m_serverInfo;

};

#endif