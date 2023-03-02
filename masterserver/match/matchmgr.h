#ifndef __MATCHMGR_H__
#define __MATCHMGR_H__

#include "timer.h"
#include "match/matchmgrbase.h"
#include "scene/scenecreate.h"
#include "cmdline.h"

class MatchMgr : public ITimer, public MatchMgrBase, public ICreateBattleListener
{
	MatchMgr();
	~MatchMgr();
	DECLARE_SINGLETON(MatchMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	virtual void CreateSceneBegin(const KKSG::KMatchFightData& data, std::vector<MatchUnit*>& matched);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	void MatchDone(int type, const KKSG::KMatchUnit& kunit);
	UINT32 GetMapID(int type, const KKSG::KMatchUnit& kunit);
	void FillDataExtra(KKSG::KMatchFightData& data);

	void OnCrossZoneUnusable();
	void OnMsCrashRecover(UINT32 gsLine, const KKSG::CrashRecoverM2GRes &roRes);

private:
	void MatchTimeOut(MatchUnit* pMatchUnit);
	virtual void OnDelReason(MatchUnit* pUnit, MatchDelReason reason);
	void DoTxLog(int type, int teamID);

	HTIMER m_handler;
};

#endif