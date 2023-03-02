#ifndef _H_IDIP_MSCOMMONDEF_HANDLER_H__
#define _H_IDIP_MSCOMMONDEF_HANDLER_H__
#include "idip/idiphandler.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rpcm2d_readroledata.h"
#include "network/dblink.h"
#include "pb/project.pb.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "network/dblink.h"
#include "role/rolesummarymgr.h"
#include "mail/mailmgr.h"
#include "define/itemdef.h"
#include "idip/idiperrdesc.h"
#include "loghelper/tlogr.h"

//ֻ���޸�roleallinfo��Ϣ��Э��ſ������������;
IDIP_HANDLER_MODIFY_DECLARE(UpdateGold, IDIP_DO_UPDATE_GOLD_REQ)
IDIP_HANDLER_MODIFY_DECLARE(UpdateFatigue, IDIP_DO_UPDATE_PHYSICAL_REQ)
IDIP_HANDLER_MODIFY_DECLARE(UpdateDragonCoin, IDIP_DO_UPDATE_DRAGON_GOLD_REQ)
IDIP_HANDLER_MODIFY_DECLARE(UpdateDiamond, IDIP_DO_UPDATE_DIAMOND_REQ)
IDIP_HANDLER_MODIFY_DECLARE(UpdateArenaCoin, IDIP_DO_UPDATE_HONOUR_REQ)
IDIP_HANDLER_MODIFY_DECLARE(DelItem, IDIP_DO_DEL_ITEM_REQ)
IDIP_HANDLER_MODIFY_DECLARE(UpdateScore, IDIP_AQ_DO_CLEAR_SCORE_REQ)
IDIP_HANDLER_MODIFY_DECLARE(SendMsg, IDIP_AQ_DO_SEND_MSG_REQ)
IDIP_HANDLER_MODIFY_DECLARE(ZeroProfit, IDIP_AQ_DO_ZEROPROFIT_REQ)
IDIP_HANDLER_MODIFY_DECLARE(BanPlay, IDIP_AQ_DO_BAN_PLAY_REQ)
IDIP_HANDLER_MODIFY_DECLARE(NameAct, IDIP_DO_NAME_ACT_REQ)//�ƺż�������
IDIP_HANDLER_MODIFY_DECLARE(DoXinyueRed, IDIP_DO_XINYUE_RED_REQ)
IDIP_HANDLER_MODIFY_DECLARE(DoIdipRed, IDIP_DO_XIAOYUE_RED_REQ)
IDIP_HANDLER_MODIFY_DECLARE(DiamondConsumeToGame, IDIP_DO_COIN_CONSUME_TO_GAME_REQ)

//��������server;
IDIP_HANDLER_MODIFY_DECLARE(WhiteList, IDIP_DO_ADD_DEL_WHITE_LIST_REQ)


//ֻ�Ƕ�ȡ������ز��漰�޸����ݵ���ϢЭ��;
IDIP_HANDLER_READ_DECLARE(QueryBag, IDIP_QUERY_BAG_REQ)
IDIP_HANDLER_READ_DECLARE(QueryEmblem, IDIP_QUERY_BADGE_REQ)
IDIP_HANDLER_READ_DECLARE(QueryItemInfo, IDIP_QUERY_EQUIPMENT_REQ)
IDIP_HANDLER_READ_DECLARE(QueryReverseRole, IDIP_QUERY_REVERSE_USER_INFO_REQ)
IDIP_HANDLER_READ_DECLARE(AqQueryOpenidInfo, IDIP_AQ_QUERY_OPENID_INFO_REQ)
IDIP_HANDLER_READ_DECLARE(QueryUserInfo, IDIP_QUERY_USER_INFO_REQ)
IDIP_HANDLER_READ_DECLARE(QuerySpriteInfo, IDIP_QUERY_FAIRY_INFO_REQ)
IDIP_HANDLER_READ_DECLARE(QueryHorseInfo, IDIP_QUERY_HORSE_INFO_REQ)
IDIP_HANDLER_READ_DECLARE(QueryWeekUserInfo, IDIP_QUERY_USER_INFO_WEEK_REQ)
IDIP_HANDLER_READ_DECLARE(QueryFashion, IDIP_QUERY_FASHION_REQ)
IDIP_HANDLER_READ_DECLARE(QueryFashionCompose, IDIP_QUERY_ROLE_CLOTHES_REQ)

