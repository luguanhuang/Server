#ifndef __ROLE_H__
#define __ROLE_H__

#include "unit.h"
#include "roleserilizeinterface.h"
#include "roleinterfacedef.h"
#include "combatstatistics.h"
#include "poskeeper.h"
#include "saveroledatatimer.h"
#include "systeminfo.h"
#include <bitset>
#include "battle/ptcg2c_battleresultntf.h"
#include "clientconfinfo.h"
#include "gamelogic/rolefashiontimer.h"
#include "scene/ptcg2c_enterscenentf.h"
#include "crpc.h"
#include "roleview.h"
#include "define/itemdef.h"
#include "time.h"
#include "gamelogic/herobattlerecord.h"

namespace KKSG
{
	class RoleAllInfo;
	class Vec3;
	class TeamInvRoleInfo;
	class FriendList;
}

class Role;
class XRole;
class Guild;
class RoleNetSender; 
class RoleEventListener;
class RoleSkillSet;
struct XItem;
struct SceneInfo;
struct ClientLoadedInfo;
class PtcG2C_UnitAppear;

struct SavedEnterSceneNtf
{
	bool isfill;
	UINT32 sceneid;
	UINT32 sceneuid;
	PtcG2C_EnterSceneNtf ntf;
	
	SavedEnterSceneNtf()
	{
		isfill = false;
		sceneid = 0;
		sceneuid = 0;
	}
};

struct WatchLiveInfo
{
	WatchLiveInfo()
	{
		m_commendTime = 0;
		m_commendCount = 0;
	}
	UINT32 GetLeftCommendTime();
	bool AddCommendCount(int count);
	UINT32 GetCommendCount(){return m_commendCount;}
	void Update();

	int m_commendTime;
	int m_commendCount;
};

enum RoleStateFlag
{
	RSF_ISReconnecting,
	RSF_ISConnected,
	RSF_ISWaitLogin,
	RSF_ISGiveReward,
	RSF_ISSelectChest,
	RSF_ISAccountBriefChanged,
	RSF_ISRoleDataNeedSave,
	RSF_ISResetSkill,
	RSF_ISUseSupplement,
	RSF_ISTeamMatching,
	RSF_ISSceneSwitch_Verifying,		//场景切换检查中
	RSF_ISFirstEnterScene,
	RSF_ISFirstInit,
	RSF_ISLoginReconnect,				// 发起了登录重连
	RSF_ISLoginReconnectFinally,		// 登录重连结束
	RSF_ISLoadingScene,
};

class Role;
class DealWithAddExp
{
public:
	DealWithAddExp(Role* role, UINT32 level);
	~DealWithAddExp();
	UINT64 GetRealExp(UINT64 exp);
	void SetUseLevelSealBuff(bool use);
	void SetUseWorldLevelBuff(bool use);
private:
	Role* m_pRole;
	bool m_useLevelSealBuff;
	bool m_useWorldLevelBuff;
};


std::ostream &operator<<(std::ostream &stream, Role *role); 

class Role : public Unit, public RoleSerializeInterface
{
	friend class RoleNetSender;
	friend std::ostream &operator<<(std::ostream &stream, Role *role);
public:
	Role();
	~Role();

	void Init(UINT32 uTemplateID, const std::string &name);
	void Init(const KKSG::RoleAllInfo &roleAllInfo, SessionType sessionID, int index, const EnterSceneInfo& info);
	void InitSerializes();
	void Uninit();

	// 重连后同步场景
	void OnClientReconnect(UINT32 sceneid);
	void BeforeNotifyClientReconnect(UINT32 sceneid);
	void AfterNotifyClientReconnect(UINT32 sceneid);
	void OnLogin();
	void OnLogout(const char *msg, UINT32 dwParam = 0);

	bool OnFirstInit(UINT32 roleNum);

	void OnEnterGs();
	void OnLeaveGs(UINT32 dwParam);

