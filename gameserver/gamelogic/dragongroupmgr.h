#ifndef __DRAGONGROUPMGR_H__
#define __DRAGONGROUPMGR_H__

#include "pb/project.pb.h"
#include "pb/enum.pb.h"

class Role;
class Scene;
struct DragonGroupRoleList;

class DragonGroupMgr : public ITimer
{
	DragonGroupMgr();
	~DragonGroupMgr();
	DECLARE_SINGLETON(DragonGroupMgr);
public:
    virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Init();
	void Uninit();

    bool CheckFile(){return true;}
    void ClearFile(){}
    bool LoadFile(){return true;}

    void RpcGetDragonGroupRoleList(Role* pRole, const KKSG::DragonGroupRoleListC2S &roArg, KKSG::DragonGroupRoleListS2C &roRes);
    void RpcGetDragonGroupRecord(Role* pRole, const KKSG::DragonGroupRecordC2S &roArg, KKSG::DragonGroupRecordS2C &roRes);

    void RecordDragonGroup(const std::vector<UINT64>& roleidlist, Scene* pScene, bool iswin);

    UINT32 DragonGroupMaxTime;
    UINT32 DragonGroupMaxCount;

    bool IsLastStageID(UINT32 stageid);
    bool IsSelfFirstStageID(UINT32 stageid, Role* pRole);
    bool IsServerFirstPass(UINT32 stageid);
};

#endif