//ֱ����ms����Ĺ���;
IDIP_HANDLER_COMMON_DECLARE(SendItem, IDIP_DO_SEND_ITEM_REQ);//���͵���;
IDIP_HANDLER_COMMON_DECLARE(UnPunish, IDIP_DO_UNBAN_USR_REQ);//�����š�����;
IDIP_HANDLER_COMMON_DECLARE(QueryLampNotice, IDIP_QUERY_RUNNING_ROLLING_MSG_REQ) //��ѯ����ƹ���;
IDIP_HANDLER_COMMON_DECLARE(DelLampNotice, IDIP_DO_STOP_ROLLING_MSG_REQ)//ֹͣ����ƹ���;
IDIP_HANDLER_COMMON_DECLARE(SendLampNotice, IDIP_DO_SEND_ROLLING_MSG_REQ) //��������ƹ���;
IDIP_HANDLER_COMMON_DECLARE(QueryGuildInfo, IDIP_QUERY_PARTY_INFO_REQ) //��ѯ����;
IDIP_HANDLER_COMMON_DECLARE(QueryGuildNotice, IDIP_AQ_QUERY_PARTY_NOTICE_REQ);
IDIP_HANDLER_COMMON_DECLARE(SetGuildNotice, IDIP_AQ_DO_SET_NOTICE_REQ);
IDIP_HANDLER_COMMON_DECLARE(SendNotice, IDIP_AQ_DO_SEND_NOTICE_REQ);//���͹���
IDIP_HANDLER_COMMON_DECLARE(BanOfflineRank, IDIP_AQ_DO_BAN_RANKING_OFFLINE_REQ);//��ֹ�������а�
IDIP_HANDLER_COMMON_DECLARE(QueryBossBattle, IDIP_AQ_QUERY_BOSS_BATTLE_INFO_REQ);//��ѯboss�˺�;
IDIP_HANDLER_COMMON_DECLARE(QueryBanInfo, IDIP_QUERY_BAN_INFO_REQ);//��ѯȫ�����״̬����
IDIP_HANDLER_COMMON_DECLARE(ClearChat, IDIP_AQ_DO_CLEAR_SPEAK_REQ);//�������
IDIP_HANDLER_COMMON_DECLARE(QueryGuildInfoX, IDIP_QUERY_GUILD_INFO_REQ) //��ѯ������Ϣ����
IDIP_HANDLER_COMMON_DECLARE(QueryGuildMemberInfo, IDIP_QUERY_GUILD_MEMBER_INFO_REQ) //��ѯ�����Ա��Ϣ����
IDIP_HANDLER_COMMON_DECLARE(QueryUserPunishInfo, IDIP_AQ_QUERY_USER_INFO_REQ)
IDIP_HANDLER_COMMON_DECLARE(UnMask, IDIP_DO_UNMASK_CHAT_REQ) //�������
IDIP_HANDLER_COMMON_DECLARE(SendMail, IDIP_DO_SEND_MAIL_REQ) //ȫ���ʼ�����
IDIP_HANDLER_COMMON_DECLARE(QueryGuildTask, IDIP_QUERY_GUILD_TASK_REQ) //��������
IDIP_HANDLER_COMMON_DECLARE(SendPersonMail, IDIP_DO_SEND_PERSON_MAIL_REQ) //���͸����ʼ�
IDIP_HANDLER_COMMON_DECLARE(QueryGuildInfoBack, IDIP_QUERY_GUILD_INFO_BACK_REQ) //�������ѯ;
IDIP_HANDLER_COMMON_DECLARE(UpdateGuildName, IDIP_DO_UPDATE_GUILD_NAME_REQ) //�޸Ĺ�����
IDIP_HANDLER_COMMON_DECLARE(XinYueRPay, IDIP_DO_XINYUE_R_RECHARGE_REQ) //���ô�r��ֵ;
IDIP_HANDLER_COMMON_DECLARE(UpdateGuildInfo, IDIP_DO_UPDATE_PRESTIGE_REQ) //�޸Ĺ������������ᾭ��
IDIP_HANDLER_COMMON_DECLARE(QueryMail, IDIP_QUERY_MAIL_STATUS_REQ) //��ǰ����ʼ�״̬��ѯ����
IDIP_HANDLER_COMMON_DECLARE(QueryItemIdToName, IDIP_QUERY_ITEM_ID_TO_NAME_REQ) //����ID���������������
IDIP_HANDLER_COMMON_DECLARE(QueryFightPunish, IDIP_QUERY_FIGHT_PUNISH_REQ) //��ѯӢ��ս�������������
IDIP_HANDLER_COMMON_DECLARE(ClearFightPunish, IDIP_DO_CLEAR_FIGHT_PUNISH_REQ) //���Ӣ��ս����������

