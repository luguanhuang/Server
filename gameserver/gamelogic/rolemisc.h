#ifndef _ROLE_MISC_H_
#define _ROLE_MISC_H_

#include "unit/roleserilizeinterface.h"
#include "unit/combatattribute.h"

class Role;

class RoleMisc : public RoleSerializeInterface
{
public:
	RoleMisc(Role *pRole);
	~RoleMisc();
	
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	void SetSaveFlag();

	void OnDayPass();
	void OnHallEnterScene();
	void OnLogin();

	inline void SetLastPush(UINT32 nVal){	m_nPushFlag = nVal; }
	inline UINT32 GetLastPush(){	return m_nPushFlag; }

	inline void SetLastTime(UINT32 nVal){	m_nPushTime = nVal; }
	inline UINT32 GetLastTime(){	return m_nPushTime; }


	inline void SetLadderTime(UINT32 nVal){	m_nLadderTime = nVal; }
	inline UINT32 GetLadderTime(){	return m_nLadderTime; }

	////////////////////////////////////////////////////////////
	inline void SetAnswersIndex(UINT32 nVal) {m_nAnswersIndex =nVal; }
	inline UINT32 GetAnswersIndex() { return m_nAnswersIndex;}
	inline void SetAnswersVersion(UINT32 nVal) { m_nAnswersVersion = nVal;}
	inline UINT32 GetAnswersVersion() { return m_nAnswersVersion;}
	inline void SetHintFlag(UINT32 nHint) {m_nHintFlag = nHint; }
	inline UINT32 GetHintFlag(){return  m_nHintFlag;}

	inline UINT32 GetDayGiftItem() { return m_daygiftitems;}
	void SetDayGiftItem(UINT32 daygiftitems) {m_daygiftitems =daygiftitems;  SetSaveFlag();}
	////////////////////////这样不好下个版本结构体话好点///////////////////////////////////

	bool ChangeProCountLimit();
	bool ChangeProTimeLimit();

	void SetChangeProTime(UINT32 time) { m_lastchangeprotime = time; SetSaveFlag(); }
	void AddChangeProCount() { ++m_changeprocount; SetSaveFlag(); }

	UINT32 GetDailyLBNum() { return m_dailyLbnum; }
	void AddDailyLBNum(UINT32 value = 1);

	void SetDeclaration(std::string declaration) { declaration_ = declaration; SetSaveFlag();}
	std::string GetDeclaration() { return declaration_;}

	void OnLevelUp();

	void CheckQQVipHint();
	void NotifyQQVipHint();
	void ReadQQVipHint();

	void CheckEGameHint();
	void ReadEGameHint();

	void CheckXinYueHint();
	void ReadXinYueHint();

	const UINT32 GetLoginActTime();
	const bool GetLoginActStatus();
	void SetLoginActTime(const UINT32 nTime) { m_loginacttime = nTime;  SetSaveFlag();}
	void SetLoginActStatus(const bool status) { m_loginactstatus = status; SetSaveFlag(); }

	UINT32 GetPassedHardestNestExpID(){return m_hardestNestExpID;}
	void CheckHardestNestExpID(UINT32 nestExpID);

	KKSG::StartUpType GetStartUpType();
	void SetStartUpType(KKSG::StartUpType type);

	bool CheckAndAddWeddingFlowerCount();
	bool CheckAndAddWeddingFireworksCount();
	bool CheckAndAddWeddingCandyCount();

	UINT32 SurviveGetPoint();
	void SurviveAddPoint(int addp);
	UINT32 SurviveGetTopCount();
	void SurviveAddTopCount();
	bool SurviveGetRewardFlag();
	void SurviveSetRewardFlag();
	void SurviveUpdate();
	void SurviveInfoPrint();

	UINT32 GetKingBackRewardCount();
	void AddKingBackRewardCount();

	void ApplyGMAttr(CombatAttribute* attrs);
	bool AddGMAttr(CombatAttrDef attrDef, double value);
	void ClearGMAttr();
	void ListGMAttr(std::string& out);

	void InitLuckyActivity(const std::vector<KKSG::ItemRecord>& all);
	void DeleteOldSpAct(UINT32 actid);
	const KKSG::LuckyActivity GetLuckyActicityInfo();
	const UINT32 GetPriceState();
	const bool GetIsPay();
	void UpdateIsPay(bool ispay = false);
	void GetRoleItemRecord(std::vector<KKSG::ItemRecord>& tmp);
	void UpdateAfterLottery(UINT32 index);
	bool IsConfigModify();

	void SetFreeFlowHintTime(UINT32 t);

	void StartChargeBack(UINT32 payCnt);
	void CheckChargeBack();

private:
	Switch m_dirty;
	Role *m_role;

	
	UINT32 m_nPushFlag;
	UINT32 m_nPushTime;
	UINT32 m_nLadderTime;
	//答题问卷
	UINT32 m_nAnswersIndex;
	UINT32 m_nAnswersVersion;
	UINT32 m_nHintFlag;

	///> 变职业
	UINT32 m_changeprocount;
	UINT32 m_lastchangeprotime;

	UINT32 m_dailyLbnum;	// 今日参加联赛积分赛的次数
	UINT32 m_updateTime;


	std::string declaration_;
	bool m_qqVipHint;
	UINT32 m_qqVipReadTime;

	bool m_eGameHint;
	UINT32 m_eGameReadTime;

	bool m_XinYueHint;
	UINT32 m_XinYueReadTime;

	UINT32 m_lastLevel;

	UINT32 m_loginacttime;
	bool m_loginactstatus;
	UINT32 m_daygiftitems;
	UINT32 m_abysspartymail;

	UINT32 m_hardestNestExpID;
	
	KKSG::StartUpType m_startUpType;
	UINT32 m_startUpTypeTime;

	UINT32 m_weddingFlowerCount;
	UINT32 m_weddingFireworksCount;
	UINT32 m_weddingCandyCount;
	KKSG::LuckyActivity m_turntable;

	KKSG::SurviveRecord m_surviveData;
	UINT32 m_kingBackReward;

	///> GM属性
	std::map<UINT32, double> m_id2gmattr;
	UINT32 m_freeHintTime;

	UINT32 m_chargeBackRewardLevel;
	UINT32 m_chargeBackTotal;
};

#endif