	void OnTaskComplete(int TaskID);
	void OnSceneFailed(Scene* scene, bool isEndScene = true, bool isdraw = false);
	void OnSceneComplete(Scene* scene);
	void OnSceneDraw(Scene* scene);//平局
	void OnCompleteSceneWithPartner(Scene* pScene, std::vector<Role*>& partners, bool isWin);
	void OnSweepScene(SceneInfo *pSceneInfo);
	void OnPreEnterScene(Scene *scene);	

	void OnComposeJade(UINT32 jadeLevel, UINT32 count);
	void OnCompose();
	void OnDecompose();
	void OnBuy(UINT32 dwItemId);
	void OnShopBuy();
	void OnAuction();
	void OnHitWorldBoss();
	void OnHitGuildBoss();

	void OnWearEquip(UINT32 pos);
	void OnEnhanceEquip(UINT32 pos);
	void OnEnhanceTrans(UINT32 pos);
	void OnChangeJadeOnEquip(UINT32 pos);
	void OnForgeEquip(UINT32 pos);
	void OnEnchantEquip(UINT32 pos);
	void OnSmeltEquip(UINT32 pos);
	void OnFuseEquip(UINT32 pos);
	void OnUpgradeEquip(UINT32 pos);

	void OnWearEmblem(UINT32 pos);
	void OnSmeltEmblem(UINT32 pos);
	void OnWearArtifact(UINT32 pos);

	void OnWearFashion();
	void OnDisplayFashion();
	void OnMilitaryChange();
	void OnChangeVip();
	void OnAddFashion(UINT32 fashionId);
	void OnReinforce();
	void OnTeamStage(int type);
	void OnGuildActivity(int type);
	void OnSendFlower();
	void OnEnhanceFashion();
	void OnSkillUp();
	void OnGetItem(XItem* item);
	void OnChangeVirtualItem(UINT32 itemid, INT64 itemcount);
	void OnInGuild();
	void OnOutGuild();
	void OnSmelting();
	void OnSlotAttr();
	void OnChangeDesignation(UINT32 designationId);
	void OnChangeAchieveV2(UINT32 achievev2Id);
	void OnPay(UINT32 totalPay, UINT32 PayNum);
	void OnFirstPay();
	void OnQA(UINT32 type);
	void OnTitleLevelUp();
	void OnRecoverFatigue(int num, int lastRecoverTime, int thisReoverTime);
	void OnBuyAileen();
	void OnChangeSpriteInFight(UINT32 pos);
	void OnPetChange(UINT64 petUid);
	void OnGuildDare();
	void OnEnhanceMaster(UINT32 level);
	void OnLoginOneDay();
	void OnGetItemCountAndReason(UINT32 itemID, UINT32 count, int nReason, int nSubReason);
	void OnTakeItemCountAndReason(UINT32 itemID, UINT32 count, int nReason, int nSubReason);
	void OnSendFriendGift();
	void OnBuyIBShopItem();
	void OnChangePayMember(INT32 ID);
	void OnBuyPayMember(INT32 ID);
	void OnGuildInherit();
	void OnBuyGrouwthFund();
	void OnAddActivityPoint(UINT32 point);
	void OnChangePre();

	void OnAddScenePlayCount(KKSG::SceneType type, int num);
	void OnAddTeamCount(int teamType, int num);
	void OnFirstTimeEnterScene(Scene *scene);
	void OnLevelUp();

	SessionType GetSessionID() const;
	void SetSessionID(SessionType sessionID) { m_sessionID = sessionID; }
	UINT64 GetRoleID() const;
	const std::string &GetName() const;
	const KKSG::RoleAllInfo &GetRoleAllInfo();
	KKSG::RoleAllInfo *GetRoleAllInfoPtr() { return m_pRoleInfo; }
	inline XRole *GetXRole() { return m_pXRole; }

