#include "pch.h"
#include "timeutil.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "network/gatelink.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "chatmgr.h"
#include "chat/ptcm2t_chatforward.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "chat/ptcm2d_mduploadchatntf.h"
#include "chat/ptcm2c_mcchatofflinenotify.h"
#include "util/timespecificmgr.h"
#include "chat/ptcm2d_updateprivatelistntf.h"
#include "network/dblink.h"
#include "friend/friendmgr.h"
#include "chat/ptcm2c_idipclearchatntf.h"
#include "partner/partnermgr.h"
#include "chat/ptcm2d_mddelchatntf.h"
#include "groupchat/groupchatmgr.h"

INSTANCE_SINGLETON(CChatMgr)

//保存数据库一次操作最大数量
#define MAX_SAVE_NUM_ONCE_GUILD 6		//公会个数
#define MAX_SAVE_NUM_ONCE_ROLE	9		//人个数，私聊
#define MAX_SAVE_NUM_ONCE_LIST	50		//人个数，私聊列表
#define MAX_SAVE_NUM_ONCE_PARTNER 10	//战友数
#define MAX_SAVE_NUM_ONCE_GROUP 10      //群聊数

#define MAX_CHAT_NUMBER			200		//最大保存聊天条数
#define MAX_PRIVATELIST_NUM		100		//最大聊天列表个数

CChatMgr::CChatMgr():m_isChangedWorld(false), m_saveToDBCount(0)
{
}

CChatMgr::~CChatMgr()
{
}

bool CChatMgr::Init()
{
	m_timeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1*1000, -1, __FILE__, __LINE__);
	if(m_timeHandler == INVALID_HTIMER)
	{
		return false;
	}
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	if(!QueryChatRecordDB())
	{
		return false;
	}
	if(!QueryPrivateChatListDB())
	{
		return false;
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all chat data(chatrecord and privatechatlist) from db use time: %llu ms", qwTimeUsed);
	return true;
}

void CChatMgr::Uninit()
{
	if(m_timeHandler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_timeHandler);
	}
}

void CChatMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT64 begin = TimeUtil::GetMilliSecond();
	string ss;
	bool isUpdate = false;

	//每1*4秒更新每一个
	if (0 == m_saveToDBCount)
	{	
		isUpdate = SaveGuildChatToDB(MAX_SAVE_NUM_ONCE_GUILD);
		if (!m_isChangedGuild.empty())
		{
			LogInfo("guild chat still need save num:%u", m_isChangedGuild.size());
		}
		ss = "guild";
	}
	else if (1 == m_saveToDBCount)
	{
		isUpdate = SavePrivateChatToDB(MAX_SAVE_NUM_ONCE_ROLE);
		if(!m_isChangedRole.empty())
		{
			LogInfo("role chat still need save num:%u", m_isChangedRole.size());
		}
		ss = "role";
	}
	else if (2 == m_saveToDBCount)
	{
		SaveWorldChatToDB();
		isUpdate = SavePrivateListToDB(MAX_SAVE_NUM_ONCE_LIST);
		if(!m_isChangedPrivateList.empty())
		{
			LogInfo("role privatelist still need save:%u", m_isChangedPrivateList.size());
		}
		ss = "role privatelist";
	}
	else if (3 == m_saveToDBCount)
	{
		isUpdate = SavePartnerChatToDB(MAX_SAVE_NUM_ONCE_PARTNER);
		if(!m_isChangedPartner.empty())
		{
			LogInfo("partner chat still need save num:%u", m_isChangedPartner.size());
		}
		ss = "partner";
	}
    else if (4 == m_saveToDBCount)
    {	
        isUpdate = SaveGroupChatToDB(MAX_SAVE_NUM_ONCE_GROUP);
        if (!m_isChangedGroup.empty())
        {
            LogInfo("group chat still need save num:%u", m_isChangedGroup.size());
        }
        ss = "group";
    }
	m_saveToDBCount = (m_saveToDBCount + 1) % 5;

	UINT64 end = TimeUtil::GetMilliSecond();
	if (isUpdate)
	{
		LogInfo("%s chat save to db use time: %llu ms", ss.c_str(), end - begin);
	}
}

void CChatMgr::SaveAllToDB()
{
	SaveWorldChatToDB();
	SaveGuildChatToDB(-1);
    SaveGroupChatToDB(-1);
	SavePartnerChatToDB(-1);
	SavePrivateChatToDB(-1);
	SavePrivateListToDB(-1);
}

