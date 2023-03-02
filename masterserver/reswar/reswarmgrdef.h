#ifndef __RESWARMGRDEF_H__
#define __RESWARMGRDEF_H__

///////////////////////////////////////////////////////////////////////////����buff/////////////////////////////////////////////////////////////////////////////
#include "notice/notice.h"
#include "role/rolemanager.h"

namespace GuildResWar
{
	#define MAX_GUILD_WAR_GROUP 9
	#define GUILD_MAX_TEAM 3
	#define GUILDRESWAR_TIME_SPAN	1
	enum GuildResType
	{
		GuildResPVE =1,
		GuildResPVP =2,
	};

	enum GuildCountDownType
	{
		eResWarReady    = 1,		//׼������ʱ
		eResWarEnd		= 2,		//��������ʱ
	};

	enum GuildResFlow
	{
		eGuildResFlowIdle				= 0,		//����״̬
		eGuildResFlowGroup      		= 1,		//����
		eGuildResFlowExplore			= 2,		//̽��
		eGuildResFlowAward				= 3,		//�ɽ�
		eGuildResFlowEnd				= 4,		//��������
	};
	enum GuildResWarState
	{
		eGuildResStateIdle			  = 0,			//
		eGuildResStateExploreIng	  = 1,			//̽��cd��
		eGuildResStateExploreEnd	  = 2,			//̽�������
		eGuildResStateMatch			  = 3,
		eGuildResStateFight			  = 4,			//ս����
		eGuildResStateResult		  = 5,			//ս�������
		eGuildResStateCancel		  = 6,			//ȡ��
	};

	enum ResWarRefresh
	{
		eResWarRank                   = 1,
		eResWarCD					  = 2,
		eResWarUseItem				  = 3,
		eResWarCardCD				  = 4,
		eResWarBuffItem				  = 5,
		eResWarMineID				  = 6,
	};

	enum ResWarMsgType
	{
		eResWarMsgTypeIcon           = 1,
		eResWarMsgTypeNotice         = 2,
		eResWarMsgTypeTime			 = 3,
		eResWarMsgTypeUseCard		 = 4,
		eResWarMsgTypeUseCardEnd	 = 5,
		eResWarOpen5Min				 = 6,
		eResWarFinal5Min			 = 7,
		eResWarAwardTypeIcon         = 8,
	};

	struct GuildFightTeam;

	struct GuildFightRole
	{
		UINT64 nRole;
		UINT32 nJob;
		UINT32 nLevel;
		UINT32 nFight;
		std::string szName;
		bool bLost;
		UINT32 nRes;
		bool bIcon;
		GuildFightTeam *pGuildFightTeam;
		GuildFightRole():nRole(0),nJob(0),nLevel(0),nFight(0),szName(""),bLost(false),nRes(0),bIcon(true),pGuildFightTeam(NULL){}
	};


	struct GuildTeam;
	struct GuildFightTeam
	{
		std::map<UINT64, GuildFightRole*> mapFightRole;
		UINT32 nKiller;	//��ɱ����
		UINT64 nHurt;	//��ɱ�˺�
		UINT32 nWin;	//ʤ������
		UINT64 nFight;	//����ս����
		UINT32 nState;	//״̬
		UINT32 nTime;	//time ̽������ʱ��
		UINT32 nUniID;	//����ID
		UINT64 nGuildID;//����ID
		UINT32 nTeamID;	//����ID
		std::vector<UINT32> vecMineral;
		std::vector<UINT32> vecBuff;
		GuildTeam *pGuildTeam;
		GuildFightTeam():nKiller(0),nHurt(0),nWin(0),nFight(0),nState(0),nTime(0),nUniID(0),nGuildID(0),nTeamID(0),pGuildTeam(NULL){}
	};

	struct GroupGuildFightTeam
	{
		UINT32 nUniID;
		GuildFightTeam *pSelf;
		GuildFightTeam *pTarget;
		UINT32 nFightTime;
		GroupGuildFightTeam():nUniID(0),pSelf(NULL),pTarget(NULL),nFightTime(0){}
	};

	struct SimpleGuildInfo
	{
		std::string szName;
		UINT32 nPrestige;
		UINT32 nIcon;
		SimpleGuildInfo():szName(""),nPrestige(0),nIcon(0){}
	};



	struct  GroupGuildInfo;
	struct  GuildTeam
	{
		UINT64 nGuildID;							//����ID
		std::string szName;							//��������
		UINT32 nIcon;								// ͼ��
		UINT32 nResValue;							//��Դֵ
		UINT32 nCD;									//̽��ʱ��
		UINT32 nNextUseBuffTime;					//��һ��ʹ��Buffʱ��
		UINT32 nFight;								
		UINT32 nTime;								
		UINT32 nState;								//Ĭ������������debuff���ǣ�
		UINT32 nAddition;							//�ӳ� Ĭ����0
		UINT32 nPrestige;
		GroupGuildInfo *pGroupGuildInfo;
		std::map<UINT32, UINT32> mapBuff;			//����ֿ�
		std::map<UINT32, GuildFightTeam *> mapTeam;
		std::vector<Notice*> vecNotice;
		GuildTeam():nGuildID(0),szName(""),nIcon(0),nResValue(0),nCD(0),nNextUseBuffTime(0),nFight(0),nTime(0),nState(0),nAddition(0),nPrestige(0),pGroupGuildInfo(NULL){}
	};

	struct GroupGuildInfo
	{
		UINT32 nGroupID;
		std::map<UINT64, GuildTeam*> mapGroupGuild;
		GroupGuildInfo():nGroupID(0){}
	};

	enum GuildBuffState
	{
		eStealGuildRes=1,   //1͵ȡĿ�깫����Դ����5%~10%
		eAddEnemyTime =2,   //2Ŀ�깫��̽��ʱ���ӳ�X��ʱ��
		eSubSelfTime  =3,   //3��������̽��ʱ������X��ʱ��
		eAddSelfRes   =4,   //4���������Ա̽����õ���Դ������X��ʱ��
		eEnemyUnKnown =5,   //5�з��������г�Ա̽�������Ϊ������ʱ��
		eClearDeBuff  =6,   //6������м�����ʽ��Ч��
	};

	enum GuildBuffCamp
	{
		eGuildBuffCampSelf  = 1,
		eGuildBuffCampEnemy = 2,
	};

	struct GuildBuffInfo
	{
		UINT64 nGuildID;
		UINT32 nID;
		time_t nTime;
		bool bDel;								//�Ƿ�ɾ��
		GuildBuffInfo():nGuildID(0),nID(0),nTime(0),bDel(false){}
	};

};

#endif
