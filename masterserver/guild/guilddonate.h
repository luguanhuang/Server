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

	UINT32 id;				// Ψһid
	UINT64 roleId;			// ������id
	UINT32 publishTime;		// ����ʱ��
	UINT32 itemId;			// ��Ʒid
	UINT32 needCount;		// �������
	UINT32 getCount;		// �Ѿ���ø���
	UINT32 level;			// �ȼ�
	UINT32 quality;			// Ʒ��
	UINT32 taskId;			// ��Ӧ����id
	KKSG::PeriodTaskType tasktype;		// ��������������(�ճ�����������)
	UINT32 itemType;
	UINT32 itemQuality;
	UINT32 index;

	bool donating;			// ���ھ��ף�����������ͬʱ���ף��첽ԭ���¾����

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
	std::vector<MemberAskInfo> m_askInfos;		// ������Ʒ

	std::map<UINT64, MemberDonateInfo> m_donateRecord;			// ���׼�¼
//	RankTree<DonateRankInfo, DonateRankInfoCmp> m_todayRank;	// ���վ�������
//	RankTree<DonateRankInfo, DonateRankInfoCmp> m_totalRank;	// �ܵľ�������
};

#endif