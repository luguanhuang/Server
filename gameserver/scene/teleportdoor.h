#ifndef _TELEPORTDOOR_H_
#define _TELEPORTDOOR_H_

#include "math/XVector3.h"
#include <set>

class Scene;
class Role;

class TeleportBase
{
public:	
	TeleportBase(const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene);
	virtual ~TeleportBase();

	virtual void Update() = 0;
	virtual bool IsFinished() = 0;
	virtual void SetLevelCallBack(int callbackNo) = 0;

	void SetLevelScriptFunc(std::string& func);

protected:
	bool InTeleportDoor(const Vector3& pos);

	Vector3 m_telePos;//传送点位置
	float m_teleR;//覆盖半径
	Vector3 m_desPos;//目标位置
	float m_desFace;//传送后的朝向

	bool m_isRunning;//状态

	Scene* m_scene;

	int m_levelCallBackNo;

	std::string m_levelScriptFunc;
};

class TeleportDoor : public TeleportBase
{
public:
	TeleportDoor(const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene);
	void Update();
	bool IsFinished();

	void SetLevelCallBack(int callbackNo);

private:

	bool CheckCondition();
	void DoTeleport();

	void OnSceneTrans();
	
	void CheckNotice();
	void EndNotice();

	void OnFinish();

	std::set<UINT64> m_noticeHistory;

	time_t m_deadTime;//存在时间限制
};

class TeleportOne : public TeleportBase
{
public:
	TeleportOne(const Vector3& telePos , float teleR , const Vector3& desPos , float desFace , Scene* scene);
	void Update();
	bool IsFinished();
	void SetLevelCallBack(int callbackNo){}

private:
	void DoTeleport(Role* pRole);

	void AddDelay(UINT64 roleID);
	bool InDelay(UINT64 roleID);
	void CheckDelay();
	
	std::unordered_map<UINT64, time_t> m_delay;	
};

#endif
