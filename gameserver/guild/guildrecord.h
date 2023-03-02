#ifndef __GUILDRECORD_H__
#define __GUILDRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "unit/combatattribute.h"
#include "guild/guildconfig.h"

namespace KKSG
{
	class RoleAllInfo;
	class GuildRecord;
}
class Role;

struct SOnlyOnceGuildBonusData
{
	SOnlyOnceGuildBonusData()
	{
		bonusType = 0;
		bonusVar = 0;
	}
	UINT64 GetKey(){return ((UINT64)bonusType << 32) + bonusVar;}

	UINT32 bonusType;
	UINT32 bonusVar;
};

struct GuildBonusData
{
	GuildBonusData()
	{
		m_askBonusTime = 0;
		m_getCheckInBonusNum = 0;
	}
	
	void OnDayPass()
	{
		m_askBonusTime = 0;
		m_getCheckInBonusNum = 0;
		m_gotGuildBonusDayNum.clear();
	}

	void Load(const KKSG::GuildRecord* record)
	{
		m_askBonusTime = record->askbonustime();
		m_getCheckInBonusNum = record->getcheckinbonusnum();
		if (record->has_bonusdata())
		{
			for (int i = 0; i < record->bonusdata().sentguildbonus_size(); ++i)
			{
				SOnlyOnceGuildBonusData tempData;
				tempData.bonusType = record->bonusdata().sentguildbonus(i).bonustype();
				tempData.bonusVar = record->bonusdata().sentguildbonus(i).bonusvar();
				m_onceBonusData.insert(std::make_pair(tempData.GetKey(), tempData));
			}
			for (int i = 0; i < record->bonusdata().gotguildbonusdaynum_size(); ++i)
			{
				UINT32 bonusTemplateID = record->bonusdata().gotguildbonusdaynum(i).key();
				UINT32 bonusNum = record->bonusdata().gotguildbonusdaynum(i).value();
				m_gotGuildBonusDayNum.insert(std::make_pair(bonusTemplateID, bonusNum));
			}
			for (int i = 0; i < record->bonusdata().gotguildbonustotalnum_size(); ++i)
			{
				UINT32 bonusTemplateID = record->bonusdata().gotguildbonustotalnum(i).key();
				UINT32 bonusNum = record->bonusdata().gotguildbonustotalnum(i).value();
				m_gotGuildBonusTotalNum.insert(std::make_pair(bonusTemplateID, bonusNum));
			}
		}

	}
	void Save(KKSG::GuildRecord* record)
	{
		record->set_askbonustime(m_askBonusTime);
		record->set_getcheckinbonusnum(m_getCheckInBonusNum);
		for (auto iter = m_onceBonusData.begin(); iter != m_onceBonusData.end(); ++iter)
		{
			KKSG::RoleGuildBonusData* bonusData = record->mutable_bonusdata();
			KKSG::OnlyOnceGuildBonusData* oneBonus = bonusData->add_sentguildbonus();
			oneBonus->set_bonustype(iter->second.bonusType);
			oneBonus->set_bonusvar(iter->second.bonusVar);
		}
		for (auto iter = m_gotGuildBonusDayNum.begin(); iter != m_gotGuildBonusDayNum.end(); ++iter)
		{
			KKSG::RoleGuildBonusData* bonusData = record->mutable_bonusdata();
			KKSG::MapKeyValue* oneBonus = bonusData->add_gotguildbonusdaynum();
			oneBonus->set_key(iter->first);
			oneBonus->set_value(iter->second);
		}
		for (auto iter = m_gotGuildBonusTotalNum.begin(); iter != m_gotGuildBonusTotalNum.end(); ++iter)
		{
			KKSG::RoleGuildBonusData* bonusData = record->mutable_bonusdata();
			KKSG::MapKeyValue* oneBonus = bonusData->add_gotguildbonustotalnum();
			oneBonus->set_key(iter->first);
			oneBonus->set_value(iter->second);
		}
	}

	UINT32 m_askBonusTime;
	UINT32 m_getCheckInBonusNum;
	std::map<UINT32, UINT32> m_gotGuildBonusDayNum;
	std::map<UINT32, UINT32> m_gotGuildBonusTotalNum;
	std::map<UINT64, SOnlyOnceGuildBonusData> m_onceBonusData;
};

class CGuildRecord : public RoleSerializeInterface
{
public:
	CGuildRecord(Role* role);
	virtual ~CGuildRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Convert2_KKSGGuildRecord(KKSG::GuildRecord* record);
	void Reset();
	void Update();

