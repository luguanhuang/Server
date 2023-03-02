#ifndef __SYSTEMINFO_H__
#define __SYSTEMINFO_H__

#include "utility/BitSet.h"
#include "unit/roleserilizeinterface.h"
#include "define/systemiddef.h"

class Role;

namespace KKSG
{
	class RoleBrief;
}

class CRoleSystem : public RoleSerializeInterface
{
public:
	CRoleSystem(Role *pRole);
	~CRoleSystem();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	void SetSystemOpened(int sysID, bool isopen = true);
	int GetSystemOpenTime(int sysID);

	void SetAllSystemOpened();

	inline bool IsFirstDone(UINT32 systemid) { return m_firsttime.check(systemid); }
	inline void SetFirstDone(UINT32 systemid) { m_firsttime.Set(systemid, true); m_ischanged.Set(); }
	bool IsSystemOpened(int sysID);
	//用于新开放系统
	bool IsSystemOpenedAtRole(int sysID);

	const std::string SystemBits() { return m_openSystem.ToString(); }
	const std::string FirstBits() { return m_firsttime.ToString(); }

private:
	Switch m_ischanged;

	BitSet<MAX_SYSTEM_ID>  m_openSystem;
	BitSet<MAX_SYSTEM_ID>  m_firsttime;

	std::map<int, int> m_openTime;
	Role *m_pRole;
};

#endif // __SYSTEMINFO_H__