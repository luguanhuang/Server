#ifndef _GUILD_DONATE_H_
#define _GUILD_DONATE_H_

#include "guild/guilddef.h"
#include "guild.h"
#include "role/switch.h"
#include "guild/guildcamprank.h"
#include "guildlistener.h"
#include "pb/project.pb.h"
#include "ranklist/ranklist.h"


struct MemberAskInfo
{
	MemberAskInfo(): id(0), roleId(0), publishTime(0), itemId(0), needCount(0), getCount(0),
					 level(0), quality(0), taskId(0), tasktype(KKSG::PeriodTaskType_Daily),
					 itemType(0), itemQuality(0), index(0), donating(false) {}

	UINT32 id;				// 唯一id
	UINT64 roleId;			// 请求者id
	UINT32 publishTime;		// 发布时间
	UINT32 itemId;			// 物品id
	UINT32 needCount;		// 请求个数
	UINT32 getCount;		// 已经获得个数
	UINT32 level;			// 等级
	UINT32 quality;			// 品质
	UINT32 taskId;			// 对应任务id
	KKSG::PeriodTaskType tasktype;		// 所属的任务类型(日常任务、周任务)
	UINT32 itemType;
	UINT32 itemQuality;
	UINT32 index;

	bool donating;			// 正在捐献，避免两个人同时捐献，异步原因导致捐多了

	void FromKKSG(const KKSG::GuildMemberAskItem& info);
	void ToKKSG(KKSG::GuildMemberAskItem& info);
};

struct MemberDonateInfo
{
	UINT64 roleId;
	UINT32 todayDonateCount;
	UINT32 totalDonateCount;
	UINT32 lastDonateTime;

	void FromKKSG(const KKSG::GuildMemberDonateInfo& info);
	void ToKKSG(KKSG::GuildMemberDonateInfo& info);
};

struct DonateRankInfo
{
	UINT64 roleId;
	UINT32 value;
	UINT32 time;
};

struct DonateRankInfoCmp
{
	bool operator()(const DonateRankInfo& lhs, const DonateRankInfo& rhs)
	{
		if (lhs.value != rhs.value)
		{
			return lhs.value > rhs.value;
		}
		if (lhs.time != rhs.time)
		{
			return lhs.time < rhs.time;
		}
		if (lhs.roleId != rhs.roleId)
		{
			return lhs.roleId < rhs.roleId;
		}
		return false;
	}
};

class GuildDonate : public IGuildSerializerInterface, public IGuildListener
{
public:
	GuildDonate(Guild* guild);
	virtual ~GuildDonate();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) {}
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) {}

	UINT32 AddAskItemInfo(MemberAskInfo& info);
	MemberAskInfo* GetAskInfo(UINT32 id);
	MemberAskInfo* GetAskInfo(UINT64 roleId, UINT32 index, UINT32 taskId = 0);
	void RemoveAskInfo(UINT32 id);
	void RemoveAskInfo(UINT64 roleId, UINT32 taskId);
	void RemoveWeekAskInfoByRoleId(UINT64 roleId);

	void OnDayPass();

	std::vector<MemberAskInfo>& GetAllMemberAskInfo() { return m_askInfos; }
	std::map<UINT64, MemberDonateInfo>& GetAllDonateRecord() { return m_donateRecord; }

	void UpdateDonateInfo(UINT64 roleId, UINT32 todayValue, UINT32 totalValue, bool init = false);
	void UpdateTodayRank(UINT64 roleId, UINT32 value, UINT32 time);
	void UpdateTotalRank(UINT64 roleId, UINT32 value, UINT32 time);

	UINT32 GetTodayDonateNum(UINT64 roleId);

	void FillMemberAskInfo(MemberAskInfo& info, KKSG::GuildMemberAskInfo& data);
	void FillMemberDonateInfo(MemberDonateInfo& info, KKSG::GuildMemberDonateRankItem& data);

	void SetDirty() { m_dirty.Set(); }

private:
	Switch m_dirty;
	Guild* m_guild;

	UINT32 m_updateTime;
	UINT32 m_newId;
	std::vector<MemberAskInfo> m_askInfos;		// 请求物品

	std::map<UINT64, MemberDonateInfo> m_donateRecord;			// 捐献记录
//	RankTree<DonateRankInfo, DonateRankInfoCmp> m_todayRank;	// 今日捐献排名
//	RankTree<DonateRankInfo, DonateRankInfoCmp> m_totalRank;	// 总的捐献排名
};

#endif