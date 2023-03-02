#ifndef __XAICOMPONENT_H__
#define __XAICOMPONENT_H__

#include "XComponent.h"
#include "gamelogic/XEnmityList.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"
#include "unit/iaiinterface.h"
#include "ai/aibehaviortree.h"
#include "gamelogic/XPatrol.h"

class XEntity;
class Enemy;
class XSkillComponent;
class AIBehaviorTree;
class AISceneHandler;
class Scene;
class XPatrol;

class XAIComponent : public XComponent
{
public:
	XAIComponent();
	~XAIComponent();

	static const UINT32 uuID = XAI_Component;
	virtual UINT32 ID() { return XAIComponent::uuID; }

	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();
	virtual void Attached();
	virtual void ActionSubscribe();

	static void InitConfig();
public:
	void InitBehaviorTree();
	void InitVariables();
	void SetBehaviorTree(const std::string& tree);
	void ManualAttached();
	void InitLinkedSkills();
	void InitFixVariables();
	void InitDummyRoleSkills();
	inline AIBehaviorTree* BehaviorTree()const {return _behaviorTree;}
	static void InitIndex();
	void SafeKillTimer();
	void ResetAILogic();
	bool IsLinkedSkillCannotCast(UINT32 skillid);
	void OnEnterScene(Scene *scene);
private:
	AIBehaviorTree* _behaviorTree;

	Unit* _host;
	int _cast_skillid;
	float _action_gap_factor;
	Vector3 _born_pos;
	int _huanraoindex;
	HTIMER _TimerToken;
	bool _bIsEnable;
	string _casting_skill;
	bool _prepare_combo_skill;
	bool _inited;

	Vector3 _center_point;
	XEnmityList _enmity_list;
	XPatrol _patrol;
	std::vector<XSkillCore*> _can_cast_skill;
	std::vector<XSkillCore*> _range_skill;
	std::vector<Unit*> _targets;
	AIDataArgs _ai_args;
	XAIEventArgs _ai_event;
	bool _is_global;
	bool _ignore_skill_cd;
	int _debug_level;   // 0: no debug  1: important info   2: base info  3: all info(include tick)
	std::vector<HTIMER> _timer_token;
	std::vector<XSkillCore*> _dummyrole_skills;

	AISceneHandler* _scene_handler;
private:
	UINT64 _target_id;
	UINT64 _old_target_id;
	UINT64 _master_id;
	Vector3 _navtarget;
	bool _is_hurt_oppo;
	float _target_distance;
	float _max_hp;
	float _current_hp;
	float _max_super_armor;
	float _current_super_armor;
	int _type;
	float _attack_range;
	float _min_keep_range;
	bool _is_casting_skill;
	bool _is_fighting;
	bool _is_qte_state;
	bool _blocked;
	int _host_templateid;
	int _host_presentid;
	int _host_typeid;
	bool _gm_autoplay;

	int _hurt_enemy_id;
	UINT32 _hurt_skill_id;

	std::map<UINT32, int> _skillid2level;
	std::map<UINT32, double> _skillid2costmp;
	std::map<UINT32, int> _skillid2hpmax;
	std::map<UINT32, int> _skillid2hpmin;
	std::set<UINT32> _link_cannot_skills;

