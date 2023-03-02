#ifndef __SERVERCROSSDATAMGR_H__
#define __SERVERCROSSDATAMGR_H__

#include "serverinfo/servercrossdatamgrbase.h"

class ServerCrossDataSyn
{
public:
	static void SynDataToWorld(UINT32 connID = 0, int synType = SCDST_ALL);
	static bool IsDragonSceneFininshed(UINT32 sceneID);

private:
	static void FillData(KKSG::ServerCrossData& data);
	static void FillTeamData(KKSG::ServerCrossTeamData& teamdata);

private:
	static KKSG::ServerCrossData m_lastSynData;
};

#endif