bool CChatMgr::QueryChatRecordDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss << "select channel, storekey, chatinfolist from chatrecord";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table chatrecord failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	KKSG::ChatInfoList oInfoList;

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		UINT32 channel;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			channel = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'channel' column failed");
			return false;
		}

		UINT64 storekey;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			storekey = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'storekey' column failed");
			return false;
		}

		if(!poRes->GetFieldValue(2, &pVal, dwLen))
		{
			LogError("Read 'chatinfolist' column failed");
			return false;
		}
		
		if(!oInfoList.ParseFromArray(pVal, dwLen))
		{
			LogError("Parse ChatInfoList failed, channel:%u, storekey:%llu", channel, storekey);
			return false;
		}

		for(int i = 0; i < oInfoList.chatinfolist_size(); i++)
		{
			if(oInfoList.chatinfolist(i).dest().roleid_size() != 0)
			{
				oInfoList.mutable_chatinfolist(i)->add_destlist()->set_roleid(oInfoList.chatinfolist(i).dest().roleid(0));
				oInfoList.mutable_chatinfolist(i)->clear_dest();
			}
			AddChat(storekey, oInfoList.chatinfolist(i), true);
		}
	}
	poRes->Release();

	return true;
}

bool CChatMgr::QueryPrivateChatListDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss << "select * from privatechatlist";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table privatechatlist failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	KKSG::PrivateChatList oChatList;

	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT64 qwRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			qwRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		if(!poRes->GetFieldValue(1, &pVal, dwLen))
		{
			LogError("Read 'privatechatlist' column failed");
			return false;
		}

		if(!oChatList.ParseFromArray(pVal, dwLen))
		{
			LogError("Parse privatechatlist failed, role %llu", qwRoleID);
			return false;
		}

		UINT32 qwSize = oChatList.rolelist().size();
		if(qwSize != oChatList.hasofflinechat_size() || qwSize != oChatList.lastchattime_size())
		{
			LogError("rolelist and messagecount size is not equal");
			return false;
		}
		for(UINT32 i = 0; i < qwSize; i++)
		{
			PrivateList privatelist;
			privatelist.roleid = oChatList.rolelist(i).roleid();
			privatelist.hasOfflineChat = oChatList.hasofflinechat(i);
			privatelist.lastChatTime = oChatList.lastchattime(i);
			m_mapPrivateList[qwRoleID].push_back(privatelist);
		}
	}

	poRes->Release();

	return true;
}

void CChatMgr::AddChat(UINT64 storekey, const KKSG::ChatInfo& chatinfo, bool isLoadInDB)
{
	if(KKSG::WorldChannel == chatinfo.channel())
	{
		m_worldChat.push_back(chatinfo);
		while(m_worldChat.size() > MAX_CHAT_NUMBER)
		{
			m_worldChat.erase(m_worldChat.begin());
		}

		if(!isLoadInDB)
		{
			m_isChangedWorld = true;
		}	
		return;
	}
	if(KKSG::GuildChannel == chatinfo.channel())
	{
		auto itFind = m_mapGuildChat.find(storekey);
		if(itFind == m_mapGuildChat.end())
		{
			m_mapGuildChat[storekey].push_back(chatinfo);
		}
		if(itFind != m_mapGuildChat.end())
		{
			itFind->second.push_back(chatinfo);
			while(itFind->second.size() > MAX_CHAT_NUMBER)
			{
				itFind->second.erase(itFind->second.begin());
			}
		}

		if(!isLoadInDB)
		{
			m_isChangedGuild.push(storekey);
		}
		return;
	}
    if(KKSG::GroupChatChannel == chatinfo.channel())
    {
        auto itFind = m_mapGroupChat.find(storekey);
        if(itFind == m_mapGroupChat.end())
        {
            m_mapGroupChat[storekey].push_back(chatinfo);
        }
        if(itFind != m_mapGroupChat.end())
        {
            itFind->second.push_back(chatinfo);
            while(itFind->second.size() > MAX_CHAT_NUMBER)
            {
                itFind->second.erase(itFind->second.begin());
            }
        }

        if(!isLoadInDB)
        {
            m_isChangedGroup.push(storekey);
        }
        return;
    }
	if (KKSG::PartnerChannel == chatinfo.channel())
	{
		auto it = m_mapPartnerChat.find(storekey);
		if (it == m_mapPartnerChat.end())
		{
			it = m_mapPartnerChat.insert(std::make_pair(storekey, std::list<KKSG::ChatInfo>())).first;
		}
		it->second.push_back(chatinfo);
		while(it->second.size() > MAX_CHAT_NUMBER)
		{
			it->second.erase(it->second.begin());
		}

		if (!isLoadInDB)
		{
			m_isChangedPartner.push(storekey);
		}

		return;
	}
	if(KKSG::PrivateChannel == chatinfo.channel())
	{
		if(0 == chatinfo.destlist_size())
		{
			return;
		}

		AddPrivateChat(storekey, chatinfo, isLoadInDB);

		if(!isLoadInDB)
		{
			m_isChangedRole.push(storekey);
		}
		return;
	}
	///>SSError << "Invalid Chat Channel:" << channel << ", StoreKeyID:" << storekeyID << ", RoleID:" << info.source().roleid() << ", Name:" << info.source().name() << END;	
}

