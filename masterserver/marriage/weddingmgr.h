#ifndef _WEDDING_MGR_H_
#define _WEDDING_MGR_H_

#include "util/utilsingleton.h"
#include "wedding.h"
#include "weddingrole.h"
#include "scene/scenecreate.h"
#include "timer.h"

struct WeddingCarInfo
{
	WeddingCarInfo() : roleId1(0), roleId2(0), type(KKSG::WeddingType_Normal), startTime(0) {}
	UINT64 roleId1;
	UINT64 roleId2;
	KKSG::WeddingType type;
	UINT32 startTime;

	bool IsRunning() { return roleId1 != 0; }
};

class WeddingMgr : public ICreateBattleListener, public ITimer, public Singleton<WeddingMgr>
{
public:
	WeddingMgr();

	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	Wedding* CreateWedding(KKSG::WeddingType type, UINT64 roleId1, UINT64 roleId2);
	Wedding* GetWedding(UINT64 weddingId);
	Wedding* GetWeddingByRoleId(UINT64 roleId);
	WeddingRole* GetWeddingRole(UINT64 roleId); 
	bool IsRoleWeddingHoldding(UINT64 roleId);


	void RemoveWedding(UINT64 weddingId);
	bool HasWeddingCarRunning();
	void SetWeddingCar(UINT64 roleId1, UINT64 roleId2, KKSG::WeddingType type);
	void ResetWeddingCar();
	void NotifyWeddingCar();
	void StartWeddingCarTimer();
	bool OnWeddingCarEnd();

	void FillAllWeddingInfo(UINT64 roleId, KKSG::GetAllWeddingInfoRes& res);

private:
	UINT64 NewID();

private:
	UINT32 m_autoIncreIndex;
	std::unordered_map<UINT64, Wedding*> m_weddings;
	std::unordered_map<UINT64, WeddingRole*> m_wroles;
	WeddingCarInfo m_curWeddingCar;
};

#endif