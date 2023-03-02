#ifndef __SCENEMORTUARY_H__
#define __SCENEMORTUARY_H__

#include "timer.h"
#include "pb/enum.pb.h"
#include "cvsreader.h"

class Role;
class Scene;
struct BuffDesc;

struct DeathRecord
{
	UINT64 qwRoleID;
	bool isaddbuff;
	int nDeathCount;
	int nReviveLimitCount;
	int nReviveCount[KKSG::ReviveMax];

	DeathRecord()
	{
		qwRoleID = 0;
		isaddbuff = false;
		nDeathCount = 0;
		nReviveLimitCount = 0;
		memset(nReviveCount, 0, sizeof(nReviveCount));
	}
};

struct AutoReviveInfo
{
	time_t revivetime;
	int revivetype;
	bool islimit;

	AutoReviveInfo()
	{
		revivetime = 0;
		revivetype = 0;
		islimit = false;
	}
};

///> �������������Ľ�ɫ����
class SceneMortuary : public ITimer
{
public:
	SceneMortuary(Scene* pScene);
	~SceneMortuary();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddDeathRole(Role* pRole);
	void DeleteDeathRoleInTimer(Role* role);
	DeathRecord* GetDeathRecord(UINT64 qwRoleID);

	DeathRecord* IncDeathCount(UINT64 qwRoleID);
	DeathRecord* IncReviveCount(UINT64 qwRoleID, int type, bool islimit);

	int GetDeathCount(UINT64 qwRoleID);
	///> �����������
	int GetReviveLimitCount(UINT64 roleid);
	///> ʹ�õ��߸������
	int GetItemReviveCount(UINT64 roleid);

	int GetReviveCount(UINT64 qwRoleID);

	bool CanVipRevive(UINT64 qwRoleID);

	void ClearDeathInfo();
	void SetReviveConfig(int interval, int maxcount, std::vector<Sequence<UINT32, 2>>& buffs);

	std::vector<BuffDesc>& GetReviveBuffs() { return m_revivebuffs; }

private:
	int GetReviveCount(UINT64 qwRoleID, int type);

private:
	bool m_isInOnTimer;
	HTIMER m_handler;
	Scene* m_pScene;
	int m_ReviveFreeInterval;
	int m_ReviveFreeMaxCount;
	int m_ReviveAutoInterval;
	std::vector<BuffDesc>  m_revivebuffs;

	std::map<UINT64, DeathRecord> m_RoleDeathCount;

	std::set<std::pair<time_t, UINT64>> m_DeathTime;
	std::map<UINT64, AutoReviveInfo> m_roleid2revivetype;
};

#endif // __SCENEMORTUARY_H__