void CChatMgr::AddPrivateChat(UINT64 roleid, const KKSG::ChatInfo& chatinfo, bool isLoadInDB)
{
	auto itFind = m_mapPrivateChat.find(roleid);
	if(itFind == m_mapPrivateChat.end())
	{
		m_mapPrivateChat[roleid].push_back(chatinfo);
	}
	else
	{
		itFind->second.push_back(chatinfo);
		while(itFind->second.size() > MAX_CHAT_NUMBER)
		{
			itFind->second.erase(itFind->second.begin());
		}
	}

	if(isLoadInDB) //从数据库读出来，不需要更新privatechatlist
	{
		return;
	}

	UINT64 from = chatinfo.source().roleid();
	UINT64 to = chatinfo.destlist(0).roleid();

	UINT64 otherid = from == roleid ? to : from;

	bool hasOffline = false;
	std::list<PrivateList>& privateList = m_mapPrivateList[roleid];
	for(auto i = privateList.begin(); i != privateList.end(); i++)
	{
		const PrivateList& list = *i;
		if(list.roleid == otherid)
		{
			hasOffline = list.hasOfflineChat;
			privateList.erase(i); //删除原来的
			break;
		}
	}

	if(from == roleid)
	{
		m_mapPrivateList[from].push_back(PrivateList(to, false, chatinfo.time()));
	}
	else
	{
		bool isOffline = !CRoleManager::Instance()->IsOnline(to) || hasOffline;
		m_mapPrivateList[to].push_back(PrivateList(from, isOffline, chatinfo.time()));
	}

	while(privateList.size() > MAX_PRIVATELIST_NUM)
	{
		privateList.erase(privateList.begin());
	}

	m_isChangedPrivateList.push(roleid);
}

void CChatMgr::SendUser(UINT64 roleID, const KKSG::ChatInfo& chatinfo, bool isPrivate)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		return; 
	}
	PtcM2T_chatforward chatforward;
	KKSG::ChatInfo* pInfo = chatforward.m_Data.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	FillSourceData(pInfo->mutable_source());

	if(isPrivate)
	{	
		FillSourceData(pInfo->mutable_destlist(0));
	}

	pRole->Send(chatforward);
	///>SSDebug << "SendUser, Send M2T Message To Client, CMD=PtcM2T_chatforward, RoleID:" << chatinfo.source().roleid() << ", Name:" << chatinfo.source().name() << END;
}

void CChatMgr::SendGuild(Guild* pGuild, const KKSG::ChatInfo& chatinfo)
{
	PtcM2T_chatforward chatforward;
	KKSG::ChatInfo* pInfo = chatforward.m_Data.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	FillSourceData(pInfo->mutable_source());

	pGuild->sendCmdToAllMember(chatforward);

	///>SSDebug << "SendGuild, Send M2T Message To Client, CMD=PtcM2T_chatforward, RoleID:" << chatinfo.source().roleid() << ", Name:" << chatinfo.source().name() << ", GuildID:" << pGuild->GetGuildUId() << END;
}

void CChatMgr::SendTeam(Team* pTeam, const KKSG::ChatInfo& chatinfo)
{
	PtcM2T_chatforward chatforward;
	KKSG::ChatInfo* pInfo = chatforward.m_Data.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	FillSourceData(pInfo->mutable_source());

	std::vector<SessionType> vecSessionIndex;
	vecSessionIndex.clear();
	pTeam->GetSessions(vecSessionIndex);
	for(UINT32 i = 0; i < vecSessionIndex.size(); i++)
	{
		UINT64 curSessionIndex = vecSessionIndex[i];
		chatforward.m_sessionID = curSessionIndex;
		GateLink::Instance()->SendToSession(chatforward);
	}
	///>SSDebug << "SendTeam, Send M2T Message To Client, CMD=PtcM2T_chatforward, RoleID:" << chatinfo.source().roleid() << ", Name:" << chatinfo.source().name() << ", TeamID:" << pTeam->GetTeamUId() << END;
}

