#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "cmdline.h"
#include "commondef.h"

class ITimer;
class Scene;
class IEnterSceneCondition;
class Role;
class SceneSecurityZone;
struct SceneInfo;

namespace KKSG
{
	class SceneSwitchData;
	class CreateBattleParam;
}

class SceneManager
{
	SceneManager();
	~SceneManager();
	DECLARE_SINGLETON(SceneManager)

public:
	bool Init();
	void Uninit();

	void Update(float deltaTime);
	void CheckScene();

	void AddScene(Scene *scene);

	///> 玩家切换场景
	bool ChangeScene(Role *poRole, UINT32 toSceneTemplateID, UINT64 ownerID = 0,UINT32 abyssID=0);
	//bool ChangeScene(Role *poRole, Scene *pScene);

	///> Find一个静态场景或创建一个动态场景 (指定InstanceID)
	Scene* FindOrCreateScene(UINT32 dwSceneTemplateID, Role *poRole, const KKSG::SceneSwitchData& roSwitchData, UINT32 dwSceneInstanceID);
	///> 新的创建副本接口
	Scene* CreateScene(UINT32 dwSceneTemplateID, UINT32 dwSceneInstanceID, const KKSG::CreateBattleParam& roParam);

	int  CheckEnterSceneCondition(Role *poRole, SceneInfo *poInfo, UINT32* arg = NULL, UINT64 ownerID = 0);
	int  CheckWatcherEnterSceneCondition(Role *poRole, SceneInfo *poInfo);

	void TakeEnterSceneCost(Role *poRole, SceneInfo *poInfo, bool iswin = false);

	Scene* FindStaticScene(UINT32 sceneTemplateID);
	void FindByTemplateID(UINT32 sceneTemplateID, std::vector<Scene*>& scenes);
	Scene* FindBySceneID(UINT32 sceneUID);

	void DumpScene(std::string &outputMessage);
	void EnterScene(Role *poRole, Scene *pScene, bool isWatchRole = false);

	SceneSecurityZone *GetSecurityZone() { return m_pSecurityZone; }

	UINT32 GetHallRoleCount();
	UINT32 GetSceneCount() { return m_AllScenes.size(); }

	static Scene *CreateScene(SceneInfo *poInfo, UINT32 dwSceneInstanceID = INVALID_SCENE_ID);
	static Scene *CreateScene(SceneInfo *poInfo, UINT64 ownerID, UINT32 dwSceneInstanceID = INVALID_SCENE_ID);
	static void CreateStaticScene(UINT32 sceneID, SceneInfo *pSceneInfo, void *);

public:
	typedef std::unordered_map<UINT32, Scene*>::iterator scene_iter;
	inline scene_iter Begin()	{ return m_AllScenes.begin(); }
	inline scene_iter End()	{ return m_AllScenes.end(); }

private:
	void RemoveScene(Scene *scene);

	Scene* FindorCreateScene(SceneInfo *poInfo, Role *poRole, UINT64 ownerID = 0, UINT32 dwSceneInstanceID = INVALID_SCENE_ID);

	static bool CmdLineScene(const CmdLineArgs &args, std::string &outputMessage);
private:
	ITimer *m_CheckSceneTimer;
	///> 场景进入条件限制
	std::list<IEnterSceneCondition*> m_EnterCondtions;

	///> 公会大厅场景
	std::map<UINT64, Scene*> m_allGuildScene;

	///> 静态地图（服务器启动时生成，大厅）
	std::list<Scene*> m_listStaticScene;
	///> 所有生成的场景实例，键值是随机生成的场景唯一id（单人PVE+大厅+多人PVE+多人PVP）
	std::unordered_map<UINT32, Scene*> m_AllScenes;

	SceneSecurityZone *m_pSecurityZone;
};

#endif // __SCENEMANAGER_H__