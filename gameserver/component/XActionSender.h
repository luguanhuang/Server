#ifndef __XNETACTOINSENDER_H__
#define __XNETACTOINSENDER_H__

#include "unit/unit.h"
#include "unit/role.h"
#include "component/XNetComponent.h"
#include "action/ptcg2c_syncstepnotify.h"

class Unit;
class Role;
class XObject;
class CProtocol;
struct RoleViewSet;
class PtcG2C_SyncStepNotify;

class XActionSender
{
public:
	XActionSender();
	virtual ~XActionSender();

	void PackageData(Role* roles[], int count, Role* self);
	void PackageData(const std::list<Role*>& roles, Role* self);

	void PackageData(const std::unordered_map<UINT64, Unit*>& units);
	void PackageData(const std::unordered_map<UINT64, RoleViewSet*>& roles);

	void Broadcast(const std::vector<Role*>& roles);
	void Broadcast(const std::unordered_map<UINT64, Role*>& roles);
	void Broadcast(const std::unordered_map<UINT64, Unit*>& units);
	void Broadcast(const std::unordered_map<UINT64, RoleViewSet*>& roles);

	inline void Send(Role* role)
	{
		if (m_ischanged)
		{
			role->Send(sm_oPtc);
		}
	}
	inline void Recycle()
	{
		m_ischanged = false;
		sm_oPtc.m_Data.Clear();
	}
private:
	inline bool Package(XObject* pObj)
	{
		if(pObj->NetComponent()->NeedBroadCast())
		{
			pObj->NetComponent()->PackageSyncData(sm_oPtc.m_Data.add_datalist());
			return true;
		}
		else
			return false;
	}

	bool m_ischanged;
	static PtcG2C_SyncStepNotify sm_oPtc;
};

#endif	//__XNETACTOINSENDER_H__