#ifndef __CHANGEPROFESSION_H__
#define __CHANGEPROFESSION_H__

#include "pb/enum.pb.h"
#include "pb/project.pb.h"

struct RoleChangeProInfo
{
	UINT32 lastpro;
	UINT32 pro;
	bool hasslotrole;
	KKSG::RoleBriefInfo slotrole;
	std::map<UINT32, KKSG::HiddenRole> hiddenroles;

	RoleChangeProInfo()
	{
		lastpro = 0;
		pro = 0;
		hasslotrole = false;
	}
};

class Role;
class ChangeProfessionMgr
{
	ChangeProfessionMgr();
	~ChangeProfessionMgr();
	DECLARE_SINGLETON(ChangeProfessionMgr);
public:
	bool Init();
	void Uninit();

	KKSG::ErrorCode Check(Role* role, UINT32 pro);
	void BeginChange(Role* role, UINT32 pro);

	KKSG::ErrorCode DoChange(Role* role);

	RoleChangeProInfo* GetInfo(UINT64 roleid);
	void DelInfo(UINT64 roleid);
	bool Fill(UINT64 roleid, bool hasslotrole, const KKSG::RoleBriefInfo& _slotrole, const KKSG::HiddenRoles& _hiddenroles);
	void DoTxLog(Role* role, INT32 lastpro, INT32 pro);

private:
	std::map<UINT64, RoleChangeProInfo> m_roleid2info; 
};

#endif