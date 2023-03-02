#ifndef __FIRSTPASSMANAGER_H__
#define __FIRSTPASSMANAGER_H__


class ITimer;
class Scene;
class Role;

namespace KKSG
{
	class UpdateMSRankListArg;
	class UpdateMSRankListRes;
}

class FirstPassMgr
{
	FirstPassMgr();
	~FirstPassMgr();
	DECLARE_SINGLETON(FirstPassMgr)

public:
	bool Init();
	void Uninit();

	void Update(float deltaTime);

	void CheckFirstPass(Scene* scene);

	void OnUpdateMsRankReply(const KKSG::UpdateMSRankListArg &roArg, const KKSG::UpdateMSRankListRes &roRes);

};

#endif // 