IDIP_HANDLER_COMMON_DECLARE(ControlFunction, IDIP_DO_CONTROL_FUNCTION_REQ)//���ܿ�������
IDIP_HANDLER_COMMON_DECLARE(DoActiveAchievement, IDIP_DO_ACTIVE_ACHIEVEMENT_REQ)//�ɾͼ�������
IDIP_HANDLER_COMMON_DECLARE(Query1v1RankReq, IDIP_QUERY_1V1_RANK_REQ)//��ѯ����1v1��������


static INT32 GetVirtualItem(const KKSG::RoleAllInfo& roInfo, UINT32 type)
{
	return (roInfo.has_bag() && (UINT32)roInfo.bag().virtualitems_size() > type) ? (INT32)roInfo.bag().virtualitems(type) : 0;
}

//��ʼ����ɫ
class CMSIdipInitialAccountHandler: public CIdipHandler
{
public:
	CMSIdipInitialAccountHandler() :CIdipHandler(0, IDIP_AQ_DO_INITIAL_ACCOUNT_REQ){}
	virtual ~CMSIdipInitialAccountHandler() {}

	virtual CIdipHandler* New() { return new CMSIdipInitialAccountHandler(); }
	virtual bool ReadDataQequest();
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool ModifyMsData(CUserData& roUserData);
	virtual bool SaveDataToDB(const CUserData& roUserData);
	virtual bool OnAsyncReply(const CUserData& roUserData);
	virtual bool KickOutReply();
	void IdipLog();

private:
	std::string szOpenId;
};

//��Ž���
class CMSIdipPunishHandler: public CIdipHandler
{
public:
	CMSIdipPunishHandler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd) {}
	CMSIdipPunishHandler() :CIdipHandler(0, IDIP_DO_BAN_USR_REQ) {} 
	virtual ~CMSIdipPunishHandler() {}
	virtual CIdipHandler* New() {return new CMSIdipPunishHandler();}
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool KickOutReply();
	void DoPunish(CIdipReply& roReply);
	void IdipLog();

private:
	KKSG::PunishData m_oData;
};

//AQ�������
class CMSIdipRelievePunishHandler: public CIdipHandler
{
public:
	CMSIdipRelievePunishHandler() :CIdipHandler(0, IDIP_AQ_DO_RELIEVE_PUNISH_REQ),
		m_bIsRelieveZeroProfit(false),
		m_bIsRelievePlayAll(false),
		m_bIsRelieveBanJoinRank(false),
		m_bIsRelieveBan(false),
		m_bIsRelieveMaskchat(false){}
	virtual ~CMSIdipRelievePunishHandler() {}

	virtual CIdipHandler* New() { return new CMSIdipRelievePunishHandler(); }
	virtual bool ReadDataQequest();
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool ModifyMsData(CUserData& roUserData);
	virtual bool SaveDataToDB(const CUserData& roUserData);
	virtual bool OnAsyncReply(const CUserData& roUserData);
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);

public:
	void UnPunishMs();
	void IdipLog();
private:
	bool m_bIsRelieveZeroProfit;
	bool m_bIsRelievePlayAll;
	bool m_bIsRelieveBanJoinRank;
	bool m_bIsRelieveBan;
	bool m_bIsRelieveMaskchat;
	std::string strOpenId;
};