	void SetFlag(RoleStateFlag e, bool flag);
	bool GetFlag(RoleStateFlag e);

	bool CanSendTo();
	virtual void Send(const CProtocol &roPtc, bool force = false);
	bool SendToMS(const CProtocol& protocol);
	bool SendToMS(CRpc& rpc);
	void SendToCtrl(const CProtocol& protocol);
	void SendToCtrl(CRpc& rpc);
	void SendToRS(const CProtocol& protocol);
	void SendToRS(CRpc& rpc);
	virtual void Update(float delta);
	virtual void GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief = true);
	virtual void GetOutLook(KKSG::OutLook& outlook);
	virtual void StreamOutput(std::ostream &stream) { stream << this; }
	virtual void OnHurt(Unit* pCastUnit, double value);
	virtual void OnTreat(Unit *pTargetUnit, double value);
	virtual void OnAttack(Unit *pTargetUnit, double value);
	virtual void OnDied();
	virtual void OnRevive();

	bool CanRoleRevive();
	bool CanSceneRevive();
	bool CanReviveLimit();
	void RecoverBuff();

	virtual std::string GetAIString();
	virtual void GetAIArgs(AIDataArgs &args);

	void SetAllTuturial();
	inline void SetRecvedReward() { SetFlag(RSF_ISGiveReward, true); m_rewardString = ""; }
	inline Scene *GetLoadingScene() { return m_pLoadingScene; }
	inline void ClearLoadingScene() { m_pLoadingScene = NULL; }
	inline RoleNetSender *GetNetHandler() { return m_pNet; }
	RoleSkillSet *GetRoleSkillLevelMgr();
	inline UINT64 GetExp(){return m_exp;}
	inline UINT64 GetRealExp(){return m_realexp;}
	inline UINT64 GetMaxExp() { return m_maxExp;}
	inline void SetGM(bool isgm) { m_isgm = isgm; }
	inline bool IsGM() { return m_isgm; }
	
	inline UINT32 GetNickId() { return m_pRoleInfo->brief().nickid(); }
	inline const std::string& GetAccount() { return m_pRoleInfo->brief().accountid(); }
	inline PositionKeeper &GetPositionKeeper() { return m_PosKeeper; }

	template <typename T>
	T *Get() { return (T*)m_serializes[IndexOf<ROLEINTERFACETYPES, T>::value]; }
	template <typename T>
	void Set(T*p) { m_serializes[IndexOf<ROLEINTERFACETYPES, T>::value] = p; }
	bool IsSerializesInited() { return m_serializes.size() > 0; }

	int GetSkillPoint();
	void AddSkillPoint(UINT32 type, int n);
	void SetSkillPoint(UINT32 type, int n);
	void SkillCoolDown();
    void SkillCDAtLaunch();

	int  GetProfession();
	void SetProfession(int prof);
	bool ResetProfession();
	int  ChooseProfession(int Prof);

	void UpdateTutorial(UINT32 tutid);

	void AddExp(UINT64 exp);
	void AddRealExp(UINT64 exp);
	void CheckExp();
	void CheckAddExp();
	void Levelup();
	void LevelUpRapidly(UINT32 level);
	void DownLevel(int level);
	void DelExp(UINT64 exp);
	void SetCalcAddExpLevel(UINT32 level);
	UINT32 GetCalcAddExpLevel() { return m_calcAddExpLevel?m_calcAddExpLevel:m_level; }	

	bool GetLevelSealStatus();
	void CheckLevelSealStatus();

	void OnQAFinish();
	void OnThroughTower(int floor);
	void OnGetActivityChest(int index);
	void OnThroughDragon();

	void RecalcuateAttributeAndNotify(RecalAttrReason reason);

	void AddListener(RoleEventListener *listener);
	void RemoveListener(RoleEventListener *listener);

	void SaveToTeamMember(KKSG::TeamMember *pMemberData);

	void SetRewardString(const std::string &str) { m_rewardString = str; }
	const char *GetRewardString() { return m_rewardString.c_str(); }

	UINT32 GetLoginTime() { return GetRoleAllInfo().extrainfo().lastlogintime(); }
	UINT32 GetLogoutTime() { return GetRoleAllInfo().extrainfo().lastlogouttime(); }
	time_t GetEnterSceneTime() { return m_enterSceneTime; }
	UINT64 GetCreateTime(); 
	time_t GetAntiAddictionTime() { return m_pRoleInfo->idiprecord().lastsendantiaddictiontime(); }
	bool GetSendAntiAddictionRemind() { return m_pRoleInfo->idiprecord().issendantiaddictionremind(); }
	UINT32 GetAntiAddictionRemindCount() { return m_pRoleInfo->idiprecord().antiaddictionremindcount(); }
	UINT32 IsGetHg() { return m_pRoleInfo->idiprecord().isgethg(); }
	void SetAntiAddictionTime(UINT32 tTime);
	void SetSendAntiAddictionRemind(bool isRemind);
	void InitAntiAddictionInfo();
	void SetAntiAddictionRemindCount(UINT32 count);
	void SetAntiAddictionHg(INT32 type, UINT32 hgBanTime, UINT32 hgGameTime);
	void CheckAntiAddictionHg();
	bool GetIsForceExit() { return m_IsForceExit; }
	void SetIsForceExit(bool Isforce) { m_IsForceExit = Isforce;}
	void SetIsOpenHg(bool isGetHg);
	void SetPicUrl(std::string strPicUrl);

	UINT32 GetOnlineTime() { return m_pRoleInfo->mutable_brief()->onlimetime(); }
	void UpdateOnlineTime();
	UINT32 GetOnlineTimeFromLogin();

	CombatStatistics &Statistics() { return *m_statistics; }
	void SetStatistics(CombatStatistics& statitics) { *m_statistics = statitics; }
	const CombatStatistics &GetStatistics() { return *m_statistics; }

	void HintNotify(UINT32 systemid, bool isremove);

	bool SetConfig(const std::string& type, const std::string& value);
	void DelConfig(const std::string& type);
	void ReConfig();
	const std::string GetConfig(const std::string& type)const;

	virtual bool AutoDestory() { return false; }
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);
	virtual UINT32 GetSkillCasterTypeID();
	virtual UINT32 GetTag() { return 1; }

	void DoAllSave();

	virtual void OnEnterScene(Scene *scene);
	void OnLoginReconnect(Scene* scene);
	virtual void OnLeaveScene();
	virtual void ResetFightGroup();

	void SetCamp(UINT32 campID){m_campID = campID;}
	UINT32 GetCamp(){return m_campID;}	

	const KKSG::ClientInfo& GetClientConf() const { return m_cliConf; }
	void SetClientConf(const KKSG::ClientInfo& roConfInfo);

	KKSG::StartUpType GetStartUpType();
	inline KKSG::LoginType GetLoginType() { return m_cliConf.logintype(); }

	UINT32 GetChatID() const { return m_uChatID; }
	void SetChatInfo(const KKSG::ChatInfo& chatinfo);
	void SetDestChatList(UINT32 uChatID, std::vector<UINT64> dest);
	void SendChatToMS(unsigned char* msg_, unsigned int msg_len_, UINT32 uChatID);

	void SetSensitiveStatus(UINT32 uFlag){uSensitiveStatus = uFlag;}
	const UINT32 GetSensitiveStatus(){ return uSensitiveStatus;}
	bool IsClientAutoFight() {return m_ClientAutoFight;}
	void SetClientAutoFight(bool value) {m_ClientAutoFight = value;}

	//获取上一次在指定频道发言的时间: 如果没有发言记录，则返回0
	UINT32 GetChannelChatTime( UINT32 channel );
	void SetChannelChatTime( UINT32 channel, UINT32 time );

	void FillTeamInviteRoleInfo(KKSG::TeamInvRoleInfo* info, int ttype);

	bool IsInBattle();
	UINT64 getGuildId();
	UINT32 GetGuildLevel();
	//void CheckGuildDareRewardNtf();

	bool IsWatcher(){return m_isWatcher;}
	void SetWatcher(bool isWatcher){m_isWatcher = isWatcher;}

	WatchLiveInfo& GetWatchLiveInfo(){return m_watchLiveInfo;}

	UINT32 GetAccountToday(); //> 获取当天结算次数
	UINT32 AddOneAccountNow(); //> 现在结算后的当天结算次数

	bool IsFamiliar(UINT64 roleID);

	int  GetLoginSceneID(UINT32 roleNum);

	void ClearEnterNtf() { m_enterntf.isfill = false; }
	void SetEnterNtf(PtcG2C_EnterSceneNtf& ntf, UINT32 sceneid, UINT32 sceneuid);
	PtcG2C_EnterSceneNtf* GetEnterNtf(UINT32 sceneid);
	bool DoEnterNtf(Scene* scene, UINT32 destid);

	bool IsFriend(UINT64 roleid) { return m_friendlist.find(roleid) != m_friendlist.end(); }
	void AddFriend(UINT64 friendid);
	void DelFriend(UINT64 friendid);
	void FillFriendList(const KKSG::FriendList& list);
	const std::set<UINT64>* GetFriendList() { return &m_friendlist; }

	bool IsSystemRewardTaken(KKSG::SystemRewardType type);
	void OnTakenSystemReward(KKSG::SystemRewardType type);

	void SaveDataOnGsShutdown();

	PtcG2C_BattleResultNtf BattlePtc;

	void DoSetProfession(int prof);

	void NoticeNameChange();

	//slow update
	void UpdatePerSecond();
	void UpdatePerMinute();

	void UpdateOnDayPass();

	bool CheckIdipAndNotify(KKSG::PunishType type);
	void SetPayMemberID(UINT32 ID) { m_PayMemberID = ID;}

	void SetRouterConnId(UINT32 connId) { m_routerConnId = connId; }
	UINT32 GetRouterConnId() { return m_routerConnId; }
	UINT32 GetServerId() { return m_serverId; }
	UINT32 GetFromServerId() { return m_fromServerId; }
	
	const KKSG::OutLookState& GetState() { return m_state; }
	void SetStateRaw(KKSG::OutLookStateType stateType, UINT32 param, UINT64 paramother);
	void SetState(KKSG::OutLookStateType stateType, UINT32 param = 0, bool isForce = false, UINT64 paramother = 0);
	bool StateCanChangeTo(KKSG::OutLookStateType nextState);
	
	///> 变身
	bool Transform(UINT32 id, UseHeroType type);

	void SetChangingPro(bool ischanging) { m_ischangingpro = ischanging; }
	bool IsChangingPro() { return m_ischangingpro; }

	void SetBeginChangingPro(bool ischanging) { m_isbeginchangingpro = ischanging; }
	bool IsBeginChangingPro() { return m_isbeginchangingpro; }

	void SendServerOpenDay();

	inline RoleView* GetRoleView() { return m_roleview; }	

	const std::string& SetBattleStamp();
	const std::string& GetBattleStamp() { return m_battleStamp; }
	bool IsSameBattleStamp(const std::string& stamp);

	//一级游戏币钻石管理;
	bool AddDiamond(INT32 iCount, INT32 reason, INT32 subreason, const std::string &param="");//获得游戏币;
	bool ConsumeDiamond(KKSG::PayConsumeBase& info, std::vector<ItemDesc>& item);//消耗游戏币或者item;

	void ClearFieldChanged() { m_oFieldChanged.clear(); }
	const CDataFieldSet& GetFieldChanged() const { return m_oFieldChanged; }

	void SetLeavingScene(bool flag) { m_leavingScene = flag; }
	bool IsLeavingScene() { return m_leavingScene; }
	void SetLockTime();
	inline UINT32 GetLockTime() { return m_nLockTime;}
	inline void ClearLockTime() { m_nLockTime = 0;}

	void SetOffFashionStamp() { m_offfashionstamp = (UINT32)time(NULL); }

	void SetChangeSameScene(bool istrue) { m_ischangetosamescene = istrue; }

	void FillCrossData(KKSG::RoleCrossData& data);
	void SetCrossData(const KKSG::RoleCrossData& data);

	void SetUseWeddingFashion(bool use = true) { m_useWeddingFashion = use; }

	void OnTakeBackFlow();
	bool IsBackFlowRole();
	bool IsBackFlowSmallRole();
	void NotifyBackFlow();

	UINT32 GetBackFlowEndTime();

	bool IsKingBack();

	void DoChangeSceneLog(INT32 type);

	void ErrorCodeNtf(UINT32 errorno, bool istip);
	bool IsAccountBackFlow() { return m_accountBackFlow; }
	void SetAccountBackFlow(bool back) { m_accountBackFlow = back; }

	void SetTemplateData();

	void OnOtherAppear(UINT64 otherroleid);

	void SendWorldLevelNtf();

