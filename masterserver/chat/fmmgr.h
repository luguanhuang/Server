#ifndef __FMMGR_H
#define __FMMGR_H

class CRole;

struct CShmFMRole
{
	UINT64 nRole;
};

class FMMgr: public ITimer
{
	FMMgr();
	~FMMgr();
	DECLARE_SINGLETON(FMMgr);
public:
	bool Init();
	void Uninit();
	void CloseRoom();
	void AddRole(UINT64 nRole);
	bool IsInRoom(UINT64 nRoleID);
	UINT64 DelRole(UINT64 nRole);
	void ClearRole();
	void AddWhiteList(const std::string& acc,const std::string &nick);
	void ClearWhiteList();
	void AddAudio(std::string &name,UINT64 roleid);
	void DelAudio(const std::string &name);
	void ClearAudio();
	void ClearFMData();
	UINT32 GetAudioSize();
	bool IsAnchor(const std::string &acc);
	bool IsAudio(UINT64 nRole);
	void SendWhiteList();
	void SendWhiteList(CRole *pRole);
	void ReloadWhiteList(KKSG::FmReloadInfo *data);
	void GetRoleList(UINT64 nRoleID,std::vector<UINT64> &rolevec);
	void OnLogin(CRole *pRole);
	void OnLoginOut(CRole *pRole);
	void LeaveRoom(const std::string &szName, UINT64 roleid);
	UINT64 GetAudioRoleID(const std::string &nickname);
	void Recover();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void DoLog(const std::string &szName, UINT64 roleid,int op,const std::string &strChat="");
public:
	inline UINT32 GetCount(){return m_nCount;}
	inline void SetCount(UINT32 nCount) { m_nCount = nCount;}
	inline UINT32 Incre(){ return ++m_nCount;}
private:
	std::set<UINT64> m_setReoverID;
	std::set<UINT64 > m_setRoleID;								//维护房间role列表
	std::unordered_map<std::string,std::string > m_mapRoleNick;	//白名单列表 key=acc value=nick
	std::unordered_map<std::string,UINT64> m_mapRadio;			//主播列表 key=std::string value=roleid
	UINT32 m_nLastTime;
	UINT32 m_nCount;
	HTIMER m_handler;

};





//
#endif