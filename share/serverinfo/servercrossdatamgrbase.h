#ifndef __SERVERCROSSDATAMGRBASE_H__
#define __SERVERCROSSDATAMGRBASE_H__

#include "pb/project.pb.h"

enum ServerCrossDataSynType
{
	SCDST_ALL = 0,
	SCDST_ONLY_TS = 1,
	SCDST_ONLY_CROSS_GS = 2,
	SCDST_ONLY_WS = 3,
};

class ServerCrossDataMgrBase
{
public:
	ServerCrossDataMgrBase() {}
	virtual ~ServerCrossDataMgrBase() {}

	bool Init();
	void Uninit();

	void AddData(const KKSG::ServerCrossData& data);
	//²»×öÉ¾³ý
	void DelData(UINT32 serverID);
	virtual KKSG::ServerCrossData* GetData(UINT32 serverID);

	void InfoPrint();
	static void InfoPrint(const KKSG::ServerCrossData& data);
	static void InfoPrint(const KKSG::ServerCrossTeamData& data);

	static bool IsDragonSceneFinished(const KKSG::ServerCrossTeamData& data, UINT32 sceneID);
	bool IsServerDragonSceneFinished(UINT32 serverID, UINT32 sceneID);

protected:
	std::unordered_map<UINT32, KKSG::ServerCrossData> m_serverData;
};

#endif
