#ifndef _WEDDING_H_
#define _WEDDING_H_

class CRole;
class Wedding
{
public:
	Wedding(UINT64 id, KKSG::WeddingType type, UINT64 roleId1, UINT64 roleId2);

	UINT64 GetId() { return m_id; }
	UINT64 GetRoleId1() { return m_roleID1; }
	UINT64 GetRoleId2() { return m_roleID2; }
	bool IsOwner(UINT64 roleId);

	inline void SetPermitStranger(bool permit) { m_permitStranger = permit; }
	bool StrangerCanSee() { return m_permitStranger; }
	void SetSceneId(UINT32 sceneId) { m_sceneId = sceneId; }
	void SetState(KKSG::WeddingState state, UINT32 startTime);
	void OnEnd();
	UINT32 GetStateLeftTime();
	UINT32 GetLeftTime();

	KKSG::ErrorCode DoInvite(UINT64 operId, UINT64 roleId);
	KKSG::ErrorCode DoApply(UINT64 roleId);
	KKSG::ErrorCode DoAgreeApply(UINT64 operId, UINT64 roleId);
	KKSG::ErrorCode DoDisAgreeApply(UINT64 operId, UINT64 roleId);

	KKSG::ErrorCode DoEnterWedding(CRole* pRole);

	void FillWeddingBrief(KKSG::WeddingBrief* data);
	void FillInviteInfo(UINT64 roleId, KKSG::GetWeddingInviteInfoRes& res);

	void CreateWeddingScene();

	bool CanEnter(UINT64 roleId);
	bool InApplyList(UINT64 roleId);
	bool InRefuseList(UINT64 roleId);
	bool IsEntered(UINT64 roleId);
	void RemoveFromApplyList(UINT64 roleId);
	void SetSendNotice(UINT32 after);

	void PushInviteNtf(KKSG::WeddingInviteOperType type, UINT64 roleId);

	UINT32 GetMaxInviteNum();

	bool CheckInvalidEnd();
	void Update();

	void DoTxLog(UINT64 roleId);

private:
	UINT64 m_id;
	KKSG::WeddingType m_type;
	UINT64 m_roleID1;
	UINT64 m_roleID2;
	bool m_permitStranger;
	UINT32 m_createTime;

	UINT32 m_sceneId;
	KKSG::WeddingState m_state;
	UINT32 m_stateStartTime;
	UINT32 m_sendNoticeTime;
	bool m_sendNotice;

	std::vector<UINT64> m_inviteList;
	std::vector<UINT64> m_applyList;
	std::vector<UINT64> m_refuseList;
	std::vector<UINT64> m_enterList;		// 进去过的玩家列表
};

#endif
