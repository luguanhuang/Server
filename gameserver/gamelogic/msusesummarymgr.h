#ifndef __MSUSESUMMARYMGR_H__
#define __MSUSESUMMARYMGR_H__

class RoleSummary;
class Role;

//namespace KKSG
//{
//	class MSRobotSumamryRes;
//}
//
//class UseSumamry
//{
//public:
//	UseSumamry(UINT64 roleID, RoleSummary* summary);
//	~UseSumamry();
//
//	UINT64 GetRoleID(){ return m_roleID; }
//	int GetUseID(){ return m_useID; }
//	RoleSummary* GetRoleSummary(){ return m_roleSummary; }
//
//private:
//	UINT64 m_roleID;
//	int m_useID;
//	RoleSummary* m_roleSummary;	
//};
//
//class MSUseSummaryMgr
//{
//	MSUseSummaryMgr();
//	~MSUseSummaryMgr();
//	DECLARE_SINGLETON(MSUseSummaryMgr)
//
//public:
//	bool Init();
//	void Uninit();
//
//	bool GetUseSummary(Role* pRole, int lookupid, KKSG::MSRobotSumamryRes& data);
//	void FreeUseSummary(UINT64 roleID);
//	RoleSummary* GetRoleSummaryByUseID(int robotUseID);
//
//private:
//	std::unordered_map<UINT64, UseSumamry*> m_useSummarys;
//};

#endif