	UINT32 _spawn_time;

public:
	static UINT32 _target_index;
	static UINT32 _master_index;
	static UINT32 _navtarget_index;
	static UINT32 _is_oppo_casting_skill_index;
	static UINT32 _is_hurt_oppo_index;
	static UINT32 _target_distance_index;
	static UINT32 _is_fixed_in_cd_index;
	static UINT32 _normal_attack_prob_index;
	static UINT32 _enter_fight_range_index;
	static UINT32 _fight_together_dis_index;
	static UINT32 _is_wander_index;
	static UINT32 _max_hp_index;
	static UINT32 _current_hp_index;
	static UINT32 _max_super_armor_index;
	static UINT32 _current_super_armor_index;
	static UINT32 _type_index;
	static UINT32 _target_rotation_index;
	static UINT32 _attack_range_index;
	static UINT32 _min_keep_range_index;
	static UINT32 _is_casting_skill_index;
	static UINT32 _is_fighting_index;
	static UINT32 _is_qte_state_index;
	static UINT32 _movedir_index;
	static UINT32 _movedest_index;
	static UINT32 _movespeed_index;
	static UINT32 _bornpos_index;
	static UINT32 _skill_id_index;
	static UINT32 _heart_rate_index;
	static bool _global_debug;

public:
	inline bool IsFighting() { return _is_fighting; }
	inline void SetFighting(bool fighting){_is_fighting = fighting;_behaviorTree->Data().SetBoolByIndex(_is_fighting_index, _is_fighting);}
	inline float MoveSpeed() { return _host->GetRunSpeed(); }
	inline Vector3 BornPos() { return _born_pos; }
	inline float AttackRange() { return _attack_range; }
	inline float MinKeepRange() { return _min_keep_range; }
	inline int HuanraoIndex() { return _huanraoindex; }
	inline void SetHuanraoIndex(int index) {_huanraoindex = index;}
	inline float AttackProb() { return _ai_args.mNormalAttackProb; }
	inline float EnterFightRange() { return _ai_args.mEnterFightRange; }
	inline float FightTogetherDis() { return _ai_args.mFightTogetherDis; }
	inline bool IsCastingSkill() { return _is_casting_skill; }
	inline bool IsFixedInCd() { return _ai_args.mIsFixedInCd; }
	inline bool IsWander() { return _ai_args.mIsWander; }
	inline bool IsHurtOppo() { return _is_hurt_oppo; }
	inline bool IsQTEState() { return _is_qte_state;}
	inline void SetHurtOppo(bool value){ _is_hurt_oppo = value;}
	inline int CastingSkillId() { return _cast_skillid; }
	inline void SetCastingSkillId(int value){_cast_skillid = value;}
	inline XAIEventArgs& AIEvent() {return _ai_event;}
	inline void SetSkillCombo(bool combo){_prepare_combo_skill = combo;}
	inline bool GetSkillCombo(){return _prepare_combo_skill;}
	inline double GetCostMP(UINT32 skillid){return _skillid2costmp[skillid];}
	inline int GetHPMax(UINT32 skillid){return _skillid2hpmax[skillid];}
	inline int GetHPMin(UINT32 skillid){return _skillid2hpmin[skillid];}
	inline void SetDebug(int debuglevel) {_debug_level = debuglevel;}
	inline bool IsDebugEnable(){return _debug_level > 0;}
	inline int GetDebugLevel(){return _debug_level;}
	inline void SetIgnoreSkillCD(bool ignore){_ignore_skill_cd = ignore;}
	inline bool GetIgnoreSkillCD(){return _ignore_skill_cd;}
	inline AISceneHandler* GetSceneHandler() { return _scene_handler; }
	void SetGmAutoPlay(bool enable);

	inline std::vector<XSkillCore*>& GetCanCastSkill(){return _can_cast_skill;}
	inline std::vector<XSkillCore*>& GetRangeSkill(){return _range_skill;}
	inline std::vector<XSkillCore*>& GetDummySkills(){return _dummyrole_skills;}
	inline void SetCanCastSkill(std::vector<XSkillCore*> skills) {_can_cast_skill = skills;}
	XSkillMgr* GetSkillMgr();
	inline XEnmityList& EnmityList() { return _enmity_list; }
	inline XPatrol& GetPatrol() { return _patrol; }
	inline std::vector<Unit*>& GetTargets() { return  _targets; }
	inline void SetTarget(UINT64 id) {_target_id = id;}
	inline UINT64 GetTarget(){return _target_id;}
	inline const string& GetCastingSkillName(){return _casting_skill;}
	inline void SetCasingSkillName(const string& name){_casting_skill = name;}

	void SetTargets(const std::vector<Unit*>& targets);
	bool CheckTargetChange() { if (_target_id == _old_target_id) return false; _old_target_id = _target_id; return true; }

	bool InitOpponetEnmity(IArgs*, void*);
	void NotifyAllyIntoFight(Unit* target);
	void EnemyDetected(Unit* pTarget);
	void AddTimerToken(HTIMER token);
	void SendEnterFightEvent();
	void SendEnterFightToClient();
	bool DoSendEnterFightEvent(IArgs *args, void *parg);

public:
	bool OnTickAI(IArgs* pargs, void *);
	bool OnAIEnterFight(IArgs* pargs, void *);
	bool OnAIStop(IArgs* pargs, void *);
	bool OnAIEnable(IArgs* pargs, void*);
	void AIFire(float fTime);
	bool OnStartSkill(IArgs* pargs, void *);
	bool OnEndSkill(IArgs* pargs, void *);
	bool OnSkillHurt(IArgs* pargs, void *);
	bool OnEnmity(IArgs* pargs, void *);
	bool OnPlayerTranser(IArgs* pargs, void *);
	bool OnArmorBroken(IArgs* pargs, void *);
	bool OnWoozyArmorRecover(IArgs* pargs, void *);
	bool OnReceiveAIEvent(IArgs* pargs, void *);
	bool OnDeathEvent(IArgs* pargs, void *);

private:
	void UpdateVariable();
	void SetTreeVariable();
private:
	UINT32 _magic_num;
};

#endif
