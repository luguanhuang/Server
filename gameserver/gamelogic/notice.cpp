#include "pch.h"
#include "notice.h"
#include "noticemgr.h"
#include "pb/project.pb.h"
#include "gamelogic/rolesummary.h"
#include "unit/role.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "unit/rolemanager.h"
#include "gatelink.h"
#include "mslink.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/teammember.h"
#include "guild/guildchat.h"
#include "chat/ptcg2m_noticeforwardntf.h"
#include "idutil.h"
#include "servermgr/servermgr.h"
#include "team/teamcommon.h"
#include "notice/ptcg2c_wordnotify.h"

Notice::Notice(UINT32 id)
{
	noticeid = id;
	UINT32 noticechannel = NoticeMgr::Instance()->GetChannel(id);
	UINT32 priority = NoticeMgr::Instance()->GetPriority(id);
	UINT32 level = NoticeMgr::Instance()->GetLevel(id);

	offset = 0;
	cansend = IsValidChannel(noticechannel);	
	paramtypes = NoticeMgr::Instance()->GetParamTypes(id);

	chatinfo = new KKSG::ChatInfo;
	const std::string* info = NoticeMgr::Instance()->GetInfo(id);
	if (NULL != info)
	{
		chatinfo->set_info(*info);
		chatinfo->set_channel(noticechannel);
		chatinfo->set_priority(priority);
		if (0 != level)
		{
			chatinfo->set_level(level);
		}
		chatinfo->set_isbroadcast(true);
	}
	else
	{
		cansend = false;
	}
}

Notice::Notice(const std::string& info, UINT32 channel, UINT32 priority)
{
	noticeid = 0;
	offset = 0;
	paramtypes = NULL;

	cansend = IsValidChannel(channel);

	chatinfo = new KKSG::ChatInfo;
	chatinfo->set_info(info);
	chatinfo->set_channel(channel);
	chatinfo->set_priority(priority);
	chatinfo->set_isbroadcast(true);
}

Notice::~Notice()
{
	if (NULL != chatinfo)
	{
		delete chatinfo;
		chatinfo = NULL;
	}
}

bool Notice::IsValidChannel(UINT32 channel)
{
	return channel == KKSG::SystemChannel || channel == KKSG::LampShortChannel || \
			channel == KKSG::LampLongChannel || channel == KKSG::TeamChannel || channel == KKSG::GuildChannel;
			
}

void Notice::Replace(const std::string& src, const std::string& dest)
{
	std::string info = chatinfo->info();
	int start = info.find(src);
	if(std::string::npos == (size_t)start)
	{
		return;
	}
	int end = src.size();
	info.replace(start, end, dest);
	++offset;
	chatinfo->set_info(info);
}

NoticeParamType Notice::GetParamType()
{
	if (NULL == paramtypes)
	{
		return ParamNoticeDummy;
	}
	if (offset >= paramtypes->size())
	{
		return ParamNoticeDummy;
	}
	return (*paramtypes)[offset];
}

//void Notice::AddSrc(RoleSummary* role)
//{
//	if (NULL != role)
//	{
//		KKSG::ChatSource* src = chatinfo->mutable_source();
//		src->set_roleid(role->qwRoleID);
//		src->set_name(role->strName);
//		src->set_powerpoint(role->dwPowerPoint);
//		src->set_profession(role->dwProfession);
//		src->set_viplevel(role->viplevel);
//	}
//}

bool Notice::AddLink()
{
	if (!cansend)
	{
		return false;
	}
	if (ParamLink!= GetParamType())
	{
		cansend = false;
	}
	else
	{
		NoticeLinkInfo* info = NoticeMgr::Instance()->GetLinkInfo(noticeid);
		if (NULL != info)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamLink* paramlink = param->mutable_link();
			paramlink->set_content(info->content);
			paramlink->set_id(info->param);
			
		}
	}
	++offset;
	return cansend;
}

