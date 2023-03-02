#ifndef __GUILDCASTLEREQUEST_H__
#define __GUILDCASTLEREQUEST_H__

namespace KKSG
{
	class GCFFightInfoRes;
	class GCFCommonRes;
	class GCFFightInfoRes;
}

class CRole;
class GCOneFight;
class GcfRequest
{
public:
	GcfRequest(CRole* pRole);
	~GcfRequest();

	int JoinReadyScene(UINT32 territoryID);
	int JoinFightScene(UINT32 mapID);

	void ReadyInfos(KKSG::GCFReadyInfoRes& roRes);
	void FightInfos(KKSG::GCFFightInfoRes& roRes);

	int BattleReport(KKSG::GCFCommonRes& roRes);
	int BattleReport(KKSG::GCFCommonRes& roRes, UINT32 territoryID);
	int FightResult(KKSG::GCFCommonRes& roRes);

	void Kill(UINT64 uid, UINT32 continueKill);
	void Occupy(KKSG::GCFJvDianType type, UINT64 uid);
	void BackToReady(UINT64 uid);
	void OnLeave(UINT64 uid, UINT32 sceneID);
	void OnLogout();

private:
	int ChangeBattleField(GCOneFight* pOneFight, UINT32 mapID);
	void KillNotice(CRole* pRole, UINT32 continueKill, GCOneFight* pOneFight);
	void OccupyNotice(CRole* pRole, KKSG::GCFJvDianType type, GCOneFight* pOneFight);
	
	CRole* m_pRole;
};

#endif