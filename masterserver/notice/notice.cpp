#include "pch.h"
#include "notice.h"
#include "noticemgr.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "chat/ptcm2t_chatforward.h"
#include "network/gatelink.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "partner/partnermgr.h"
#include "dragonguild/dragonguildmgr.h"

Notice::Notice(UINT32 id)
{
	cansend = true;
	type = KKSG::Notice_World;
	uid = 0;
	offset = 0;
	noticeid = id;
	chatinfo = new KKSG::ChatInfo;
	UINT32 noticechannel = NoticeMgr::Instance()->GetChannel(id);
	UINT32 priority = NoticeMgr::Instance()->GetPriority(id);
	const std::string* info = NoticeMgr::Instance()->GetInfo(id);
	paramtypes = NoticeMgr::Instance()->GetParamTypes(id);
	level = NoticeMgr::Instance()->GetLevel(id);
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

Notice::Notice(UINT32 channel, UINT32 priority, std::string info)
{
	cansend = true;
	type = KKSG::Notice_World;
	uid = 0;
	offset = 0;
	noticeid = 0;
	paramtypes = NULL;
	level = 0;
	chatinfo = new KKSG::ChatInfo;
	chatinfo->set_info(info);
	chatinfo->set_channel(channel);
	chatinfo->set_priority(priority);
	chatinfo->set_isbroadcast(true);
}

Notice::Notice(KKSG::NoticeForwardNtf& ntf)
{
	cansend = true;
	offset = 0;
	noticeid = 0;
	paramtypes = NULL;
	level = 0;
	type = ntf.type();
	uid = ntf.uid();
	chatinfo = new KKSG::ChatInfo;
	*chatinfo =	*ntf.mutable_chatinfo();

	FillGuildName();
}

Notice::~Notice()
{
	if (NULL != chatinfo)
	{
		delete chatinfo;
	}
}

bool Notice::Send()
{
	if (!cansend)
	{
		return false;
	}

	if (KKSG::Notice_World == type)
	{
		PtcM2T_chatforward chatforward;
		KKSG::ChatInfo* tempchatinfo = chatforward.m_Data.mutable_chatinfo();
		tempchatinfo->CopyFrom(*chatinfo);
		tempchatinfo->set_time((UINT32)time(NULL));
		GateLink::Instance()->SendToAllLine(chatforward);		
		return true;
	}
	std::vector<SessionType> sessions;
	GetForwardSession(type, uid, sessions);
	for (auto i = sessions.begin(); i != sessions.end(); ++i)
	{
		PtcM2T_chatforward chatforward;
		KKSG::ChatInfo* tempchatinfo = chatforward.m_Data.mutable_chatinfo();
		tempchatinfo->CopyFrom(*chatinfo);
		tempchatinfo->set_time((UINT32)time(NULL));
		chatforward.m_sessionID = *i;
		GateLink::Instance()->SendToSession(chatforward);
	}
	return true;
}

bool Notice::SendGuild(UINT64 guildid)
{
	uid = guildid;
	type = KKSG::Notice_Guild;
	chatinfo->set_isbroadcast(false);
	return Send();
}

bool Notice::SendPartner(UINT64 partnerId)
{
	uid = partnerId;
	type = KKSG::Notice_Partner;
	chatinfo->set_isbroadcast(false);
	return Send();
}

bool Notice::SendServerID(UINT32 serverID)
{
	uid = serverID;
	type = KKSG::Notice_Server;
	chatinfo->set_isbroadcast(false);
	return Send();
}

bool Notice::SendPlays(std::vector<UINT64>& roles)
{
	chatinfo->set_isbroadcast(false);

	if (!cansend)
	{
		return false;
	}

	std::vector<SessionType> sessions;
	for ( auto it = roles.begin(); it != roles.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(*it);
		if (NULL != role)
		{
			sessions.push_back(role->GetSessionID());
		}
	}
	
	for (auto i = sessions.begin(); i != sessions.end(); ++i)
	{
		PtcM2T_chatforward chatforward;
		KKSG::ChatInfo* tempchatinfo = chatforward.m_Data.mutable_chatinfo();
		tempchatinfo->CopyFrom(*chatinfo);
		tempchatinfo->set_time((UINT32)time(NULL));
		chatforward.m_sessionID = *i;
		GateLink::Instance()->SendToSession(chatforward);
	}

	return true;
}
bool Notice::SendRole(UINT64 roleid)
{
	uid = roleid;
	type = KKSG::Notice_Role;
	chatinfo->set_isbroadcast(false);
	return Send();
}

void Notice::GetForwardSession(UINT32 type, UINT64 uid, std::vector<SessionType>& sessions)
{
	switch (type)
	{
	case KKSG::Notice_Guild:
		{
			Guild* guild = CGuildMgr::Instance()->GetGuild(uid);
			if (NULL != guild)
			{
				guild->GetSessions(sessions);
			}
			break;
		}
	case KKSG::Notice_Team:
		{
			Team* team = TeamMgr::Instance()->FindTeam(uid);
			if (NULL != team)
			{
				team->GetSessions(sessions);
			}
			break;
		}
	case KKSG::Notice_Role:
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(uid);
			if (NULL != role)
			{
				sessions.push_back(role->GetSessionID());
			}
			break;
		}
	case KKSG::Notice_Partner:
		{
			Partner* p = PartnerMgr::Instance()->GetPartner(uid);
			if (NULL != p)
			{
				p->GetSessions(sessions);
			}
			break;
		}
	case KKSG::Notice_Server:
		{
			auto begin = CRoleManager::Instance()->Begin();
			auto end = CRoleManager::Instance()->End();
			for (auto iter = begin; iter != end; ++iter)
			{
				CRole* role = *iter;
				if (NULL != role && role->GetServerId() == uid)
				{
					sessions.push_back(role->GetSessionID());
				}
			}
			break;
		}
	default:
		break;
	}
}