bool Notice::Add(Role* role)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamRole != GetParamType())
	{
		cansend = false;
	}
	else
	{
		if (NULL != role)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamRole* paramrole = param->mutable_role();
			paramrole->set_uniqueid(role->GetID());
			paramrole->set_name(role->GetName());
			paramrole->set_profession(role->GetProfession());
		}
	}
	++offset;
	return cansend;
}

bool Notice::Add(RoleSummary* role)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamRole != GetParamType())
	{
		cansend = false;
	}
	else
	{
		if (NULL != role)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamRole* paramrole = param->mutable_role();
			paramrole->set_uniqueid(role->GetID());
			paramrole->set_name(role->GetName());
		}
	}
	++offset;
	return cansend;
}

bool Notice::Add(XItem* item)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamItem != GetParamType())
	{
		cansend = false;
	}
	else
	{
		if (NULL != item)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamItem* paramitem = param->mutable_item();
			paramitem->mutable_item()->set_itemid(item->itemID);
			paramitem->mutable_item()->set_itemcount(item->itemCount);
		}
	}
	++offset;
	return cansend;
}

bool Notice::Add(const ItemDesc& desc)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamItem != GetParamType())
	{
		cansend = false;
	}
	else
	{
		KKSG::ChatParam* param = chatinfo->add_param();
		KKSG::ChatParamItem* paramitem = param->mutable_item();
		paramitem->mutable_item()->set_itemid(desc.itemID);
		paramitem->mutable_item()->set_itemcount(desc.itemCount);
	}
	++offset;
	return cansend;
}

bool Notice::AddGuild(UINT64 guildid)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamGuild != GetParamType())
	{
		cansend = false;
	}
	else
	{
		KKSG::ChatParam* param = chatinfo->add_param();
		KKSG::ChatParamGuild* paramguild = param->mutable_guild();
		paramguild->set_guildid(guildid);
		CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(guildid);
		if (pGuild)
		{
			paramguild->set_guildname(pGuild->GetGuildName());
		}
		else
		{
			LogError("guild :%llu not exist.", guildid);
		}
	}
	++offset;
	return cansend;
}

bool Notice::Add(UINT32 num)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamNum != GetParamType())
	{
		cansend = false;
	}
	else
	{
		KKSG::ChatParam* param = chatinfo->add_param();
		KKSG::ChatParamNum* paramnum = param->mutable_num();
		paramnum->set_num(num);
	}
	++offset;
	return cansend;
}

bool Notice::CanSend()
{
	if (!cansend)
	{
		return false;
	}	
	if (NULL != paramtypes && offset != paramtypes->size())
	{
		return false;
	}
	if (NULL == chatinfo)
	{
		return false;
	}
	return true;
}

//¼æÈÝCross_GS
bool Notice::Send(UINT32 serverID/* = 0*/)
{
	if (!CanSend())	
	{
		return false;
	}

	PtcG2M_NoticeForwardNtf ntf;
	ntf.m_Data.set_type(KKSG::Notice_World);
	ntf.m_Data.set_uid(0);
	KKSG::ChatInfo* tempchatinfo = ntf.m_Data.mutable_chatinfo();
	tempchatinfo->CopyFrom(*chatinfo);
	tempchatinfo->set_time((UINT32)time(NULL));
	if (GSConfig::Instance()->IsCrossGS())
	{
		if (serverID)
		{
			ServerMgr::Instance()->SendTo(serverID, ntf);
		}
		else
		{
			SSError << " cross gs send serverid = 0, noticeid = " << noticeid << END;
			return false;
		}
	}
	else
	{
		MSLink::Instance()->SendTo(ntf);
	}

	return true;
}

bool Notice::SendInCrossGs(UINT32 serverID)
{
	return Send(serverID);
}

