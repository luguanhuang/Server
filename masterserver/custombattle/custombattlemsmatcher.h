#ifndef __CUSTOMBATTLEMSMATCHER_H__
#define __CUSTOMBATTLEMSMATCHER_H__

#include "scene/scenecreate.h"
#include "custombattle/custombattlematcher.h"

struct MatchIndex
{
	MatchIndex()
	{
		iscross = false;
	}
	bool iscross;
};
class CustomBattleMsMatcher : public CustomBattleMatcherMgr, public ICreateBattleListener
{
public:
	CustomBattleMsMatcher();
	~CustomBattleMsMatcher();
	DECLARE_SINGLETON(CustomBattleMsMatcher);
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	virtual void DoBattlePair(CustomBattleMatchSceneInfo& info, const std::vector<UINT64>& matchone, const std::vector<UINT64>& matchtwo);

	void Cancel(UINT64 roleid);
	void Add(UINT64 roleid, bool iscross);

private:
	std::unordered_map<UINT64, MatchIndex> m_roleid2index;
};

#endif
