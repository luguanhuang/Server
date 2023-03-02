#ifndef __XACTIONDEFINE_H__
#define __XACTIONDEFINE_H__

#include "pch.h"
#include "math/XMath.h"
#include "xdelegate.hpp"
#include "timeutil.h"
#include "table/SkillReader.h"
#include "buff/XBuffTemplateManager.h"

enum XActionDefine
{
	XAction_Invalid      = -1,
	//move event
	XAction_Idle		 = 0,
	XAction_Move,
	XAction_Jump,
	XAction_Fall,
	//skill event
	XAction_Charge,
	XAction_Freeze,
	XAction_BeHit,
	XAction_Attack,
	XAction_QTE,
	XAction_QTE_On,
	XAction_OnSkillFired,
	//rotation
	XAction_Rotation,
	//others
	XAction_EquipChange,

	XAction_AITick,
	XAction_AIEnterFight,
	XAction_PlayerAutoFight,
	XAction_PlayerRestartFight,
	XAction_AIEnable,
	XAction_AIDisable,
	XAction_AIEvent,

	XAction_BuffAdd,
	XAction_BuffRemove,
	XAction_AttributeChange,
	XAction_RealDead,
	XAction_Revive,

	XAction_AddSkill,
	XAction_RemoveSkill,

	XAction_LeaveScene,

	XAction_AIStartSkill,
	XAction_AIEndSkill,
	XAction_AISkillHurt,
	XAction_Enmity,

	XAction_ArmorBroken,
	XAction_ArmorRecover,
	XAction_OnPlayerTransfer,

	XAction_WoozyOn,
	XAction_WoozyOff,

	XAction_StrengthPresevationStop,
	XAction_ProjectDamage,
	XAction_ComboChange,

	XAction_Manipulation_On,
	XAction_Manipulation_Off,

	XAction_SkillExternal,

	XAction_BattleEnd,

	XAction_Max
};

class XActionArgs : public IArgs
{
	static UINT64 s_token;

	//event define
protected:
	XActionDefine _eDefine;
	UINT64 _token;

public:
	XActionArgs()
		: _eDefine(XAction_Invalid)
	{
		_token = s_token++;
	}

	virtual ~XActionArgs(){ }

	inline XActionDefine ArgsDefine(){ return _eDefine; }
	inline UINT64 GetToken() const { return _token; }
	inline void SetToken(UINT64 token){ _token = token; }
};

class XSkillExternalArgs;
typedef bool (*SkillExternalCallback)(const XSkillExternalArgs*);

class XSkillExternalArgs : public XActionArgs
{
public:
	XSkillExternalArgs()
	{
		_eDefine = XAction_SkillExternal;
		
		callback = NULL;
		delay = 0;
	}

	SkillExternalCallback callback;
	float delay;
};

class XIdleActionArgs : public XActionArgs
{
public:
	XIdleActionArgs()
	{
		_eDefine = XAction_Idle;
	}
};

class XRotationActionArgs : public XActionArgs
{
public:
	XRotationActionArgs()
		:TargetDir(Vector3::forward),
		Palstance(0),
		ClearSkillTarget(false)
	{
		_eDefine = XAction_Rotation;
	}
	virtual ~XRotationActionArgs(){ }

	Vector3 TargetDir;
	float Palstance;
	bool ClearSkillTarget;
};

class XMoveActionArgs : public XActionArgs
{
public:
	XMoveActionArgs()
		:Destination(Vector3::zero),
		StopTowards(Vector3::zero),
		Inertia(false),
		Stoppage(true),
		Speed(0)
	{
		_eDefine = XAction_Move;
	}

	virtual ~XMoveActionArgs(){ }

	//destination
	Vector3 Destination;
	//stop rotate to
	Vector3 StopTowards;

	bool Inertia;
	bool Stoppage;
	float Speed;
};

class XAttackActionArgs : public XActionArgs
{
public:
	XAttackActionArgs()
		:Identify(0), Target(NULL), Slot(-1), HasManualFace(false), ManualFace(0), TimeScale(1.0f)
	{
		_eDefine = XAction_Attack;
	}
	virtual ~XAttackActionArgs(){ }