void CChatMgr::SendPartner(const KKSG::ChatInfo& chatinfo)
{
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(chatinfo.source().roleid());
	if (partner == NULL)
	{
		return;
	}

	PtcM2T_chatforward chatforward;
	KKSG::ChatInfo* pInfo = chatforward.m_Data.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	FillSourceData(pInfo->mutable_source());
	partner->BroadCast(chatforward);

	AddChat(partner->GetID(), chatinfo, false);
}

void CChatMgr::SendWorld(const KKSG::ChatInfo& chatinfo)
{
	PtcM2T_chatforward chatforward;
	KKSG::ChatInfo* pInfo = chatforward.m_Data.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	FillSourceData(pInfo->mutable_source());
	pInfo->set_isbroadcast(true);

	GateLink::Instance()->SendToAllLine(chatforward);
}

///>发送给组：一批指定roleID的用户
void CChatMgr::SendGroup(const std::vector<UINT64>& roleList, const KKSG::ChatInfo& chatinfo)
{
	for(UINT32 i = 0; i < roleList.size(); i++)
	{
		SendUser(roleList[i], chatinfo, false);
	}
}

void CChatMgr::SendGroupChat(const std::map<UINT64, UINT32>& roleList, const KKSG::ChatInfo& chatinfo)
{
    for(auto itor = roleList.begin(); itor != roleList.end(); ++itor)
    {
        SendUser(itor->first, chatinfo, false);
    }
}

