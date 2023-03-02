#ifndef __DESIGNATION_RECORD_H__
#define __DESIGNATION_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "unit/role.h"

struct stDesignationInfo
{
	UINT32	dwDesignationID;
	UINT32	dwReachTimestamp;
	bool	bIsNew; 
	std::string szName;
	UINT32 dwType;
	stDesignationInfo()
	{
		dwDesignationID  = 0;
		dwReachTimestamp = 0;
		bIsNew = false; 
		szName = "";
		dwType = 0;
	}
};

class DesignationRecord : public RoleSerializeInterface
{
public:
	DesignationRecord(Role* role);
	virtual ~DesignationRecord();

	bool Illegal(UINT32 nDesID);
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	void Init();

	stDesignationInfo* GetDesignationInfo(UINT32 designationID);

	bool AddDesignation(stDesignationInfo& info);
	bool SetName(UINT32 designationID,std::string &name);
	std::string GetName();
	std::string GetName(UINT32 nDesID);
	bool DelDesignation(UINT32 designationID);

	bool SetAbilityDesignation(UINT32 designationID);
	std::string SetCoverDesignation(UINT32 designationID);

	UINT32 GetAbilityDesignation() { return m_dwAbilityDesignationID; }
	UINT32 GetCoverDesignation() { return m_dwCoverDesignationID; }

	UINT32 GetMaxDesignationID();

	bool IsCurAbilityDesignationMax();

	bool CheckUpdateMaxDesignationID(UINT32 designationID = 0);

	UINT32 CalcMaxDesignation();

	void Change();

	void DoLog(UINT32 designationID, INT32 iOperType);

	bool HasDes(UINT32 nDesID);
	

public:
	std::map<UINT32, stDesignationInfo> m_mapDesignationInfo;

private:
	Role* m_pRole;

	Switch m_ismodify;
	UINT32 m_dwCoverDesignationID;
	UINT32 m_dwAbilityDesignationID;
	UINT32 m_dwMaxAbilityDesignationID;
};

#endif // __LOTTERYRECORD_H__