bool Notice::SendSelf(UINT64 roleid)
{
	if (!CanSend())	
	{
		return false;
	}

	chatinfo->set_isbroadcast(false);

	PtcG2M_NoticeForwardNtf ntf;
	ntf.m_Data.set_type(KKSG::Notice_Role);
	ntf.m_Data.set_uid(roleid);
	KKSG::ChatInfo* tempchatinfo = ntf.m_Data.mutable_chatinfo();
	tempchatinfo->CopyFrom(*chatinfo);
	tempchatinfo->set_time((UINT32)time(NULL));
	if (GSConfig::Instance()->IsCrossGS())
	{
		ServerMgr::Instance()->SendByRoleId(roleid, ntf);
	}
	else
	{
		MSLink::Instance()->SendTo(ntf);
	}

	return true;
}

bool Notice::SendSelf(Role* role)
{
	return SendSelf(role->GetID());
}

bool Notice::SendTeam(int teamid)
{
	if (!CanSend())
	{
		return false;
	}

	if (HTeam::IsTsTeam(teamid))
	{
		SSError << " is cross team teamid = " << teamid << END;
		return false;
	}
	if (GSConfig::Instance()->IsCrossGS())
	{
		SSError << " in cross gs teamid = " << teamid << END;
		return false;
	}

	chatinfo->set_isbroadcast(false);

	PtcG2M_NoticeForwardNtf ntf;
	ntf.m_Data.set_type(KKSG::Notice_Team);
	ntf.m_Data.set_uid(teamid);
	KKSG::ChatInfo* tempchatinfo = ntf.m_Data.mutable_chatinfo();
	tempchatinfo->CopyFrom(*chatinfo);
	tempchatinfo->set_time((UINT32)time(NULL));
	MSLink::Instance()->SendTo(ntf);

	return true;
}

bool Notice::SendGuild(UINT64 guildid)
{
	if (!CanSend())
	{
		return false;
	}

	// to do...
	//CGuildChat* guildchat = guild->Get<CGuildChat>();
	//guildchat->Push(*tempchatinfo);

	chatinfo->set_isbroadcast(false);
	
	PtcG2M_NoticeForwardNtf ntf;
	ntf.m_Data.set_type(KKSG::Notice_Guild);
	ntf.m_Data.set_uid(guildid);
	KKSG::ChatInfo* tempchatinfo = ntf.m_Data.mutable_chatinfo();
	tempchatinfo->CopyFrom(*chatinfo);
	tempchatinfo->set_time((UINT32)time(NULL));
	if (GSConfig::Instance()->IsCrossGS())
	{
		ServerMgr::Instance()->SendTo(GetServerIDByGuildID(guildid), ntf);
	}
	else
	{
		MSLink::Instance()->SendTo(ntf);
	}

	return true;
}

bool Notice::SendRoleGuild(Role* role)
{
	if (!CanSend())
	{
		return false;
	}

	chatinfo->set_isbroadcast(false);

	PtcG2M_NoticeForwardNtf ntf;
	ntf.m_Data.set_type(KKSG::Notice_Guild);
	ntf.m_Data.set_roleid(role->GetID());
	KKSG::ChatInfo* tempchatinfo = ntf.m_Data.mutable_chatinfo();
	tempchatinfo->CopyFrom(*chatinfo);
	tempchatinfo->set_time((UINT32)time(NULL));
	role->SendToMS(ntf);

	return true;
}

RawNotice::RawNotice(UINT32 id)
{
	const std::string* info = NoticeMgr::Instance()->GetInfo(id);
	if (NULL != info)
	{
		hint = *info;
	}
}

void RawNotice::Replace(const std::string& src, const std::string& dest)
{
	int start = hint.find(src);
	if(std::string::npos == (size_t)start)
	{
		return;
	}
	int end = src.size();
	hint.replace(start, end, dest);
}

void RawNotice::Send(Role* role)
{
	if (!hint.empty())
	{
		PtcG2C_WordNotify notify;
		*notify.m_Data.mutable_hint() = hint;
		role->Send(notify);
	}
}

