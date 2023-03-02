#include "pch.h"
#include "mayhem/ptck2m_mayhemawardntf.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "event/eventmgr.h"
#include "mayhem/mayhemmgr.h"

// generate by ProtoGen at date: 2017/6/14 21:09:08

void PtcK2M_MayhemAwardNtf::Process(UINT32 dwConnID)
{
	std::string strPointMailTitle;
	std::string strPointMailContent;
	std::string strRankMailTitle;
	std::string strRankMailContent;
	std::string strJoinMailTitle;
	std::string strJoinMailContent;

	MailConfig::Instance()->GetMailString(MailConf_MayhemPointAward, strPointMailTitle, strPointMailContent);
	MailConfig::Instance()->GetMailString(MailConf_MayhemRankAward, strRankMailTitle, strRankMailContent);
	MailConfig::Instance()->GetMailString(MailConf_MayhemJoinAward, strJoinMailTitle, strJoinMailContent);


	for (INT32 i = 0; i < m_Data.item_size(); i ++)
	{
		std::vector<ItemDesc> vlist;
		for (INT32 j = 0; j < m_Data.item(i).pointaward_size(); j ++)
		{
			vlist.push_back(ItemDesc(m_Data.item(i).pointaward(j).itemid(), m_Data.item(i).pointaward(j).itemcount()));
		}
		if (vlist.size())
		{
			std::string strPoint = MailConfig::Instance()->ReplaceString("0", ToString(m_Data.item(i).point()), strPointMailContent);
			MailData oData(strPointMailTitle, strPoint, vlist, MAIL_EXPIRE_TIME);
			oData.SetReason(ItemFlow_MayhemPointAward);
			CMailMgr::Instance()->SendMail(m_Data.item(i).roleid(), oData);
		}
		vlist.clear();
		for (INT32 j = 0; j < m_Data.item(i).rankaward_size(); j ++)
		{
			vlist.push_back(ItemDesc(m_Data.item(i).rankaward(j).itemid(), m_Data.item(i).rankaward(j).itemcount()));
		}
		if (vlist.size())
		{
			std::string strRank = MailConfig::Instance()->ReplaceString("0", ToString(m_Data.item(i).rank()), strRankMailContent);
			MailData oData(strRankMailTitle, strRank, vlist, MAIL_EXPIRE_TIME);
			oData.SetReason(ItemFlow_MayhemRankAward);
			CMailMgr::Instance()->SendMail(m_Data.item(i).roleid(), oData);
			EventMgr::Instance()->AddEvent(m_Data.item(i).roleid(),DESIGNATION_COM_TYPE_BIGMELEE,m_Data.item(i).rank());
			MayhemMsMgr::Instance()->AddRoleID(m_Data.item(i).roleid());

			EventMgr::Instance()->AddEventOnlyOnMS(m_Data.item(i).roleid(),BONUS_EVENT_OFFSET +	GUILDBONUS_COM_TYPE_MAYHEMRANK, m_Data.item(i).rank());
		}

		vlist.clear();
		for (INT32 j = 0; j < m_Data.item(i).joinaward_size(); j ++)
		{
			vlist.push_back(ItemDesc(m_Data.item(i).joinaward(j).itemid(), m_Data.item(i).joinaward(j).itemcount()));
		}
		if (vlist.size())
		{
			MailData oData(strJoinMailTitle, strJoinMailContent, vlist, MAIL_EXPIRE_TIME);
			oData.SetReason(ItemFlow_MayhemJoinAward);
			CMailMgr::Instance()->SendMail(m_Data.item(i).roleid(), oData);
		}

	}
}
