#ifndef __XNETCOMPONENT_H__
#define __XNETCOMPONENT_H__

#include "XActionStateComponent.hpp"
#include "entity/XActionDefine.h"
#include "skill/XSkillCore.h"
#include "process.h"
#include "pb/project.pb.h"

class Unit;
class Scene;
class CProtocol;
class PtcG2C_SkillResult;
class PtcG2C_SyncMoveNotify;

class XNetComponent : public XComponent
{
public:
	static const UINT32 uuID = XNet_Component;

	XNetComponent(void);
	virtual ~XNetComponent(void);

	virtual UINT32 ID() { return XNetComponent::uuID; }

	bool NeedBroadCast();
	void PackageSyncData(KKSG::StepSyncData* pdata);

	bool BroadcastSkillResult(PtcG2C_SkillResult& oPtc);
	bool BroadcastCDCall(uint skillid, int runtime, bool onsyntonic);
	bool BroadcastRandomWarningPos(uint skill, const vector<vector<XSkillCore::XWarningRandomPackage> >& set);
	bool BroadcastCorrectLocation(const Vector3& pos, float face, bool transfer, bool toall);

	inline void Passive(bool p) { _passive_move = p; }
	inline bool IsPassive() { return _passive_move; }

	inline bool DuringCorrectProtected() { return _correct_protected; }

	virtual void Update(float fDeltaT);

	inline UINT32 CurrentSequence() { return _last_sync_sequence; }
	void ResetSyncData();

	void ReportMoveAction(const Vector3& des, bool stoppage, const Vector3& face);

private:
	bool Broadcast(const CProtocol& proto, bool pExludeSelf);
	bool BroadcastToself(const CProtocol& proto);
	bool BroadcastMoveAction(const PtcG2C_SyncMoveNotify& oPtc);

private:
	bool _passive_move;
	bool _last_passive_move;
	bool _correct_protected;

	Vector3 _last_sync_pos;
	float _last_sync_face;
	UINT32 _last_sync_sequence;

	float _elapsed;

	KKSG::StepSyncData _sync_data;
};

#endif	//__XNETCOMPONENT_H__