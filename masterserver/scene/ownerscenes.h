#ifndef __OWNERSCENES_H__
#define __OWNERSCENES_H__

#include "timer.h"
#include "time.h"
#include "scene/scenecreate.h"

enum OwnSceneState
{
	OwnScene_Normal,
	OwnScene_PreClose,
};

struct OneOwnerScene
{
	OwnSceneState state;	
	UINT32 delayclose;
	UINT32 sceneid;
	std::vector<UINT64> prepares;

	void ResetState()
	{
		state = OwnScene_Normal;
		delayclose = (UINT32)(-1);
	}

	void SetClose()
	{
		state = OwnScene_PreClose;
		delayclose = (UINT32)time(NULL) + 300; 
	}

	OneOwnerScene()
	{
		state = OwnScene_Normal;
		delayclose = (UINT32)(-1);
		sceneid = 0;
	}
};

class OwnerScene : public ICreateBattleListener, public ITimer
{
public:
	OwnerScene();
	~OwnerScene();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	OneOwnerScene* FindScene(UINT64 uid);
	//bool HasPrepareRole(UINT64 uid);
	//void PushPrepareRole(UINT64 uid, UINT64 roleid);

	void RemoveScene(UINT32 sceneuid);

	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

public:
	HTIMER m_timehandler;
	std::map<UINT32, UINT64> m_sceneid2uid;
	std::unordered_map<UINT64, OneOwnerScene> m_uid2scene;
};

class OwnerScenes
{
public:
	OwnerScenes();
	~OwnerScenes();

	OneOwnerScene* FindScene(UINT32 type, UINT64 uid);
	OneOwnerScene* FindSceneBySceneId(UINT32 type, UINT32 sceneid);
	void EnterScene(UINT32 type, UINT64 uid, UINT64 roleid);

	void RemoveScene(UINT32 scenetype, UINT32 sceneuid);
	void RecoverScene(UINT64 ownerid, UINT32 mapid, UINT32 sceneid);

	UINT32 GetSceneCount(UINT32 mapid);

private:
	std::map<UINT32, OwnerScene*> m_type2scenes;
};

#endif