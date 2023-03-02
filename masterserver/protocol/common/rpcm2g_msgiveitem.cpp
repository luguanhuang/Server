#include "pch.h"
#include "common/rpcm2g_msgiveitem.h"
#include "arenastar/arenastarmgr.h"
#include "define/tlogenum.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"

// generate by ProtoGen at date: 2017/6/24 11:30:41

RPC_CLIENT_IMPLEMETION(RpcM2G_MsGiveItem, MsGiveItemArg, MsGiveItemRes)

void RpcM2G_MsGiveItem::OnReply(const MsGiveItemArg &roArg, const MsGiveItemRes &roRes, const CUserData &roUserData)
{
	if (roRes.errorcode() != KKSG::ERR_SUCCESS)
	{
		switch(roArg.subreason())
		{
		case ItemFlow_ArenaStar_DianZan:
			{
				ArenaStarMgr::Instance()->GiveDianZanRewardMail(roArg);
				break;
			}
		default:
			{
				const MailConf* mailConf = MailConfig::Instance()->GetMailConf(MailConf_MSGiveItemFailed);
				if (mailConf != NULL)
				{
					LogInfo("role [%llu], ms give item failed and send mail", roArg.roleid());
					std::vector<ItemDesc> item;
					for (int i = 0; i < roArg.items_size(); ++i)
					{
						ItemDesc desc;
						desc.ToThis(roArg.items(i));
						item.push_back(desc);
					}
					MailData mail(mailConf->m_title, mailConf->m_content, item, MAIL_EXPIRE_TIME);
					CMailMgr::Instance()->SendMail(roArg.roleid(), mail);
				}
				break;
			}
		}
	}
}

void RpcM2G_MsGiveItem::OnTimeout(const MsGiveItemArg &roArg, const CUserData &roUserData)
{
}
