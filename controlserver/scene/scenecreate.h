#ifndef _H_Scene_Create_H__
#define _H_Scene_Create_H__


#include "pb/project.pb.h"

class CScene;
class ICreateBattleListener
{
public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res) = 0;
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
	bool CreateBattleScene(UINT32 dwSceneTemplateID, ICreateBattleListener* poListener, const KKSG::CreateBattleParam& roParam);

	void OnCreateBattleReply(UINT64 qwUid, CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
private:
	UINT64 NewID();

	typedef std::map<UINT64, ICreateBattleListener*>	CListenerMap;

	UINT64			m_qwUID;
	CListenerMap	m_oMap;
};

#endif