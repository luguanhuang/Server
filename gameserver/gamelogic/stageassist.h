#ifndef __STAGEASSIST_H__
#define __STAGEASSIST_H__

#include "table/BattleAssist.h"
#include "table/BattleBuff.h"

struct StageAssistInfo
{
	UINT32 stageid;
	UINT32 time;
	UINT32 hp;
	UINT32 revive;
	double ppt;
	bool iswin;

	StageAssistInfo()
	{
		stageid = 0;
		time = 0;
		hp = 0;
		ppt = 0;
		revive = 0;
		iswin = false;
	}
};

class Role;
struct BuffDesc;
class StageAssistMgr
{
	StageAssistMgr();
	~StageAssistMgr();
	DECLARE_SINGLETON(StageAssistMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	int CalAssistPoint(const StageAssistInfo& info);
	BuffDesc GetAssistBuff(Role* role, UINT32 scenetype);

private:
	BattleAssist m_assisttable;
	BattleBuff m_bufftable;
};

#endif