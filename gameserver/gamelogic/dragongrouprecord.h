#ifndef __DRAGONGROUPRECORD_H__
#define __DRAGONGROUPRECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "dragongroupmgr.h"

class Role;
struct DragonGroupRole
{
    UINT64 roleid;
    std::string rolename;
    UINT32 profession;
    DragonGroupRole(Role& role);
    DragonGroupRole(const KKSG::DragonGroupRoleInfo& roleinfo);
};

struct DragonGroupRoleList
{
    UINT32 stageid;
    UINT32 time;
    UINT32 costtime;
    bool iswin;
    bool isFirstPass;
    bool isServerFirstPass;
    UINT32 commadNum;
    UINT32 watchNum;
    std::list<DragonGroupRole> roleinfolist;

    DragonGroupRoleList(UINT64 roleid, std::vector<Role*>& rolelist);
    DragonGroupRoleList(const KKSG::DragonGroupRecordInfoList& record);
};

class DragonGroupRecord : public RoleSerializeInterface
{
public:
	DragonGroupRecord(Role* role);
	virtual ~DragonGroupRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

    void GMResetRecord();

    void FillDragonGroupRoleInfo(KKSG::DragonGroupRoleInfo& roleinfo, Role* pRole);
    void addRecord(UINT32 time, UINT32 stageid, UINT32 mcosttime, bool iswin, bool isfirstpass, bool isserverfirstpass, 
        UINT32 commadnum, UINT32 watchnum, std::vector<Role*>& rolelist);
    void clearRecord();
    void fixRecordFirstPass();

    void FillProtocRoleList(KKSG::DragonGroupRoleListS2C& rolelist);
    void FileProtocRecordList(KKSG::DragonGroupRecordS2C& record, bool isSaveDB, UINT32 maxCommadNum, UINT32 maxWatchNum);

public:
	Role* m_pRole;
	Switch m_ismodify;

    std::unordered_map<UINT64, KKSG::DragonGroupRoleInfo> teamerlist;
    std::list<DragonGroupRoleList> recordlist;

    UINT32 GetTime();
};

#endif