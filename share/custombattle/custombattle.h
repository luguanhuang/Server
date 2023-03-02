#ifndef __CUSTOMBATTLE_H__
#define __CUSTOMBATTLE_H__

#include "custombattledef.h"

class CustomBattle
{
public:
	CustomBattle();
	CustomBattle(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid);
	virtual ~CustomBattle();

	static UINT64 NewID(UINT32 serverid);

	virtual bool IsIn(UINT64 roleid) = 0;
	virtual void AddRole(UINT64 roleid, const std::string& name) = 0;
	virtual void DelRole(UINT64 roleid) = 0;
	virtual void AddPoint(UINT64 roleid, int point) = 0;

	virtual void Update() { };
	virtual bool StartNow() { return false; };
	virtual bool IsCD(UINT64 roleid) { return false; }
	virtual void ClearCD(UINT64 roleid) { };

	virtual void DoReward(UINT64 roleid) { };
	virtual UINT32 GetRank(UINT64 roleid) { return (UINT32)(-1); };
	virtual UINT32 GetPoint(UINT64 roleid) { return (UINT32)(-1); };
	virtual KKSG::ErrorCode CanReward(UINT64 roleid) { return KKSG::ERR_FAILED; };
	virtual KKSG::ErrorCode CanJoin(const std::string& password) { return KKSG::ERR_FAILED; }

	UINT32 Size() { return m_ranks.size(); }
	const KKSG::CustomBattleData& GetData() { return m_data; }

	void SetID(UINT64 uid) { m_data.set_uid(uid); }
	UINT64 GetID() { return m_data.uid(); }
	UINT64 GetCreator() { return m_data.config().creator(); }
	UINT32 GetScaleMask() { return m_data.config().scalemask(); }
	UINT32 GetTagMask() { return m_data.config().tagmask(); }
	UINT32 GetConfigId() { return m_data.config().configid(); }

	bool IsFair() { return m_data.config().isfair(); }
	bool IsSystem() { return m_data.config().issystem(); }
	bool IsPvpTwo() { return m_data.config().fighttype() == KKSG::CustomBattle_PKTwo_Normal; }
	bool IsGM() { return ((1 << KKSG::CustomBattle_Tag_GM) & GetTagMask()) > 0; }
	bool CanCreatorUnJoin() { return m_data.config().state() == KKSG::CustomBattle_End; }
	bool IsFriendOnly() { return ((1 << KKSG::CustomBattle_Scale_Friend) & GetScaleMask()) > 0; }
	bool IsGuildOnly() { return ((1 << KKSG::CustomBattle_Scale_Guild) & GetScaleMask()) > 0; }
	bool IsFull() { return Size() >= m_data.config().canjoincount(); }
	bool IsDestory() { return KKSG::CustomBattle_Destory == m_data.config().state(); }
	bool IsEnd() { return KKSG::CustomBattle_End <= m_data.config().state(); }

	// password
	void SetPassword(const std::string& password);
	const std::string GetPassword() { return m_data.config().password(); }
	bool HasPassword() { return !m_data.config().password().empty(); }

	const KKSG::CustomBattleState GetState() { return m_data.config().state(); }
	void SetState(KKSG::CustomBattleState state);

	virtual bool FillRole(UINT64 uid, UINT64 roleid, KKSG::CustomBattleRole& datarole) { return false; }
	virtual bool FillMatch(UINT64 uid, UINT64 roleid, KKSG::CustomBattleMatchInfo& info) { return false; }

	KKSG::ErrorCode CanMatch();

	bool IsChanged() { return m_ischanged; }
	void SetChanged() { m_ischanged = true; }
	void ResetChanged() { m_ischanged = false; }

	void ToKKSGBrief(KKSG::CustomBattleData* data);
	void ToKKSGWithRank(KKSG::CustomBattleData* data);
	void ToKKSGDB(KKSG::CustomBattleData* data);
	void ToThis(const KKSG::CustomBattleData& data);

	static bool IsLocalCreate(const KKSG::CustomBattleConfig& conf); 

protected:
	bool m_ischanged;
	KKSG::CustomBattleData m_data;
	struct RankCompare
	{
		bool operator()(const KKSG::CustomBattleRank& ranka, const KKSG::CustomBattleRank& rankb) const
		{
			if (ranka.point() != rankb.point())
			{
				return ranka.point() > rankb.point();
			}
			else
			{
				if (ranka.timestamp() != rankb.timestamp())
				{
					return ranka.timestamp() < rankb.timestamp();
				}
				else
				{
					return ranka.roleid() < rankb.roleid();	
				}
			}
		}
	};
	std::set<KKSG::CustomBattleRank, RankCompare> m_ranks;
	std::map<UINT64, std::set<KKSG::CustomBattleRank, RankCompare>::iterator> m_roleid2rank;
};

///////////////////////////////////////////////////////////////////////////
///> 赏金模式PK
class CustomBattleSystemPk : public CustomBattle
{
public: 
	CustomBattleSystemPk(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid);
	virtual ~CustomBattleSystemPk();

	virtual bool IsIn(UINT64 roleid);
	virtual void AddRole(UINT64 roleid, const std::string& name);
	virtual void DelRole(UINT64 roleid);
	virtual void AddPoint(UINT64 roleid, int point);
};

///////////////////////////////////////////////////////////////////////////
///> 自定义模式PK/PKTwo
class CustomBattlePk : public CustomBattle
{
public: 
	CustomBattlePk();
	CustomBattlePk(const KKSG::CustomBattleConfig& config, UINT64 roleid, UINT32 serverid);
	virtual ~CustomBattlePk();

	virtual bool IsIn(UINT64 roleid);
	virtual void AddRole(UINT64 roleid, const std::string& name);
	virtual void DelRole(UINT64 roleid);
	virtual void AddPoint(UINT64 roleid, int point);

	virtual void Update();
	virtual bool StartNow();
	virtual void ClearCD(UINT64 roleid) ;
	virtual bool IsCD(UINT64 roleid);

	virtual KKSG::ErrorCode CanReward(UINT64 roleid);
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetPoint(UINT64 roleid);
	virtual void DoReward(UINT64 roleid);

	virtual bool FillRole(UINT64 uid, UINT64 roleid, KKSG::CustomBattleRole& datarole);
	virtual bool FillMatch(UINT64 uid, UINT64 roleid, KKSG::CustomBattleMatchInfo& info);

	void AddInfo(const KKSG::CustomBattleRank& info);

	virtual KKSG::ErrorCode CanJoin(const std::string& password);
};

#endif