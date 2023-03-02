#ifndef __FIRSTPASSMANAGER_H__
#define __FIRSTPASSMANAGER_H__


class ITimer;
class Scene;
class Role;

namespace KKSG
{
	class UpdateMSRankListArg;
}

class CFirstPassMgr
{
	CFirstPassMgr();
	~CFirstPassMgr();
	DECLARE_SINGLETON(CFirstPassMgr)

public:
	bool Init();
	void Uninit();

	void UpdataFirstPassRank(const KKSG::UpdateMSRankListArg& roArg, KKSG::UpdateMSRankListRes& roRes);
	void OnFirstPassRank(int nRank,UINT32 nSceneID ,std::vector<UINT64>& roleIDs);
	void NoticeNewCommendReward(std::vector<int>& serverIDs, int firstPassID);

	void Update(float deltaTime);

	void CheckFirstPass(Scene* scene);

private:

};

#endif // 
