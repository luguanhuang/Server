#ifndef __FIRSTPASSRECORD_H__
#define __FIRSTPASSRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;

struct FirstPassInfo
{
	int firstPassID;
	bool isPass;
	bool isGetReward;


};
namespace KKSG
{
	class FirstPassStageInfo;
	class GetRankInfoRes;
	class FirstPassRankInfo;
}

class Scene;
class CFirstPassRecord : public RoleSerializeInterface
{
public:
	CFirstPassRecord(Role* role);
	virtual ~CFirstPassRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Change();

	void Update();
	void OnFirstEnterScene();
	void OnFirstEnterSceneReply(const KKSG::GetRankInfoRes& roRes);
	void OnDayPass();

	void DebugReset();

	void OnFirstPass(Scene* scene);
	void UpdateStarOneFirstPassRank(int firstPassID, int rank);

	/*const */KKSG::FirstPassStageInfo* GetFirstPassStageInfo(int firstPassID);
	void NotifyFirstPassReward(bool hasPass, bool hasCommend);
	UINT32 GetFirstPassReward(int firstPassID, int rank);

	void BeforeGetCommendReward(int firstPassID);
	void RevertGetCommendReward(int firstPassID);
	UINT32 GetCommendReward(int firstPassID, int starLevel);
	UINT32 CheckCommend(int firstPassID, int starLevel);

	void OnOpenSystem();

private:
	bool _HasReward(const KKSG::FirstPassRankInfo& info);

private:
	Role* m_pRole;
	Switch m_switch;

	std::map<int, KKSG::FirstPassStageInfo> m_passStageInfos;
};

#endif