#ifndef _H_Scene_Create_H__
#define _H_Scene_Create_H__

#include "commondef.h"
#include "pb/project.pb.h"

class CScene;
class ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res) = 0;
};

class ICreateTestListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateTestListener GlobalCreateTestListener;
};

class ICreateGMFBattleListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateGMFBattleListener GlobalCreateGMFBattleListener;
};

class ICreateSkyCityListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateSkyCityListener GlobalCreateSkyCityListener;
};

class ICreateSkyCityWaitListener :public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateSkyCityWaitListener GlobalCreateSkyCityWaitListener;
};


class ICreateResWarListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateResWarListener GlobalCreateResWarListener;
};

class ICreateInvFightListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateInvFightListener GlobalCreateInvFightListener;
};

class ICreateHeroBattleListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateHeroBattleListener GlobalCreateHeroBattleListener;
};

class ICreateGuildCastleListner : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateGuildCastleListner GlobalCreateGuildCastleListener;
};

class ICreatePkListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreatePkListener GlobalCreatePkListener;
};

class ICreateMobaBattleListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateMobaBattleListener GlobalCreateMobaBattleListener;
};


class ICreateBattleFieldReadyListener : public ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
	static ICreateBattleFieldReadyListener GlobalCreateBattleFieldReadyListener;
};





class CSceneCreator
{
	CSceneCreator();
	virtual ~CSceneCreator();
	DECLARE_SINGLETON(CSceneCreator)
public:
	bool Init();
	void Uninit();

	//创建副本
	bool CreateBattleScene(UINT32 dwSceneTemplateID, ICreateBattleListener* poListener, const KKSG::CreateBattleParam& roParam, UINT32 destline = INVALID_LINE_ID, UINT32 createType = 0);

	void OnCreateBattleReply(UINT64 qwUid, CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
private:
	UINT64 NewID();

	typedef std::map<UINT64, ICreateBattleListener*>	CListenerMap;

	UINT64			m_qwUID;
	CListenerMap	m_oMap;
};

#endif