void Notice::FillGuildName()
{
	for (int i = 0; i < chatinfo->param_size(); ++i)
	{
		const KKSG::ChatParam& param = chatinfo->param(i);
		if (param.has_guild())
		{
			Guild* guild = CGuildMgr::Instance()->GetGuild(param.guild().guildid());
			if (NULL != guild)
			{
				chatinfo->mutable_param(i)->mutable_guild()->set_guildname(guild->GetName());
			}
		}
	}
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

bool Notice::AddItem(UINT32 itemid, UINT32 itemcount)
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
		paramitem->mutable_item()->set_itemid(itemid);
		paramitem->mutable_item()->set_itemcount(itemcount);
	}
	++offset;
	return cansend;
}

bool Notice::AddRole(CRoleSummary* summary)
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
		if (NULL != summary)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamRole* paramrole = param->mutable_role();
			paramrole->set_uniqueid(summary->GetID());
			paramrole->set_name(summary->GetName());
			paramrole->set_profession(summary->GetProfession());
		}
	}
	++offset;
	return cansend;
}

bool Notice::AddLink()
{
	if (!cansend)
	{
		return false;
	}
	if (ParamLink != GetParamType())
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

bool Notice::AddLink(Guild* guild)
{
	if (!cansend)
	{
		return false;
	}
	if (ParamLink != GetParamType())
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
			if (NULL != guild)
			{
				KKSG::ChatParamGuild* paramguild = param->mutable_guild();
				paramguild->set_guildid(guild->GetID());
				paramguild->set_guildname(guild->GetName());
			}
		}
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
		Guild* guild = CGuildMgr::Instance()->GetGuild(guildid);
		paramguild->set_guildname(NULL == guild ? "" : guild->GetName());
	}
	++offset;
	return cansend;
}

bool Notice::AddDragonGuild(UINT64 dragonguildId)
{
	if (!cansend)
	{
		return false;
	}

	if (ParamDragonGuild != GetParamType())
	{
		cansend = false;
	}
	else
	{
		KKSG::ChatParam* param = chatinfo->add_param();
		if(param == NULL)
			return false;
		KKSG::ChatParamDragonGuild* paramDragonGuild = param->mutable_dragonguild();
		if(paramDragonGuild == NULL)
			return false;
		paramDragonGuild->set_dragonguildid(dragonguildId);
		DragonGuild* pDragonGuild = DragonGuildMgr::Instance()->GetDragonGuild(dragonguildId);
		if(pDragonGuild == NULL)
			return false;
		paramDragonGuild->set_dragonguildname(pDragonGuild ? pDragonGuild->GetName() : "");
	}

	++ offset;
	return cansend;
}

bool Notice::AddLink(DragonGuild* dragonguild)
{
	if (!cansend)
	{
		return false;
	}

	if (ParamLink != GetParamType())
	{
		cansend = false;
	}
	else
	{
		NoticeLinkInfo* info = NoticeMgr::Instance()->GetLinkInfo(noticeid);
		if (NULL != info)
		{
			KKSG::ChatParam* param = chatinfo->add_param();
			KKSG::ChatParamLink* ParamLink = param->mutable_link();
			ParamLink->set_content(info->content);
			ParamLink->set_id(info->param);
			if (NULL != dragonguild)
			{
				KKSG::ChatParamDragonGuild* paramDragonGuild = param->mutable_dragonguild();
				paramDragonGuild->set_dragonguildid(dragonguild->GetID());
				paramDragonGuild->set_dragonguildname(dragonguild->GetName());
			}
		}
	}

	++ offset;
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
