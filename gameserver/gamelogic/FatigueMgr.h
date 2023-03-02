#ifndef __FATIGUEMGR_H__
#define __FATIGUEMGR_H__

class Role;

class FatigueMgr
{
	FatigueMgr();
	~FatigueMgr();
	DECLARE_SINGLETON(FatigueMgr)

public:

	bool Init();
	void Uninit();

	static void AddLoginFatigue(Role *pRole);
	static int AddFatigue(Role *pRole, int addFatigue, int fatigueEnum, int maxFatigue);
	static void NotifyFatigueRecoverTime(Role *pRole);

	static void CheckFatigueRecover(Role* pRole);
};

#endif // __FATIGUEMGR_H__