#ifndef _ROLE_PUSH_INFO_RECORD_H_
#define _ROLE_PUSH_INFO_RECORD_H_

#include "unit/roleserilizeinterface.h"

namespace KKSG
{
	class RolePushInfo;
}

class Role;

class RolePushInfoRecord : public RoleSerializeInterface
{
public:
	RolePushInfoRecord(Role *pRole);
	~RolePushInfoRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);
	void ToKKSG(KKSG::RolePushInfo& info);

	void UpdateConfig(const std::string& type, const std::string& value);
	void AddInfo(UINT32 type, UINT32 subType, UINT32 time);
	void SetForbid(UINT32 type, bool forbid);

private:
	struct RolePushInfoItem
	{
		UINT32 type;
		UINT32 subType;
		UINT32 time;
	};
	struct RolePushConfigItem
	{
		UINT32 type;
		bool forbid;
	};

private:
	Switch m_dirty;
	Role *m_role;

	std::list<RolePushInfoItem> m_infos;
	std::list<RolePushConfigItem> m_configs;
};

#endif