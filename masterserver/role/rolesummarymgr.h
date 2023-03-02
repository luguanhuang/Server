#ifndef __ROLESUMMARYMGR_H__
#define __ROLESUMMARYMGR_H__

#include "rolesummary.h"
#include "util/uniquequeue.h"

typedef std::function<bool (CRoleSummary*)> FindRoleFilter;

class CRoleSummaryMgr : ITimer
{
	CRoleSummaryMgr();
	~CRoleSummaryMgr();
	DECLARE_SINGLETON(CRoleSummaryMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool InsertRoleSummary(KKSG::RoleSummaryStored& roData, UINT32 mask, UINT32 timestamp, bool isbrief);

	void SetChanged(UINT64 roleid);
	void UpdateRoleSummary(CRoleSummary* roleSummary, UINT32 mask, bool isnew);

	CRoleSummary* GetRoleSummary(UINT64 qwRoleID, bool checkbrief = false);
	CRoleSummary* GetRoleSummary(const std::string& strName, bool checkbrief = false);
	CRoleSummary* GetRoleSummaryByNickId(UINT64 nickid, bool checkbrief = false);

	const std::string& GetRoleName(UINT64 qwRoleID);

	UINT32 GetCount() const { return m_oRoleSummarys.size(); }
	UINT32 GetQueueCount() const { return m_queuechanges.size(); }

	std::unordered_map<UINT64, CRoleSummary*>::iterator Begin() { return m_oRoleSummarys.begin(); }
	std::unordered_map<UINT64, CRoleSummary*>::iterator End() { return m_oRoleSummarys.end(); }

	void UpdateLastLoginRole(const std::string& account);
	CRoleSummary* GetLastLoginRole(const std::string& account);
	void GetRoleIdByAccount(const std::string& account, std::vector<UINT64>& rolelist);

	// platType 平台类型
	void GetPushRole(UINT32 platType, FindRoleFilter filter, std::list<CRoleSummary*>& roles);

	void UpdateRankList();

	void LockName(const std::string& strName);
	void UnlockName(const std::string& strName);
	bool IsNameLocked(const std::string& strName) const;
	bool IsValidAcountRole(const std::string& account, UINT64& qwRoleID);

	void SetEnd() { m_isend = true; }

	UINT32 IsNameInvalid(const std::string& name);
	bool ChangeName(const std::string& lastname, const std::string& nowname);

	void DumpRoleAllInfoSize();
	void RecordSize(const ::google::protobuf::MessageLite& data, std::stringstream& ss);

	UINT32 GetAccountNum() { return m_account2LastRole.size(); }

	void TestSave();
	inline UINT32 GetMaxLevel() { return m_maxLevel; }
	void SetMaxLevel(UINT32 level) { m_maxLevel = level; }

	void DoBrief(KKSG::RoleSummaryStored& data);
	void QuerySummaryFromDb(UINT64 roleid);

	bool IsExpire(UINT64 roleid, int expiretime, int level = 0);
	UINT32 GetAccountRoleNumSameServer(UINT64 roleId);

private:
	HTIMER m_timer;
	bool m_isend;
	UniqueQueue<UINT64> m_queuechanges;

	std::unordered_map<UINT64, CRoleSummary*>	m_oRoleSummarys;
	std::unordered_map<std::string, UINT64>		m_oName2RoleId;
	std::unordered_map<UINT64, UINT64>			m_oNickId2RoleId;
	std::unordered_set<std::string>				m_oNameLocked;

	std::unordered_multimap<std::string, CRoleSummary*> m_account2Role;	/// 一个帐号下多个角色
	std::unordered_map<std::string, CRoleSummary*>	m_account2LastRole; /// 一个帐号下最近玩过的角色
	std::map<UINT32, std::unordered_set<CRoleSummary*> > m_plat2LastRole;	/// 一个平台下最近玩过的角色
	UINT32 m_maxLevel;		// 全服最高等级
};

#endif // __ROLESUMMARYMGR_H__
