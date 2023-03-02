#ifndef __SCENECASTLE_H__
#define __SCENECASTLE_H__

#include "scenevsbase.h"
#include "pb/project.pb.h"
#include "vshandler.h"

class Scene;
class SceneCastle : public SceneVsBase
{
public:
	SceneCastle(Scene* scene);
	~SceneCastle();

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT64 GetFightGroupUId(UINT64 roleid);
	virtual void InitState();
	void InitJvDian();
	void AddJvdian(const VsJvDian& jvdian);

	inline void SetFightEnd() { m_isFightEnd = true; } 
	inline bool IsFightEnd() { return m_isFightEnd; }

	virtual VsStateType CheckFightEnd();

	virtual void OnStateEnd();

	void OnMultiPoint(UINT32 mapID, UINT32 multi);
	void OnKill(UINT64 roleID, UINT64 deadID);
	void MakeOccupy(UINT64 roleID, KKSG::GCFJvDianType type);
	void BackToReady(UINT64 roleID);
	void OnLeave(UINT64 roleID);

	void AllCountChange(UINT64 roleID, UINT32 count);
	void AllCountToMe(UINT64 roleID);
	void AllCountToOther(UINT64 roleID);

	void OnOccupyState(UINT64 roleID, int type, int state);

private:

	//³¡¾°ÊÂ¼þ¼àÌý
	virtual bool OnEvent(const VsEvent& event);
	//OnTick
	virtual void Update();
	virtual UINT32 GetUpInterval() { return 0; }

private:
	inline bool IsWaitS() { return m_data.iswait(); }
	void UpdateDeadRole();
	void UpdateJvDian();

	bool m_isFightEnd;

	KKSG::GCFCreateBattleInfo m_data;

	std::unordered_map<UINT64, time_t> m_deadRoles;

	std::vector<VsJvDian> m_jvDians;

	std::unordered_map<UINT64, UINT32> m_killCount;
};

#endif