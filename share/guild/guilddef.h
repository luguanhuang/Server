#ifndef __GUILDDEF_H__
#define __GUILDDEF_H__

#include "typelist.h"

#define INVALID_GUILDID 0

enum GuildBonsuOperation
{
	GBonus_OP_GetCheckInBonusInfo,
	GBonus_OP_GetBonusList,
	GBonus_OP_GetBonusDetailInfo,
	GBonus_OP_GetBonusReward,
	GBonus_OP_GetBonusSend,
	GBonus_OP_GetBonusAskFor,
	GBonus_OP_GetBonusThank,
	GBonus_OP_GetLastCheckinBonusTime,
	GBonus_OP_GetBonusLeft,
};

enum GuildPosition
{
	GPOS_LEADER,       //会长
	GPOS_VICELEADER,   //副会长
	GPOS_OFFICER,      //官员
	GPOS_ELITEMEMBER,  //精英会员
	GPOS_MEMBER,       //普通会员
	GPOS_MAX,
};

enum GuildPermission
{
	GPEM_DISMISS,       //解散
	GPEM_CHANGELEADER,  //转让
	GPEM_SET_VICELEADER, //任命副会长
	GPEM_SET_OFFICER,    //任命官员
	GPEM_SET_ELITEMEMBER,//任命精英
	GPEM_SETTINGS,       //公会设置
	GPEM_ANNOUNCEMENT,   //公告
	GPEM_APPROVAL,       //审批
	GPEM_FIREMEMBER,     //开除成员
	GPEM_SENDCHECKINBONUS,//发放签到红包
	GPEM_STUDY_SKILL,     // 研究工会技能
	GPEM_ADMINISTRATOR_REWAD,    // 擂台战管理奖励
	GPEM_ADMINISTRATOR_DISTRIBUTION, //擂台战布阵权限
	GPEM_MAX,
};

struct GuildMember
{
	GuildMember()
	{
		qwRoleID = 0;
		contribute = 0;
		position = GPOS_MEMBER;
		updateTime = 0;
		joinTime = 0;
		groupPos = KKSG::GBS_NotMember;
		updateBindTime = 0;
	}

	UINT64 qwRoleID;
	int contribute;
	GuildPosition position;
	UINT32 updateTime; // 贡献更新时间
	UINT32 joinTime;   // 加入公会时间
	KKSG::GuildBindStatus groupPos;			/// 在公会qq群的位置
	UINT32 updateBindTime;					/// 更新公会群信息的时间
	//UINT32 groupFightContribute;			///>同公会成员组队深渊，巢穴，龙本获得的公会贡献值，其他途径获得的贡献值不包含在内
};

namespace KKSG { class GuildAllDBData; }

class IGuildSerializerInterface
{
public:
	virtual ~IGuildSerializerInterface() {}

	virtual bool IsChanged() = 0;
	virtual bool Load(const KKSG::GuildAllDBData & guilddata) = 0;
	virtual void Save(KKSG::GuildAllDBData & guilddata) = 0;
	virtual const std::string ToBinary() = 0;
};

struct GuildBrief;
class GuildHistory;
class GuildApplication;
class CGuildCard;
class GuildFatigue;
class CGuildCheckin;
class CGuildChat;
class GuildBonusMgr;
class GuildGoblin;
class CGuildActivity;
class CGuildWage; 
class GuildGarden;
class CGuildCamp;
class GuildDonate;

typedef TYPELIST_12(GuildBrief, GuildApplication, GuildHistory, CGuildCheckin, GuildBonusMgr, GuildGoblin, CGuildCard, CGuildActivity,
	CGuildWage, GuildGarden, CGuildCamp, GuildDonate) GuildHandlerType;

enum GuildDBOPType
{
	GDBOP_UPDATE = 0,
	GDBOP_CREATE = 1,
	GDBOP_DELETE = 2,
};

enum EGuildCampRank
{
	GUILDCAMP_COSTTIME = 1,
	GUILDCAMP_KILLNUM = 2,
	GUILDCAMP_LIVETIME = 3,
};

#endif // __GUILDDEF_H__