	UINT32 Identify;
	UINT64 Target;

	INT32 Slot;
	bool HasManualFace;
	float ManualFace;

	float TimeScale;
};

class XRealDeadActionArgs : public XActionArgs
{
public:
	XRealDeadActionArgs()
		:RemainTime(0)
	{
		_eDefine = XAction_RealDead;
	}
	virtual ~XRealDeadActionArgs(){ }

	float RemainTime;
};

class XChargeActionArgs : public XActionArgs
{
public:
	XChargeActionArgs()
	{
		new(this) XChargeActionArgs(NULL);
	}

	XChargeActionArgs(const XChargeData* data)
		:XActionArgs(),
		Height(0),
		TimeSpan(0),
		Target(0),
		TimeGone(0),
		TimeScale(1.0f)
	{
		_eDefine = XAction_Charge;
		_data = data;
	}

	float Height;
	float TimeSpan;
	UINT64 Target;
	float TimeGone;
	float TimeScale;

public:
	inline const XChargeData* Data() const { return _data; }

private:
	const XChargeData* _data;
};

class XFreezeActionArgs : public XActionArgs
{
public:
	XFreezeActionArgs()
		:FreezeFromHit(false),
		 HitFrom(0),
		 SkillFrom(0),
		 ResultIdx(0),
		 Duration(0),
		 Present(false)
	{
		_eDefine = XAction_Freeze;
	}

	bool FreezeFromHit;

	UINT64 HitFrom;
	UINT32 SkillFrom;
	UINT32 ResultIdx;

	float Duration;
	bool Present;
};

class XBeHitActionArgs : public XActionArgs
{
public:
	XBeHitActionArgs()
	{
		new(this) XBeHitActionArgs(NULL);
	}

	XBeHitActionArgs(const XHitData* data)
		:XActionArgs(),
		HitFrom(0),
		ForceToFlyHit(false),
		HitDirection(Vector3::forward),
		Paralyze(1),
		SkillFrom(0),
		ResultIdx(0)
	{
		_eDefine = XAction_BeHit;
		_data = data;
	}

	UINT64 HitFrom;
	bool ForceToFlyHit;

	Vector3 HitDirection;
	float Paralyze;

	UINT32 SkillFrom;
	UINT32 ResultIdx;

public:
	inline const XHitData* Data() const { return _data; }

private:
	const XHitData* _data;
};

class XAddBuffActionArgs : public XActionArgs
{
public:
	XAddBuffActionArgs()
		:XActionArgs(),
		Caster(0),
		EffectImm(false),
		bClientOnly(false)
	{
		_eDefine = XAction_BuffAdd;
	}

	//liming
	UINT64 Caster;
	bool EffectImm;
	bool bClientOnly;

	BuffDesc singleBuff;

	vector<BuffDesc> vecBuffs;
	
};

class XRemoveBuffActionArgs : public XActionArgs
{
public:
	XRemoveBuffActionArgs()
		:XActionArgs(),
		nBuffId(0),
		Caster(0)
	{
		_eDefine = XAction_BuffRemove;
	}

	//liming
	int   nBuffId;
	UINT64 Caster;
};

class XAIAutoFightEventArgs : public XActionArgs
{
public:
	XAIAutoFightEventArgs() : XActionArgs()
	{
		_eDefine = XAction_AITick;
		TemplateId = 0;
		PresentId = 0;
		HeartRateTime = 0.0f;
		AlphaTime = 0.0f;
		EventTime = 0.0f;
		SceneId = 0;
		TypeId = 0;
	}

	int TemplateId;
	int PresentId;
	float HeartRateTime;
	float AlphaTime;
	float EventTime;
	int SceneId;
	int TypeId;
};

class XAIStartSkillEventArgs : public XActionArgs
{
public:
	XAIStartSkillEventArgs() : XActionArgs()
	{
		SkillId = 0;
		IsCaster = false;
		_eDefine = XAction_AIStartSkill;
	}