///>离线消息--上线时，将缓存消息推送给用户
void CChatMgr::OffLineChatNotify(CRole* pRole)
{
	FriendMgr::Instance()->SendNoNotifyAddFriend(pRole->GetID()); //放在firstenterscene里因为还没完全进入场景不能显示，所以就放在这里了

	PtcM2C_MCChatOffLineNotify oOffLineNtfPtc;
	///>world
	for(auto i = m_worldChat.begin(); i != m_worldChat.end(); i++)
	{
		if((*i).time() < pRole->GetCreateTime())
		{
			continue;
		}
		KKSG::ChatInfo* chatinfo = oOffLineNtfPtc.m_Data.add_worldchat();
		chatinfo->CopyFrom(*i);
		FillSourceData(chatinfo->mutable_source());
	}

	///>private
	UINT32 privateNum = 0;
	auto itFind = m_mapPrivateChat.find(pRole->GetID());
	if(itFind != m_mapPrivateChat.end())
	{
		for(auto i = itFind->second.begin(); i != itFind->second.end(); i++)
		{
			KKSG::ChatInfo* chatinfo = oOffLineNtfPtc.m_Data.add_rolechat();
			chatinfo->CopyFrom(*i);
			FillSourceData(chatinfo->mutable_source());
			FillSourceData(chatinfo->mutable_destlist(0));
		}
		privateNum = itFind->second.size();
	}

	///>guild
	UINT32 guildNum = 0;
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if(NULL != guild)
	{
		itFind = m_mapGuildChat.find(guild->GetID());
		if(itFind != m_mapGuildChat.end())
		{
			UINT32 joinTime = guild->GetJoinTime(pRole->GetID());
			for(auto i = itFind->second.begin(); i != itFind->second.end(); i++)
			{
				if((*i).time() < joinTime) //previous message before join guild
				{
					continue;
				}
				KKSG::ChatInfo* chatinfo = oOffLineNtfPtc.m_Data.add_guildchat();
				chatinfo->CopyFrom(*i);
				FillSourceData(chatinfo->mutable_source());
			}
			guildNum = itFind->second.size();
		}
	}

    ///>GroupChat
    UINT32 groupNum = 0;
    GroupChatRoleInfo* pRoleInfo = GroupChatMgr::Instance()->GetRoleInfo(pRole->GetID());
    if(NULL != pRoleInfo)
    {
        std::set<UINT64>& groupidlist = GroupChatMgr::Instance()->GetRoleGroupList(*pRoleInfo);
        for(auto itor = groupidlist.begin(); itor != groupidlist.end(); ++itor)
        {
            itFind = m_mapGroupChat.find(*itor);
            if(itFind != m_mapGroupChat.end())
            {
                UINT32 tmpcount = 0;
                UINT32 tmpsize = itFind->second.size();
                UINT32 tmpindex = 0;
                for(auto i = itFind->second.begin(); i != itFind->second.end(); i++)
                {
                    tmpindex++;
                    UINT32 chatinfotime = (*i).time();
                    UINT32 joingrouptime = GroupChatMgr::Instance()->GetGroupMemberJoinTime(*itor, pRole->GetID());
                    if(chatinfotime < joingrouptime && tmpindex+0 < tmpsize)
                    {
                        continue;
                    }
                    if(chatinfotime < pRoleInfo->timeLastClear && tmpindex+0 < tmpsize) //previous message before join guild
                    {
                        continue;
                    }
                    if(!i->has_source())
                    {
                        LogInfo("GroupChat source is null. roleid = %llu\n", pRole->GetID());
                        continue;
                    }
                    if(!i->has_groupchatinfo())
                    {
                        LogInfo("GroupChat groupchatinfo is null. roleid = %llu\n", pRole->GetID());
                        continue;
                    }
                    KKSG::ChatInfo* chatinfo = oOffLineNtfPtc.m_Data.add_groupchat();
                    chatinfo->CopyFrom(*i);
                    FillSourceData(chatinfo->mutable_source());
                    tmpcount++;
                }
                groupNum += tmpcount;
            }
        }
    }

	///>partner
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner != NULL)
	{
		auto it = m_mapPartnerChat.find(partner->GetID());
		if (it != m_mapPartnerChat.end())
		{
			for(auto i = it->second.begin(); i != it->second.end(); i++)
			{
				KKSG::ChatInfo* chatinfo = oOffLineNtfPtc.m_Data.add_partnerchat();
				chatinfo->CopyFrom(*i);
				FillSourceData(chatinfo->mutable_source());
			}
		}
	}

	///>privatechatlist
	auto itBegin = m_mapPrivateList[pRole->GetID()].begin();
	auto itEnd = m_mapPrivateList[pRole->GetID()].end();
	for(auto i = itBegin; i != itEnd; i++)
	{
		KKSG::PrivateChatList* chatlist = oOffLineNtfPtc.m_Data.mutable_privatechatlist();
		KKSG::ChatSource* source = chatlist->add_rolelist();
		source->set_roleid((*i).roleid);
		FillSourceData(source);
		chatlist->add_hasofflinechat((*i).hasOfflineChat);
		chatlist->add_lastchattime((*i).lastChatTime);
	}
	
	pRole->Send(oOffLineNtfPtc);
	SSDebug << "Send M2C message, cmd=PtcM2C_MCChatOffLineNotify, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", World:" << m_worldChat.size() << ", Guild:" << guildNum << ", Group Chat:" << groupNum << ", Private:" << privateNum << END;
}

void CChatMgr::ClearPrivateList(UINT64 roleid, UINT64 otherid)
{
	if(m_mapPrivateList.find(roleid) != m_mapPrivateList.end())
	{
		m_mapPrivateList[roleid].remove(PrivateList(otherid, 0, 0));

		m_isChangedPrivateList.push(roleid);
	}
}

void CChatMgr::ClearPrivateListAll(UINT64 roleid)
{
	if(m_mapPrivateList.find(roleid) == m_mapPrivateList.end())
	{
		return;
	}
	UINT32 sz = m_mapPrivateList[roleid].size();
	if(sz)
	{
		m_mapPrivateList[roleid].clear();

		m_isChangedPrivateList.push(roleid);
	}
}

void CChatMgr::OpenPrivateChat(UINT64 roleid, UINT64 otherid)
{
	if(m_mapPrivateList.find(roleid) == m_mapPrivateList.end())
	{
		return;
	}
	for(auto i = m_mapPrivateList[roleid].begin(); i != m_mapPrivateList[roleid].end(); i++)
	{
		PrivateList& list = *i;
		if(list.roleid == otherid)
		{
			if(list.hasOfflineChat)
			{
				list.hasOfflineChat = false;
				
				m_isChangedPrivateList.push(roleid);
			}
			return;
		}
	}
}

