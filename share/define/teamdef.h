#ifndef __TEAMDEF_H__
#define __TEAMDEF_H__

enum TeamType 
{
	EXP_TEAM_SCENE			= 1,	// Զ����
	GUILD_TEAM_SCENE		= 2,	// ������ӱ�
	NEST_TEAM_SCENE			= 3,	// ��Ѩ
	ABYSS_TEAM_SCENE		= 4,	// ��Ԩ����
	SMALL_GOBLIN_TEAM_SCENE = 5,	// С����
	DRAGON_TEAM_SCENE		= 6,	// ��
	TEAM_TOWER				= 7,	// ����
	PVP_TEAM_SCENE			= 8,	// PVP
	GODDESS_TEAM_SCENE		= 9,	// Ů������
	ENDLESSABYSS_SCENE		= 10,	// ˫����Ԩ
	SKYCITY_SCENE			= 11,   // ���֮��
	RESWAR_SCENE			= 12,	//
	GUILDCAMP_TEAM_SCENE	= 13,	// ����Ӫ��
	WEEK_NEST_TEAM_SCENE	= 14,   // ÿ�ܳ�Ѩ 
	PARTNER_TEAM_SCENE      = 15,   // ���
	HEROBATTLE_TEAM_SCENE	= 16,	// Ӣ��ս��
	LEAGUETEAM_SCENE		= 17,	// ս������(��ӽ�����ϵ�����ս������)
	FESTIVAL_TEAM_SCENE     = 18,   // ����
	TASK_TEAM_SCENE         = 19,   // ��������
	SKYCRAFT_SCENE			= 20,	// ���������
	TEAM_TYPE_PKTWO         = 21,   // ����
    TEAM_WEEKEND4V4_SCENE   = 22,   // ��ĩ�
	MOBABATTLE_TEAM_SCENE	= 23,	// Ӣ��Ͽ��
	TEAM_TYPE_CUSTOMPKTWO	= 24,	// ȫ��PK2v2
	TEAM_TYPE_BIOHELL		= 25,	// ��������Ȯ;
	TEAM_TYPE_COUPLE		= 26,	// ���޸���;
	TEAM_TYPE_MARRIAGE		= 27,	// ������;
	TEAM_TYPE_COMPETE_DRAGON = 28,	// ��������

	TEAM_TYPE_MAX 
};

enum TeamCostCountType
{
	TEAM_CCT_NONE = 0,//���۴���
	TEAM_CCT_ENTER = 1,//����۴���
	TEAM_CCT_WIN = 2,//ʤ���۴���
};

enum TeamOpType
{
	TEAM_OPT_NORMAL = 0,
	TEAM_OPT_FORCE = 1,
	TEAM_OPT_MATCH = 2,
	TEAM_OPT_INVITE = 3,
};

enum TeamTransType
{
	TEAM_TRANS_OP_REQ = 0,
	TEAM_TRANS_FETCH_TEAM = 1,
	TEAM_TRANS_TEAM_CHANGEDATA = 2,
	TEAM_TRANS_ROLE2TEAM = 3,
	TEAM_TRANS_ROLELEAVE = 4,
	TEAM_TRANS_ROLELOGOUT = 5,
	TEAM_TRANS_ERRERCODE = 6,
	TEAM_TRANS_INVITEDATA = 7,
	TEAM_TRANS_TEAMFULLDATA = 8,
	TEAM_TRANS_TEAMSYN2GS = 9,
	TEAM_TRANS_GS_REGISTER = 11,
	TEAM_TRANS_INV_REFUSE = 12,
	TEAM_TRANS_TEAM_MATCH = 13,
	TEAM_TRANS_SYN_EXTRAINFO = 14,
	TEAM_TRANS_CHAT = 15,
	TEAM_TRANS_START_BATTLE = 16,
	TEAM_TRANS_COST_CANCEL = 17,
	TEAM_TRANS_BATTLE_RESULT = 18,
	TEAM_TRANS_ROLE_MATCH = 19,
	TEAM_TRANS_RM_CONFIRM_NOTICE = 20,
	TEAM_TRANS_RM_REFUSE_NOTICE = 21,
	TEAM_TRANS_START_BATTLE_FAILED = 22,
	TEAM_TRANS_CHECK_EXP_CHANGE = 23,
};

enum TGotoNextSceneType
{
	TGOTONST_FIRST = 0,
	TGOTONST_NEXT = 1,
	TGOTONST_RETURN = 2,
};

enum THSpecialType
{
	THSpeType_None = 0,
	THSpeType_Ac2Join = 1,
};

const UINT32 TeamCostInvalidID = (UINT32)(-1);
const int TeamListSendCountLimit = 100;
const int TeamForCheckCountLimit = 50000;

#define TJUDGERET(ret) if (0 != ret) {return ret;}

#endif