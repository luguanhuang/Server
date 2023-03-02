#ifndef __XENTITY_H__
#define __XENTITY_H__

#include "XObject.h"
#include "state/XStateDefine.hpp"
#include "pb/enum.pb.h"

class XComponent;
class XSkillMgr;

class XEntity : public XObject
{
	friend class Unit;

public:
	static XEntity* ValidEntity(UINT64 id);
	static XEntity* ValidEntity(const Unit* pUnit);

	static bool IsValidEntity(const XEntity* pEntity);
	static bool IsValidEntity(const Unit* pUnit);
	static bool IsValidEntity(UINT64 id);

	virtual void OnCreated();

	virtual void OnEnterScene(Scene *scene);
	virtual void OnSceneReady();
	virtual void OnLeaveScene();

	virtual void CoolDownAtLaunch();

	virtual bool Has_H_Movement(){ return _movement.x != 0 || _movement.z != 0; }
	void ApplyMove(const Vector3& movement);
	void ApplyMove(float x, float y, float z);

	virtual void Dying();
	virtual void Died();

	virtual void Update(float fDeltaT);

	virtual void LateUpdate(float fDeltaT);

	inline bool StateTransfered(){ return _state_transfered; }
	inline bool StateCasted(){ return _state_casted; }
	inline bool StateSynced() { return _state_synced; }
	inline void SetStateSynced() { _state_synced = true; }

	inline void SetClientManualForward(const Vector3& forward){ _client_manual_forward = forward; }
	inline const Vector3& ClientManualForward(){ return _client_manual_forward; }
	inline bool IsClientManualOn(){ return !_client_manual_forward.IsZero(); }

	inline bool StandOn() const { return _stand_on; }
	inline bool IsDead() const { return GetUnit()->IsDead(); }

	inline bool GravityDisabled() { return _gravity_disabled; }
	inline void DisableGravity() { _gravity_disabled = true; }
	/* 
	 * only can be UINT32
	 * due to _sync_sequence can be cast to void*
	 */
	inline UINT32 SyncSequence(){ return _sync_sequence; }
	inline void SyncIncreament(){ _sync_sequence++; }

	inline UINT64 MobbedBy(){ return _mobbed_by; }
	inline void SetMobber(UINT64 mobber){ _mobbed_by = mobber; }

	inline bool IsLifewithinMobbedSkill(){ return _life_within_mobbed_skill; }
	inline void SetLifewithinMobbedSkill(bool value){ _life_within_mobbed_skill = value; }

	inline bool IsTransform(){ return _transformer != 0; }
	inline UINT32 GetTransformer(){ return _transformer; }

	KKSG::XQTEState GetQTESpecificPhase();

	Vector3 RadiusCenter();
	Vector3 HugeMonsterColliderCenter(int idx);

	/*
	 * the state at the time of function invoked
	 */
	XStateDefine GetCurrentState();
	/*
	 * the state of last frame
	 */
	inline XStateDefine GetLastState() { return _last_state; }

	int GetProfession();
	virtual void Idled();
	void Puppet(bool bPuppet);

	inline bool IsInPVPScene(){ return _in_pvp_scene; }
	inline bool IsInPVPOneScene(){ return _in_pvpone_scene; }
	/*
	 * OnTransformer
	 */
	virtual bool OnTransform(UINT32 to);

	XSkillMgr* SkillManager();

protected:
	XEntity(Unit* pUnit);
	virtual ~XEntity();

	//override the Initilize
	virtual bool Initilize();
	virtual void Move(float fDelatT);

protected:
	XStateDefine _last_state;
	UINT32 _last_casted;

	Vector3 _movement;
	/*
	 * zero means losing client manual operation
	 */
	Vector3 _client_manual_forward;

	bool _gravity_disabled;
	bool _stand_on;

	bool _state_transfered;
	bool _state_casted;
	bool _state_synced;

	/* 
	 * only can be UINT32
	 * due to _sync_sequence can be cast to void*
	 */
	UINT32 _sync_sequence;
	UINT32 _transformer;

	UINT64 _mobbed_by;
	bool _life_within_mobbed_skill;

	bool _in_pvp_scene;
	bool _in_pvpone_scene;

private:
	bool Initialization();
};

#endif	//__XENTITY_H__