void CChatMgr::FillSourceData(KKSG::ChatSource* source)
{
	if (source == NULL) return;
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(source->roleid());
	if (pRoleSummary == NULL)
	{
		return;
	}

	const KKSG::RoleSummaryStored& roleData = pRoleSummary->GetData();
	source->set_profession(roleData.profession());
	source->set_name(roleData.name());
	source->set_viplevel(roleData.viplevel());
	source->set_powerpoint(roleData.powerpoint());
	source->set_coverdesignationid(roleData.desigation());
	std::string  covername =  GetGlobalConfig().GetCoverName(roleData.covername(),roleData.covertype());
	source->set_desname(covername);
	source->set_paymemberid(pRoleSummary->GetPayMemberID());
	source->set_military_rank(pRoleSummary->GetMilitaryRank());
	source->set_isbackflow(roleData.backflowendtime() >= GameTime::GetTime());
	for (UINT32 i = 0; i < roleData.pre().setid_size(); i ++)
	{
		source->mutable_pre()->add_setid(roleData.pre().setid(i));
	}
}

void CChatMgr::SaveWorldChatToDB()
{
	if(!m_isChangedWorld) return;	

	PtcM2D_MDUpLoadChatNtf oLoadPtc;
	oLoadPtc.m_Data.set_channel(KKSG::WorldChannel);

	KKSG::ChatInfoList oInfoList;
	for(auto i = m_worldChat.begin(); i != m_worldChat.end(); i++)
	{
		KKSG::ChatInfo* chatinfo = oInfoList.add_chatinfolist();
		chatinfo->CopyFrom(*i);
	}

	oLoadPtc.m_Data.set_chatinfolist(oInfoList.SerializeAsString());
	DBLink::Instance()->Send(DBLink::Instance()->GetConnID(), oLoadPtc);

	m_isChangedWorld = false;
}

bool CChatMgr::SaveGuildChatToDB(UINT32 maxCount)
{
	if(m_isChangedGuild.empty()) return false;	

	PtcM2D_MDUpLoadChatNtf oLoadPtc;
	oLoadPtc.m_Data.set_channel(KKSG::GuildChannel);
	
	UINT32 count = 0;
	std::string tmpString;
	while(!m_isChangedGuild.empty() && count < maxCount)
	{
		UINT64 guildid = m_isChangedGuild.front();
		m_isChangedGuild.pop();
		count++;

		if(m_mapGuildChat.find(guildid) == m_mapGuildChat.end())
		{
			LogError("guildid:%llu, can't find in GuildChat", guildid);
			assert(false);
			continue;
		}

		oLoadPtc.m_Data.set_storekey(guildid);

		KKSG::ChatInfoList oInfoList;
		auto itBegin = m_mapGuildChat[guildid].begin();
		auto itEnd = m_mapGuildChat[guildid].end();
		for(auto j = itBegin; j != itEnd; j++)
		{	
			KKSG::ChatInfo* chatinfo = oInfoList.add_chatinfolist();
			chatinfo->CopyFrom(*j);
		}

		oInfoList.SerializeToString(&tmpString);
		oLoadPtc.m_Data.set_chatinfolist(tmpString);

		DBLink::Instance()->Send(DBLink::Instance()->GetConnID(), oLoadPtc);
	}

	return true;
}

bool CChatMgr::SaveGroupChatToDB(UINT32 maxCount)
{
    if(m_isChangedGroup.empty()) return false;	

    PtcM2D_MDUpLoadChatNtf oLoadPtc;
    oLoadPtc.m_Data.set_channel(KKSG::GroupChatChannel);

    UINT32 count = 0;
    std::string tmpString;
    while(!m_isChangedGroup.empty() && count < maxCount)
    {
        UINT64 groupid = m_isChangedGroup.front();
        m_isChangedGroup.pop();
        count++;

        if(m_mapGroupChat.find(groupid) == m_mapGroupChat.end())
        {
            LogError("guildid:%llu, can't find in GroupChat", groupid);
            assert(false);
            continue;
        }

        oLoadPtc.m_Data.set_storekey(groupid);

        KKSG::ChatInfoList oInfoList;
        auto itBegin = m_mapGroupChat[groupid].begin();
        auto itEnd = m_mapGroupChat[groupid].end();
        for(auto j = itBegin; j != itEnd; j++)
        {	
            KKSG::ChatInfo* chatinfo = oInfoList.add_chatinfolist();
            chatinfo->CopyFrom(*j);
        }

        oInfoList.SerializeToString(&tmpString);
        oLoadPtc.m_Data.set_chatinfolist(tmpString);

        DBLink::Instance()->Send(DBLink::Instance()->GetConnID(), oLoadPtc);
    }

    return true;
}