//��ѯ����
class CMSIdipQueryNoticeHandler: public CIdipHandler 
{ 
public: 
	CMSIdipQueryNoticeHandler(UINT32 dwID, INT32 nCmd) 
		:CIdipHandler(dwID, nCmd)
	{
		m_AreaID = 0;
		m_PlatID = 0;
		m_PageNo = 0;
		m_Start = 0;
	} 
	CMSIdipQueryNoticeHandler() :CIdipHandler(0, IDIP_QUERY_RUNNING_NOTICE_REQ) {} 
	virtual ~CMSIdipQueryNoticeHandler() {} 
	virtual CIdipHandler* New() {return new CMSIdipQueryNoticeHandler();}
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool OnAsyncReply(const CUserData& roUserData);

private:
	UINT32 m_AreaID;
	UINT32 m_PlatID;
	UINT32 m_PageNo;
	UINT32 m_Start;
};

class CMSIdipDoDelItemXHandler: public CIdipHandler
{
public:
	CMSIdipDoDelItemXHandler() :CIdipHandler(0, IDIP_DO_DEL_ITEM_OUTBAG_REQ)
	{
		m_Type = 0;
		m_ItemID = 0;
		m_ItemUid = 0;
		m_RoleId = 0;
		m_JadeId = 0;
	}
	virtual ~CMSIdipDoDelItemXHandler() {}
	virtual CIdipHandler* New() {return new CMSIdipDoDelItemXHandler();}
	virtual bool ReadDataQequest();
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool ModifyMsData(CUserData& roUserData);
	virtual bool SaveDataToDB(const CUserData& roUserData);
	virtual bool OnAsyncReply(const CUserData& roUserData);
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);
	virtual bool KickOutReply();
	virtual bool DoOther(CIdipReply& roReply);
	virtual bool OnAsyncReplyFromOther(const CUserData& roUserData);
public:
	void IdipLog();
private:
	INT32 m_Type;
	UINT32 m_ItemID;
	UINT64 m_ItemUid;
	UINT64 m_RoleId;
	INT32 m_JadeId;
	std::string m_strOpenId;
};

class CMSIdipUpdateNameHandler: public CIdipHandler
{
public:
	CMSIdipUpdateNameHandler() :CIdipHandler(0, IDIP_DO_UPDATE_NAME_REQ){}
	virtual ~CMSIdipUpdateNameHandler() {}

	virtual CIdipHandler* New() { return new CMSIdipUpdateNameHandler(); }
	virtual bool ReadDataQequest();
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool ModifyMsData(CUserData& roUserData);
	virtual bool SaveDataToDB(const CUserData& roUserData);
	virtual bool OnAsyncReply(const CUserData& roUserData);
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);

private:
	void DoAfterChangeName();//�����ɹ���Ĳ���;
public:
	std::string szOpenId;
	std::string szMailTitle;
	std::string szMailContent;
	std::string szName;
};

class CMSIdipItemChangeHandler: public CIdipHandler
{
public:
	CMSIdipItemChangeHandler() :CIdipHandler(0, IDIP_DO_CHANGE_REQ)
	{
		m_SItemListCount = 0;
		m_DItemListCount = 0;
	}
	virtual ~CMSIdipItemChangeHandler() {}
	virtual CIdipHandler* New() {return new CMSIdipItemChangeHandler();}
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);
public:
	void IdipLog();
	void DoLog(std::vector<ItemDesc>& list, INT32 op);
	bool CheckItemChange(UINT32 Count, std::vector<ItemDesc>& list);

private:
	UINT32 m_SItemListCount;
	UINT32 m_DItemListCount;
	std::vector<ItemDesc> m_vSItemList;
	std::vector<ItemDesc> m_vDItemList;
	std::string m_strOpenId;
};

class CMSIdipQueryRoleInfoXHandler: public CIdipHandler
{
public:
	CMSIdipQueryRoleInfoXHandler(UINT32 dwID, INT32 nCmd) :CIdipHandler(dwID, nCmd) {}
	CMSIdipQueryRoleInfoXHandler() :CIdipHandler(0, IDIP_QUERY_ROLE_INFO_REQ) {}
	virtual ~CMSIdipQueryRoleInfoXHandler() {}
	virtual CIdipHandler* New() {return new CMSIdipQueryRoleInfoXHandler();}
	virtual bool ReadDataQequest();
	virtual bool OnAsyncReplyFromGs(const CUserData& roUserData);
	virtual INT32 HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply);
	virtual bool OnAsyncReply(const CUserData& roUserData);

	void BuildMarriageData(UINT64 roleid, std::string& strJsonData);
public:
	std::string szOpenId;
};
#endif