#ifndef _SCENE_WEDDING_H_
#define _SCENE_WEDDING_H_

#include "scenecb.h"
#include "timer.h"
#include "util/gametime.h"
#include "math/XVector3.h"
#include "timeutil.h"

struct WeddingRoleData
{
	WeddingRoleData(): roleId(0), lastFlowTime(0), lastFireworksTime(0), applyVows(false), applyVowsTime(0) {} 
	UINT64 roleId;
	UINT32 lastFlowTime;
	UINT32 lastFireworksTime;
	bool applyVows;
	UINT32 applyVowsTime;

	bool IsApplyVows()
	{
		if (applyVows == false)
		{
			return false;
		}
		if (TimeUtil::GetTime() < applyVowsTime + 60)
		{
			return true;
		}
		applyVows = false;
		return false;
	}
};

class Role;
class Scene;
class SceneWedding : public ISceneEventListener, public ITimer
{
public:
	SceneWedding(Scene* scene);
	~SceneWedding();

	void Init(const KKSG::WeddingCreateInfo& info);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void UpdateState();
	void UpdateHappyness();
	void UpdateVows();

	virtual void OnPreEnterScene(Scene *pScene, Role* role);
	virtual void OnEnterSceneFinally(Scene *pScene, Role* role);
	virtual void OnRoleReconnect(Role* role);
	virtual void OnLeaveScene(Scene *pScene, Role* role);
	virtual void OnAddNonRole(Scene *pScene, Unit* unit);

	void OnEnd();
	void PrepareVows();
	void DoVows();
	void DoVowsEnd();

	bool IsOwner(UINT64 roleId);
	WeddingRoleData* GetRoleData(UINT64 roleId);
	void AddHappyness(UINT32 value);

	KKSG::ErrorCode DoFlower(Role* pRole);
	KKSG::ErrorCode DoFireworks(Role* pRole);
	KKSG::ErrorCode DoApplyVows(Role* pRole);
	KKSG::ErrorCode DoAgreeVows(Role* pRole);
	KKSG::ErrorCode DoDisagreeVows(Role* pRole);

	UINT32 GetStateLeftTime();
	void GMSetStateEnd();

	void PushLoadingInfo(Role* pRole);
	void PushWeddingState(Role* pRole = NULL);
	void PushWeddingEvent(Role* pEventRole, KKSG::WeddingOperType type, Role* pRole = NULL);
	void PushWeddingStateToMs();

	void GiveFlowerReward();
	void GiveFireworksReward();
	void SendMail();

private:
	Scene* m_pScene;
	HTIMER m_timer;
	KKSG::WeddingBrief m_brief;
	UINT64 m_weddingId;
	KKSG::WeddingType m_type;
	UINT64 m_roleId1;
	UINT64 m_roleId2;

	bool m_isEnd;
	KKSG::WeddingState m_state;
	UINT32 m_stateStartTime;
	UINT32 m_happyness;
	UINT32 m_lastHappynessTime;
	std::map<UINT64, WeddingRoleData> m_data;
	bool m_prepareVows;
	bool m_vows;
	bool m_vowsEnd;
	UINT32 m_vowsTime;
	UINT32 m_vowsEndTime;
	Vector3 m_rolePos1;
	Vector3 m_rolePos2;
	float m_roleFace1;
	float m_roleFace2;
	UINT32 m_lastPushStateTime;
};

#endif