bool CChatMgr::SavePartnerChatToDB(UINT32 maxCount)
{
	if(m_isChangedPartner.empty()) return false;	

	PtcM2D_MDUpLoadChatNtf oLoadPtc;
	oLoadPtc.m_Data.set_channel(KKSG::PartnerChannel);
	
	UINT32 count = 0;
	std::string tmpString;
	while(!m_isChangedPartner.empty() && count < maxCount)
	{
		UINT64 id = m_isChangedPartner.front();
		m_isChangedPartner.pop();
		count++;

		auto it = m_mapPartnerChat.find(id);
		if(it == m_mapPartnerChat.end())
		{
			LogError("parterid: %llu, can't find in PartnerChat", id);
			continue;
		}

		oLoadPtc.m_Data.set_storekey(id);

		KKSG::ChatInfoList oInfoList;
		auto itBegin = it->second.begin();
		auto itEnd = it->second.end();
		for(auto j = itBegin; j != itEnd; j++)
		{	
			KKSG::ChatInfo* chatinfo = oInfoList.add_chatinfolist();
			chatinfo->CopyFrom(*j);
		}

		oInfoList.SerializeToString(&tmpString);
		oLoadPtc.m_Data.set_chatinfolist(tmpString);

		DBLink::Instance()->Send(DBLink::Instance()->GetConnID(), oLoadPtc);
	}

	return true;
}

bool CChatMgr::SavePrivateChatToDB(UINT32 maxCount)
{
	if(m_isChangedRole.empty()) return false;

	PtcM2D_MDUpLoadChatNtf oLoadPtc;
	oLoadPtc.m_Data.set_channel(KKSG::PrivateChannel);
	
	UINT32 count = 0;
	std::string tmpString;
	while(!m_isChangedRole.empty() && count < maxCount)
	{
		UINT64 roleid = m_isChangedRole.front();
		m_isChangedRole.pop();
		count++;

		if(m_mapPrivateChat.find(roleid) == m_mapPrivateChat.end())
		{
			LogError("roleid:%llu, can't find in PrivateChat", roleid);
			assert(false);
			continue;
		}

		oLoadPtc.m_Data.set_storekey(roleid);
		
		KKSG::ChatInfoList oInfoList;
		auto itBegin = m_mapPrivateChat[roleid].begin();
		auto itEnd = m_mapPrivateChat[roleid].end();
		for(auto j = itBegin; j != itEnd; j++)
		{	
			KKSG::ChatInfo* chatinfo = oInfoList.add_chatinfolist();
			chatinfo->CopyFrom(*j);
		}

		oInfoList.SerializeToString(&tmpString);
		oLoadPtc.m_Data.set_chatinfolist(tmpString);

		DBLink::Instance()->Send(DBLink::Instance()->GetConnID(), oLoadPtc);
	}

	return true;
}

bool CChatMgr::SavePrivateListToDB(UINT32 maxCount)
{
	if(m_isChangedPrivateList.empty()) return false;
	
	PtcM2D_UpdatePrivateListNtf oUpdateListPtc;

	UINT32 count = 0;
	std::string tmpString;
	while(!m_isChangedPrivateList.empty() && count < maxCount)
	{
		UINT64 roleid = m_isChangedPrivateList.front();
		m_isChangedPrivateList.pop();
		count++;

		if(m_mapPrivateList.find(roleid) == m_mapPrivateList.end())
		{
			LogError("roleid:%llu, can't find in PrivateList", roleid);
			assert(false);
			continue;
		}
	
		oUpdateListPtc.m_Data.set_roleid(roleid);

		KKSG::PrivateChatList oChatList;
		auto itBegin = m_mapPrivateList[roleid].begin();
		auto itEnd = m_mapPrivateList[roleid].end();
		for (auto j = itBegin; j != itEnd; j++)
		{
			const PrivateList& list = *j;
			ChatSource* source = oChatList.add_rolelist();
			source->set_roleid(list.roleid);
			oChatList.add_hasofflinechat(list.hasOfflineChat);
			oChatList.add_lastchattime(list.lastChatTime);
		}

		oChatList.SerializeToString(&tmpString);
		oUpdateListPtc.m_Data.set_privatechatlist(tmpString);

		DBLink::Instance()->SendTo(oUpdateListPtc);
	}
	return true;
}