	int SkillId;
	bool IsCaster;
};

class XAIEndSkillEventArgs : public XActionArgs
{
public:
	XAIEndSkillEventArgs() : XActionArgs()
	{
		SkillId = 0;
		IsCaster = false;
		_eDefine = XAction_AIEndSkill;
	}

	int SkillId;
	bool IsCaster;
};

class XAISkillHurtEventArgs : public XActionArgs
{
public:
	XAISkillHurtEventArgs() : XActionArgs()
	{
		SkillId = 0;
		IsCaster = false;
		CasterUid = 0;
		TargetUid = 0;
		_eDefine = XAction_AISkillHurt;
	}

	int SkillId;
	bool IsCaster;
	UINT64 CasterUid;
	UINT64 TargetUid;
};


class XAIEnterFightEventArgs : public XActionArgs
{
public:
	XAIEnterFightEventArgs() : XActionArgs()
	{
		TargetID = 0;
		_eDefine = XAction_AIEnterFight;
	}

	UINT64 TargetID;
};

class XPlayerAutoFightEventArgs : public XActionArgs
{
public:
	XPlayerAutoFightEventArgs() : XActionArgs()
	{
		_eDefine = XAction_PlayerAutoFight;
	}
};

class XPlayerRestartFightEventArgs : public XActionArgs
{
public:
	XPlayerRestartFightEventArgs() : XActionArgs()
	{
		_eDefine = XAction_PlayerRestartFight;
	}
};

class XAIEnableEventArgs : public XActionArgs
{
public:
	XAIEnableEventArgs() : XActionArgs()
	{
		_eDefine = XAction_AIEnable;
	}
};

class XAIDisableEventArgs : public XActionArgs
{
public:
	XAIDisableEventArgs() : XActionArgs()
	{
		_eDefine = XAction_AIDisable;
	}
};

class XAIEventArgs : public XActionArgs
{
public:
	XAIEventArgs() : XActionArgs()
	{
		_eDefine = XAction_AIEvent;
		EventArg = "";
		Templateid = 0;
		WaveId = 0;
		UnitId = 0;
		Pos.set(0, 0, 0);
		MsgTargetTemplateId = 0;
		EnemyTemplateId = 0;
		SkillId = 0;
		DelayTime = 0.0f;
		FloatArg = 0.0f;
		SenderUID = 0;
	}

	int EventType;
	int MsgTargetTemplateId;
	std::string EventArg;
	int Templateid;
	Vector3 Pos;
	int WaveId;
	UINT64 UnitId;  // old unit id, used for reborn
	int SkillId;
	float DelayTime;
	int EnemyTemplateId; // used for skill firer
	float FloatArg;
	UINT64 SenderUID;

};

class XLeaveSceneEventArgs : public XActionArgs
{
public:
	XLeaveSceneEventArgs() : XActionArgs()
	{
		_eDefine = XAction_LeaveScene;
	}

};

class XAddSkillEventArgs : public XActionArgs
{
public:
	XAddSkillEventArgs() : XActionArgs()
	{
		_eDefine = XAction_AddSkill;
		SkillHash = 0;
	}

	UINT32 SkillHash;
};

class XRemoveSkillEventArgs : public XActionArgs
{
public:
	XRemoveSkillEventArgs() : XActionArgs()
	{
		_eDefine = XAction_RemoveSkill;
		SkillHash = 0;
	}

	UINT32 SkillHash;
};

class XReviveEventArgs : public XActionArgs
{
public:
	XReviveEventArgs()
	{
		_eDefine = XAction_Revive;
	}
};

class XAttributeChangedEventArgs : public XActionArgs
{
public:
	XAttributeChangedEventArgs()
	{
		_eDefine = XAction_AttributeChange;
		AttrID = 0;
	}

	int AttrID;
};

class XSkillQTEEventArgs : public XActionArgs
{
public:
	XSkillQTEEventArgs()
	{
		_eDefine = XAction_QTE;
		State = 0;
		On = false;
	}