private:
	bool		m_ischangingpro;
	bool		m_isbeginchangingpro;
	bool		m_isgm;
	UINT64      m_exp;
	UINT64      m_maxExp;
	UINT64		m_realexp;
	UINT32		m_campID;
	UINT32		m_nLockTime;
	bool        m_ischangetosamescene;

	UINT32 uSensitiveStatus; ///> 0:未检查 1：检查通过 2：检查未通过

	UINT32 m_offfashionstamp;

	SessionType m_sessionID;
	KKSG::ClientInfo m_cliConf;
	KKSG::OutLookState m_state;
	CombatStatistics* m_statistics;
	KKSG::RoleAllInfo* m_pRoleInfo;

	XRole* m_pXRole;
	std::bitset<32> m_flags;

	// scene
	Scene* m_pLoadingScene;
	RoleView* m_roleview;
	PositionKeeper m_PosKeeper;
	SavedEnterSceneNtf m_enterntf;

	RoleNetSender     *m_pNet;
	SaveRoleDataTimer  m_Timer;
	RoleFashionTimer   m_FashionTimer;

	UINT32 m_onlinetime;
	time_t m_loginTime;
	time_t m_enterSceneTime;
	std::string m_rewardString;

	UINT32 m_uChatID;
	std::map<UINT32, KKSG::ChatInfo> m_achat;

	bool m_isWatcher;
	WatchLiveInfo m_watchLiveInfo;
	
	UINT32 m_accountNumberLastDay; //结算次数如果为0，时间无意义
	UINT64 m_lastAccountTime; 
	UINT32 m_lastFMAudioTime;	

	// friend
	std::set<UINT64> m_friendlist;

	UINT64 m_qwPrevSecTick;
	UINT64 m_qwPrevMinTick;
	UINT32 m_routerConnId;		// 跨服玩家从哪个router进来
	UINT32 m_serverId;			// 玩家入口serverid
	UINT32 m_fromServerId;		// 配置serverid

	UINT32 m_PayMemberID;
	bool m_ClientAutoFight;

	std::string m_battleStamp;
	UINT32 m_calcAddExpLevel;

	bool m_leavingScene;
	CDataFieldSet	m_oFieldChanged;

	std::list<RoleEventListener*> m_eventListener;
	std::vector<RoleSerializeInterface*> m_serializes;

	KKSG::RoleCrossData m_crossData;
	bool m_useWeddingFashion;

	bool m_tempbackflowmark;
	bool m_accountBackFlow;

	bool m_IsForceExit;
};


#endif // __ROLE_H__