	//inline UINT64 GetGuildId() { return m_guildid; }
	//inline void SetGuildId(UINT64 guildId) { m_guildid = guildId; m_switch.Set();}
	inline UINT32 GetRecvFatigue() { return m_recvFatigue; }
	inline UINT32 GetPlayCount() { return m_cardplaycount; }
	inline UINT32 GetChangeCount() { return m_cardchangecount; }
	inline UINT32 GetBuyChangeCount() { return m_cardbuychangecount; }
	inline UINT32 GetCheckin() { return m_checkin; }
	inline UINT32 GetBoxMask() { return m_boxmask; }
	inline const std::set<UINT32>& getDareReward() { return m_dareReward; }   
	inline UINT32 GetRecDareRewardNum() { return m_dareReward.size(); }
	inline bool hasRewardDare(UINT32 rewardType) { return m_dareReward.find(rewardType) != m_dareReward.end() ? true : false; }
	inline const std::map<UINT32, UINT32>& GetGuildSkill() { return m_guildSkillMap; }
	inline void GuildSkillUp(UINT32 skillId) { m_guildSkillMap[skillId] += 1; m_switch.Set();}
	inline UINT32 GetGuildSkillLvl(UINT32 skillId) { return m_guildSkillMap.find(skillId) == m_guildSkillMap.end() ? 0 : m_guildSkillMap[skillId]; }
	inline std::vector<UINT32>& GetCards() { return m_cards; }
	inline bool IsHintCard() { return m_ishintcard; }
	inline void SetHintCard(bool ishint) { m_ishintcard = ishint; m_switch.Set(); }
	inline UINT32 GetAskBonusTime() {return m_guildBonusData.m_askBonusTime;}
	inline void SetAskBonusTime(int time) {m_guildBonusData.m_askBonusTime = time; m_switch.Set();}
	inline UINT32 GetCheckInBonusNum() {return m_guildBonusData.m_getCheckInBonusNum;}
	inline void AddCheckInBonusNum(int num) {m_guildBonusData.m_getCheckInBonusNum += num; m_switch.Set();}
	inline void DebugResetCheckInBonusNum() {m_guildBonusData.m_getCheckInBonusNum = 0;m_guildBonusData.m_askBonusTime = 0; m_switch.Set();}
	inline UINT32 GetInheritTeaTime() { return m_guildInheritTeaTime; }
	inline UINT32 GetInheritStuTime() { return m_guildInheritStuTime; }
	inline void AddInheritTeaTime() { ++m_guildInheritTeaTime; m_switch.Set(); } 
	inline void AddInheritStuTime() { ++m_guildInheritStuTime; m_switch.Set(); } 
	inline UINT32 GetGuildInheritCDTime() { return m_guildInheritCDTime; }
	inline void SetGuildInheritTime(UINT32 nowTime) { m_guildInheritCDTime = nowTime; m_switch.Set(); }
	inline UINT32 GetTeacherInheritTime() { return m_teacherInheritTime; }
	inline void AddTeacherInheritTime() { m_teacherInheritTime++; m_switch.Set(); }

	UINT32 AddGetBonusNum(UINT32 bonusTemplateID, UINT32 num);
	UINT32 CheckAddGetBonusNum(const GuildBonusTable::RowData* data, UINT32 bonusTemplateID, UINT32 num);
	bool CheckSendGuildBonus(UINT32 bonusType, UINT32 bonusVar);
	void DebugResetGuildBonus();
	const GuildBonusData& GetGuildBonusData(){return m_guildBonusData;}

	bool IsCountLimit(UINT32 count);
	bool IsChangeLimit(UINT32 count);
	bool IsCheckin();
	bool IsBoxTaken(UINT32 index);
	void AddPlayCount();
	void AddChangeCount();
	void AddBuyChangeCount();
	void AddRecvFatigue();
	void AddRecvFatigue(UINT32 addNum);
	void AddDareReward(UINT32 rewardType);
	//void ClearDareRewardTimes();
	void SetCheckin(UINT32 checkin);
	void SetBoxMask(UINT32 index);
	UINT64 GetCardMatch() { return m_cardmatchid; }
	void SetCardMatch(UINT64 id) { m_cardmatchid = id; m_switch.Set(); }
	bool IsNoCardMatch() { return 0 == m_cardmatchid; }
	UINT32 RandomStore();
	UINT32 ChangeCard(UINT32 card);
	UINT32 EndCard(Role* role, UINT64 guildid = 0);
	void ResetCardCount();
	void ApplyEffect(CombatAttribute *combatAttr);
	void HintNotify(Role* pRole);

	UINT32 GetPartyRewardCount(UINT32 reward_id);
	void AddPartyRewardCount(UINT32 reward_id);
	void GetPartyRewardInfo(std::map<UINT32,UINT32>& reward_info);
private:
	Role* m_pRole;
	UINT64 m_guildid;  
	Switch m_switch;
	UINT32 m_updateday;
	bool m_ishintcard;
	UINT64 m_cardmatchid;
	UINT32 m_cardplaycount;
	UINT32 m_cardchangecount;
	UINT32 m_cardbuychangecount;
	std::vector<UINT32> m_cards;
	std::vector<UINT32> m_cardstore;
	UINT32 m_checkin;
	UINT32 m_boxmask;
	UINT32 m_recvFatigue;
	//UINT32 m_askBonusTime;
	//UINT32 m_getCheckInBonusNum;
	UINT32 m_guildInheritTeaTime;
	UINT32 m_guildInheritStuTime;
	UINT32 m_guildInheritCDTime;
	UINT32 m_teacherInheritTime;
	std::set<UINT32> m_dareReward;            // 公会挑战领奖记录 
	std::map<UINT32, UINT32> m_guildSkillMap; // 公会技能学习
	//std::map<UINT64, SOnlyOnceGuildBonusData> m_guildBonusData;

	std::unordered_map<UINT32,UINT32> party_rewards_;

	GuildBonusData m_guildBonusData;
};

#endif