	int State;
	bool On;
};

class XOnQTEOnEventArgs : public XActionArgs
{
public:
	XOnQTEOnEventArgs()
	{
		_eDefine = XAction_QTE_On;
		State = 0;
	}

	int State;
};

class XSkillFiredEventArgs : public XActionArgs
{
public:
	XSkillFiredEventArgs()
	{
		_eDefine = XAction_OnSkillFired;

		Succeed = false;
		Id = 0;
	}

	bool Succeed;
	UINT32 Id;
};

class XEnmityEventArgs : public XActionArgs
{
public:
	XEnmityEventArgs() : XActionArgs()
	{
		m_caster = 0;
		m_value = .0f;
		m_skillid = 0;
		_eDefine = XAction_Enmity;
	}

	float m_value;
	UINT64 m_caster;
	UINT32 m_skillid;
};

class XArmorBrokenArgs : public XActionArgs
{
public:

	XArmorBrokenArgs()
	{
		_eDefine = XAction_ArmorBroken;
	}
};

class XArmorRecoverArgs : public XActionArgs
{
public:

	XArmorRecoverArgs()
	{
		_eDefine = XAction_ArmorRecover;
	}
};

//½øÈëÐéÈõ×´Ì¬
class XWoozyOnArgs : public XActionArgs
{
public:
	XWoozyOnArgs()
	{
		_eDefine = XAction_WoozyOn;
	}
};

//ÍË³öÐéÈõ×´Ì¬
class XWoozyOffArgs : public XActionArgs
{
public:
	XWoozyOffArgs()
	{
		_eDefine = XAction_WoozyOff;
	}
};

class XStrengthPresevationStopArgs : public XActionArgs
{
public:
	XStrengthPresevationStopArgs()
	{
		_eDefine = XAction_StrengthPresevationStop;
	}
};

struct HurtInfo;
struct ProjectDamageResult;
class XProjectDamageArgs : public XActionArgs
{
public:
	XProjectDamageArgs(const HurtInfo* pHurtInfo, const ProjectDamageResult* pResult)
		: _pProjectDamageResult(pResult)
		, _pHurtInfo(pHurtInfo)
	{
		_eDefine = XAction_ProjectDamage;
	}

	inline const ProjectDamageResult* GetProjectDamageResult() const { return _pProjectDamageResult; }
	inline const HurtInfo* GetHurtInfo() const { return _pHurtInfo; }

private:
	const ProjectDamageResult* _pProjectDamageResult;
	const HurtInfo* _pHurtInfo;
};

class XComboChangeArgs : public XActionArgs
{
public:
	XComboChangeArgs(UINT32 count)
		: _ComboCount(count)
	{
		_eDefine = XAction_ComboChange;
	}

	inline const UINT32 GetComboCount() const { return _ComboCount; }
private:
	UINT32 _ComboCount;
};

class XManipulationOnArgs : public XActionArgs
{
public:
	XManipulationOnArgs()
	{
		new(this) XManipulationOnArgs(NULL);
	}

	XManipulationOnArgs(const XManipulationData* data)
		:XActionArgs()
	{
		_eDefine = XAction_Manipulation_On;
		_data = data;
	}

public:
	inline const XManipulationData* Data() const { return _data; }

private:
	const XManipulationData* _data;
};

class XManipulationOffArgs : public XActionArgs
{
public:
	XManipulationOffArgs()
	{
		_eDefine = XAction_Manipulation_Off;
		DenyToken = 0;
	}

	int DenyToken;
};

class XSkillExternalBuffArgs : public XSkillExternalArgs
{
public:
	XSkillExternalBuffArgs()
		:XSkillExternalArgs()
	{
		xTargetID = 0;
	}

	BuffDesc xBuffDesc;
	UINT64 xTargetID;
};

class XBattleEndArgs : public XActionArgs
{
public:
	XBattleEndArgs()
	{
		_eDefine = XAction_BattleEnd;
	}
};
#endif	//__XACTIONDEFINE_H__
