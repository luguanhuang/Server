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
	GPOS_LEADER,       //�᳤
	GPOS_VICELEADER,   //���᳤
	GPOS_OFFICER,      //��Ա
	GPOS_ELITEMEMBER,  //��Ӣ��Ա
	GPOS_MEMBER,       //��ͨ��Ա
	GPOS_MAX,
};

enum GuildPermission
{
	GPEM_DISMISS,       //��ɢ
	GPEM_CHANGELEADER,  //ת��
	GPEM_SET_VICELEADER, //�������᳤
	GPEM_SET_OFFICER,    //������Ա
	GPEM_SET_ELITEMEMBER,//������Ӣ
	GPEM_SETTINGS,       //��������
	GPEM_ANNOUNCEMENT,   //����
	GPEM_APPROVAL,       //����
	GPEM_FIREMEMBER,     //������Ա
	GPEM_SENDCHECKINBONUS,//����ǩ�����
	GPEM_STUDY_SKILL,     // �о����Ἴ��
	GPEM_ADMINISTRATOR_REWAD,    // ��̨ս������
	GPEM_ADMINISTRATOR_DISTRIBUTION, //��̨ս����Ȩ��
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
	UINT32 updateTime; // ���׸���ʱ��
	UINT32 joinTime;   // ���빫��ʱ��
	KKSG::GuildBindStatus groupPos;			/// �ڹ���qqȺ��λ��
	UINT32 updateBindTime;					/// ���¹���Ⱥ��Ϣ��ʱ��
	//UINT32 groupFightContribute;			///>ͬ�����Ա�����Ԩ����Ѩ��������õĹ��ṱ��ֵ������;����õĹ���ֵ����������
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