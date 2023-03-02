#ifndef __SCENEPREPARELIST_H__
#define __SCENEPREPARELIST_H__

#include <time.h>
#include "math/XVector3.h"
#include "table/globalconfig.h"

#define PREPARE_TIMEOUT GetGlobalConfig().SceneWaitTimeout

enum LoadingState
{
	Loading,
	Loaded,
	EnterState,
};

class Role;
class Scene;
struct ClientLoadedInfo
{
	Role* role;
	Scene* scene;

	ClientLoadedInfo()
	{
		role = NULL;
		scene = NULL;
	}
};

struct ClientLoadingInfo
{
	bool iswatcher;
	UINT64 qwRoleID;
	std::string  strRoleName;
	LoadingState eState;
	time_t time;
	UINT32 delayid;

	ClientLoadingInfo()
	{
		iswatcher = false;
		qwRoleID = 0;
		eState = Loading;
		time = 0;
		delayid = 0;
	}
};

class IScenePrepareListener
{
public:
	virtual ~IScenePrepareListener(){}
	virtual void OnEnterSceneFinish() = 0;
	virtual bool OnClientLoaded(UINT64 qwRoleID, UINT32 delayid) = 0;
};

struct SceneInfo;
class IScenePrepareList
{
public:
	IScenePrepareList() { m_pListener = NULL; m_Name = NULL; m_isenterscenefinish = false;}
	virtual ~IScenePrepareList() {}

	virtual void Update() {}
	virtual void Clear () {}
	virtual void Reset() {}
	virtual std::vector<std::string> GetNames() = 0;
	virtual bool ChangeRoleState(UINT64 qwRoleID, LoadingState eNewState, UINT32 delayid) = 0;
	virtual void AddPrepareRole(UINT64 qwRoleID, const std::string &RoleName, bool iswatcher);
	virtual bool RemovePrepareRole(UINT64 qwRoleID);

	ClientLoadingInfo* GetLoadingInfo(UINT64 roleid); 

	void SetListener(IScenePrepareListener *l) { m_pListener = l; }

	static IScenePrepareList *CreateScenePrepareList(SceneInfo *pSceneInfo);
	static IScenePrepareList *CreateSoloPrepareList();

protected:
	bool m_isenterscenefinish;
	const char *m_Name;
	IScenePrepareListener *m_pListener;
	std::map<UINT64, ClientLoadingInfo> m_prepareClients;
};

#endif // __SCENEPREPARELIST_H__