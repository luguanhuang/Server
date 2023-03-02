#ifndef __SCENEGMF_H__
#define __SCENEGMF_H__

#include "scenegwb.h"

class SceneGMF : public SceneGWB 
{
public:
	SceneGMF(Scene* scene);
	~SceneGMF();

	void OnJoinStart();
	virtual VsStateType OnJoinStop();

	virtual void OnEnterWaitLoad();
	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();

	virtual VsStateType CheckFightEnd();
	int CheckRoleFail(UINT64 roleid);	

	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	virtual void OnStateEnd();

	UINT64 GetHpPrecentLessID();

	virtual bool SetFightRole();
	void OnRoleFailed(UINT64 failID, int type);

	void OneFightEndNotice(UINT64 winID, UINT64 loseID, UINT64 winGuildID, UINT64 loseGuildID);
	void TLogOneFightEnd(UINT64 winID, UINT64 loseID, UINT64 winGuildID, UINT64 loseGuildID);

	void AllEndToClient(UINT64 winGuildID, UINT64 loseGuildID, int wintype);

	void MakeFightEnd();

	void SetRoleID11(UINT64 roleID){ m_roleID11 = roleID; }
	void SetRoleID22(UINT64 roleID){ m_roleID22 = roleID; }
	inline UINT64 GetRoleID11(){ return m_roleID11; }
	inline UINT64 GetRoleID22(){ return m_roleID22; }
	UINT64 GetWinID();

private:

	//为0表示是一个空位
	UINT64 m_roleID11;
	UINT64 m_roleID22;
};

#endif