void CChatMgr::DeleteOneRoleAllMsg(UINT64 roleid)
{
	//删除世界聊天中的
	auto worldIter = m_worldChat.begin();
	while(worldIter != m_worldChat.end())
	{
		const KKSG::ChatInfo& chatinfo = *worldIter;
		if(roleid == chatinfo.source().roleid())
		{
			m_worldChat.erase(worldIter++);
		}
		else worldIter++;
	}
	m_isChangedWorld = true;

	//删除公会聊天中的
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(roleid);
	if(NULL != guild)
	{
		if(m_mapGuildChat.find(guild->GetID()) != m_mapGuildChat.end())
		{
			auto guildIter = m_mapGuildChat[guild->GetID()].begin();
			auto guildIterEnd = m_mapGuildChat[guild->GetID()].end();
			while(guildIter != guildIterEnd)
			{
				const KKSG::ChatInfo& chatinfo = *guildIter;
				if(roleid == chatinfo.source().roleid())
				{
					m_mapGuildChat[guild->GetID()].erase(guildIter++);
				}
				else guildIter++;
			}

			m_isChangedGuild.push(guild->GetID());
		}	
	}

    //删除群聊聊天中的
    GroupChatRoleInfo* pRoleInfo = GroupChatMgr::Instance()->GetRoleInfo(roleid);
    if(NULL != pRoleInfo)
    {
        std::set<UINT64>& groupidlist = GroupChatMgr::Instance()->GetRoleGroupList(*pRoleInfo);
        for(auto itor = groupidlist.begin(); itor != groupidlist.end(); ++itor)
        {
            auto itFind = m_mapGroupChat.find(*itor);
            if(itFind == m_mapGroupChat.end()) continue;
            for(auto itx = itFind->second.begin(); itx != itFind->second.end();)
            {
                const KKSG::ChatInfo& chatinfo = *itx;
                if(roleid == chatinfo.source().roleid())
                {
                    itFind->second.erase(itx++);
                }
                else itx++;
            }
            m_isChangedGroup.push(*itor);
        }
    }

	//删除战友频道聊天
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(roleid);
	if (NULL != partner)
	{
		auto it = m_mapPartnerChat.find(partner->GetID());
		if (it != m_mapPartnerChat.end())
		{
			for (auto i = it->second.begin(); i != it->second.end();)
			{
				const KKSG::ChatInfo& chatinfo = *i;
				if(roleid == chatinfo.source().roleid())
				{
					it->second.erase(i++);
				}
				else
				{
					i++;
				}
			}
		}
	}

	//删除私聊
	std::set<UINT64> otherid;
	auto IterBegin = m_mapPrivateChat[roleid].begin();
	auto IterEnd = m_mapPrivateChat[roleid].end();
	for(auto i = IterBegin; i != IterEnd; i++)
	{
		const KKSG::ChatInfo& chatinfo = *i;
		if(roleid == chatinfo.source().roleid())
		{
			otherid.insert(chatinfo.destlist(0).roleid());
		}
	}
	otherid.insert(roleid);
	for(auto i = otherid.begin(); i != otherid.end(); i++)
	{
		UINT64 other = *i;
		if(m_mapPrivateChat.find(other) == m_mapPrivateChat.end())
		{
			continue;
		}
		auto privateIter = m_mapPrivateChat[other].begin();
		auto privateIterEnd = m_mapPrivateChat[other].end();
		while(privateIter != privateIterEnd)
		{
			const KKSG::ChatInfo& chatinfo = *privateIter;
			if(roleid == chatinfo.source().roleid())
			{
				m_mapPrivateChat[other].erase(privateIter++);
			}
			else privateIter++;
		}

		m_isChangedRole.push(other);
	}
	
	PtcM2C_IdipClearChatNtf clearChatNtf;
	clearChatNtf.m_Data.set_roleid(roleid);
	//send to all online role to delete chat from this man
	for(auto i = CRoleManager::Instance()->Begin(); i != CRoleManager::Instance()->End(); i++)
	{
		CRole* role = *i;
		role->Send(clearChatNtf);
	}
}

void CChatMgr::RemovePartnerChat(UINT64 id)
{
	auto it = m_mapPartnerChat.find(id);
	if (it == m_mapPartnerChat.end())
	{
		return;
	}

	PtcM2D_MDDelChatNtf ntf;
	ntf.m_Data.set_channel(KKSG::PartnerChannel);
	ntf.m_Data.set_storekey(id);
	DBLink::Instance()->SendTo(ntf);

	m_mapPartnerChat.erase(it);
}
