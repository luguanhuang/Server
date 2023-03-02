#include "pch.h"
#include <time.h>
#include "platformfriendmgr.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "table/globalconfig.h"
#include "mail/mailmgr.h"
#include "platfriend/rpcm2s_m2sloginandoutsync.h"
#include "role/role.h"
#include "network/cslink.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "db/mysqlmgr.h"
#include "platfriend/ptcm2d_saveplatfriendinfo.h"
#include "network/dblink.h"
#include "platfriend/ptcm2s_m2suserstatussync.h"
#include "loghelper/tlogr.h"
#include "util/gametime.h"
#include "platfriend/rpcm2s_m2sreqplatuserinfo.h"

INSTANCE_SINGLETON(CPlatformFriendMgr)


CPlatformFriendMgr::CPlatformFriendMgr()
{
	//load from db

}

CPlatformFriendMgr::~CPlatformFriendMgr()
{

}

bool CPlatformFriendMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 70 * 1000, -1, __FILENAME__, __LINE__);

	LoadFromDB();

	m_cachedDetailPlatInfo.Init(GetGlobalConfig().GetInt("MaxPlatFriendDetailCacheSize", 10000));

	return true;
}

void CPlatformFriendMgr::Uninit()
{

}

void CPlatformFriendMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	int now = time(0);
	for (auto iter = m_reqCSList.begin(); iter != m_reqCSList.end();)
	{
		if (now - iter->second > 7)
		{
			m_reqCSList.erase(iter++);
		}
		else
			++iter;
	}

	Save2DB();
}

bool CPlatformFriendMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select * from platformfriendinfo";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table platformfriendinfo failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		PlatFriendList list;
		std::string openid;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			openid.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			std::string strData;
			strData.assign(pVal, dwLen);
			if(!list.LoadFromDb(strData))
			{
				return false;
			}
		}
		else
		{
			LogError("Read data column failed");
			return false;
		}

		// m_platFriendInfo.insert(std::make_pair<std::string, PlatFriendList>(openid, list));modify by lgh
		m_platFriendInfo[openid]=list;
	}
	poRes->Release();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all platformfriendinfo data,size %u from db use time: %llu ms",m_platFriendInfo.size(), qwTimeUsed);

	return true;
}

#define ONCE_COUNT 10
void CPlatformFriendMgr::Save2DB()
{
	PtcM2D_SavePlatFriendInfo ptc;
	int count = 1;
	for (auto i = m_changedlist.begin(); i != m_changedlist.end(); ++i)
	{
		auto j = m_platFriendInfo.find(*i);
		if (j == m_platFriendInfo.end())
		{
			continue;
		}
		KKSG::MSSavePlatFriend2DBData* platFriendsInfo = ptc.m_Data.add_platfriendinfo();
		platFriendsInfo->set_openid(*i);
		KKSG::PlatFriendsInfo2DB info;
		j->second.Save2Db(&info);
		platFriendsInfo->mutable_info()->assign(info.SerializeAsString());

		if (0 == count % ONCE_COUNT)
		{
			DBLink::Instance()->SendTo(ptc);
			ptc.m_Data.Clear();
		}
		++count;
	}
	DBLink::Instance()->SendTo(ptc);
	m_changedlist.clear();

	SSInfo<<"save platfriendinfo to db, count:"<<count - 1<<END;
}

PlatFriendList* CPlatformFriendMgr::GetPlatFriendList(CRole* role)
{
	const std::string& openid = role->GetAccount();
	auto find = m_platFriendInfo.find(openid);
	if (find == m_platFriendInfo.end())
	{
		PlatFriendList tempList;
		tempList.InitSelfInfo(role);
		m_platFriendInfo[openid] = tempList;
		AddChangedInfo(openid);
		return &m_platFriendInfo[openid];
	}

	return &find->second;
}

PlatFriendList* CPlatformFriendMgr::GetPlatFriendListByOpenID(const std::string& openid)
{
	auto find = m_platFriendInfo.find(openid);
	if (find == m_platFriendInfo.end())
	{
		return NULL;
	}

	return &find->second;
}

const PlatUserInfo* CPlatformFriendMgr::GetPlatSelfInfo(const std::string& openid)
{
	auto find = m_platFriendInfo.find(openid);
	if (find == m_platFriendInfo.end())
	{
		return NULL;
	}

	return &find->second.GetSelfInfo();
}

template<class T>
struct Compare
{
	bool operator()(const PlatUserInfo* a, const PlatUserInfo* b) const
	{
		if (strcmp(a->openid.c_str(), b->openid.c_str()) == 0)
		{
			return false;
		}
		if (!a->GetDetail() || !b->GetDetail())
		{
			return false;
		}
		if (a->GetDetail()->maxAbility > b->GetDetail()->maxAbility)
		{
			return true;
		}
		else if (a->GetDetail()->maxAbility == b->GetDetail()->maxAbility)
		{
			if (a->GetDetail()->updatetime < b->GetDetail()->updatetime)
			{
				return true;
			}
			else if (a->GetDetail()->updatetime == b->GetDetail()->updatetime)
			{
				if (a->platformID < b->platformID)
				{
					return true;
				}
				else if (a->platformID == b->platformID)
				{
					if (a->roleID < b->roleID)
					{
						return true;
					}
				}
			}
		}

		return false;
	}
};

UINT32 CPlatformFriendMgr::GetPlatFriendRankList2Client(CRole* role, KKSG::ReqPlatFriendRankListRes& roRes)
{
	const std::string& openid = role->GetAccount();

	if (m_reqCSList.find(openid) != m_reqCSList.end())
	{
		return KKSG::ERR_SUCCESS;
	}

	PlatFriendList* list = GetPlatFriendList(role);

	return list->GetPlatFriendRankList2Client(role, roRes);
}

void CPlatformFriendMgr::SyncPlatFriendListFromClient(CRole* role, KKSG::SyncPlatFriend2MSData& data)
{
	bool change = false;
	const std::string& openid = role->GetAccount();
	auto list = GetPlatFriendList(role);
	if (list->CheckSyncTimeFromClient(time(0)))
	{
		change = list->SyncPlatFriendListFromClient(data);
		if (change)
		{
			QueryDetailInfoFromCS(role);
			AddChangedInfo(openid);
		}
	}

	bool changeSelf = list->SyncPlatSelfInfoFromClient(role, data);
	if (!change && changeSelf)
	{
		AddChangedInfo(openid);
	}

	LogDebug("syncfromclient:%s, platfriendssize:%u, selfinfo:%u", openid.c_str(), data.friendinfo_size(), data.has_selfinfo() ? 1 : 0);
}

void CPlatformFriendMgr::SyncPlatFriendListFromCSAndFillRank(CRole* role, KKSG::M2SReqPlatUserInfoRes& data, KKSG::ReqPlatFriendRankListRes &roRes)
{
	const std::string& openid = role->GetAccount();
	PlatFriendList* list = GetPlatFriendList(role);

	list->SyncPlatFriendListFromCS(data);

	AddChangedInfo(openid);

	list->GetPlatFriendRankList2Client(role, roRes);
}

void CPlatformFriendMgr::AddChangedInfo(const std::string& openid)
{
	m_changedlist.insert(openid);
}

void CPlatformFriendMgr::AddClientReq2CS(const std::string& openid)
{
	// m_reqCSList.insert(std::make_pair<std::string, UINT32>(const_cast<std::string&>(openid), time(0)));lgh
	 m_reqCSList[openid]=time(0);
}

void CPlatformFriendMgr::RemoveClientReq2CS(const std::string& openid)
{
	m_reqCSList.erase(openid);
}

bool CPlatformFriendMgr::IsClientReq2CSFull()
{
	return m_reqCSList.size() >= REQCS_PLATFRIENDINFO_MAXSIZE;
}

UINT32 CPlatformFriendMgr::GiveGift2PlatFriend(CRole* role, const std::string& destOpenID)
{
	const std::string& openid = role->GetAccount();
	PlatFriendList* list = GetPlatFriendList(role);

	return list->GiveGift2PlatFriend(role, destOpenID);
}

void CPlatformFriendMgr::ReceivePlatFriendGift(const KKSG::S2MSendGift2PlatFriendArg& roArg)
{
	UINT64 roleID = roArg.destuser().platuserdetailinfo().roleid();

	auto destUser = GetPlatFriendListByOpenID(roArg.destuser().openid().c_str());
	if (!destUser)
	{
		LogError("%s, %llu,receive platfriend gift from %s error.",roArg.destuser().openid().c_str(), roleID, roArg.sourceopenid().c_str());
		return;
	}

	std::string nickName("");
	auto sourceInfo = destUser->GetPlatFriendUserInfo(roArg.sourceopenid());
	if (!sourceInfo)
	{
		LogError("%s, %llu,receive platfriend gift from %s error, not friend.",roArg.destuser().openid().c_str(), roleID, roArg.sourceopenid().c_str());
	}
	else
	{
		nickName = sourceInfo->nickname;
	}

	std::vector<ItemDesc> vecItem;
	for (int i = 0; i < roArg.item_size(); ++i)
	{
		ItemDesc temp;
		temp.itemID = roArg.item(i).itemid();
		temp.itemCount = roArg.item(i).itemcount();
		vecItem.push_back(temp);
	}
	std::string content = GetGlobalConfig().GivePlatFriendGiftMailContent;
	if (!vecItem.empty())
	{
		MailData mail( GetGlobalConfig().GivePlatFriendGiftMailTitle , nickName + content, vecItem, MAIL_EXPIRE_TIME);
		mail.SetReason(ItemFlow_PlatFriendGift);
		CMailMgr::Instance()->SendMail(roleID, mail);
	}

	LogDebug("%s, %llu,receive platfriend gift",roArg.destuser().openid().c_str(), roleID);
}

bool CPlatformFriendMgr::IsPlatFriend(const std::string& srcOpenID, const std::string destOpenID)
{
	PlatFriendList* list = GetPlatFriendListByOpenID(srcOpenID);
	if (!list)
	{
		return false;
	}

	if(list->GetPlatFriendUserInfo(destOpenID))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CPlatformFriendMgr::DebugSyncPlatSelf(CRole* role)
{
	PlatFriendList* list = GetPlatFriendList(role);

	list->SyncPlatSelfInfo2CS(role);
}

void CPlatformFriendMgr::QueryDetailInfoFromCS(CRole* role)
{
	if (m_reqCSList.find(role->GetAccount()) != m_reqCSList.end())
	{
		return;
	}
	//send rpc
	PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(role);
	std::vector<std::string> platFriends;
	list->GetPlatFriends(platFriends);
	RpcM2S_M2SReqPlatUserInfo* rpc = RpcM2S_M2SReqPlatUserInfo::CreateRpc();
	rpc->m_oArg.set_sourceopenid(role->GetAccount());
	for (auto iter = platFriends.begin(); iter != platFriends.end(); ++iter)
	{
		rpc->m_oArg.add_openid(*iter);
	}
	CSLink::Instance()->SendTo(*rpc);
	CPlatformFriendMgr::Instance()->AddClientReq2CS(role->GetAccount());

	LogDebug("%llu, %s request platfriendlist to cs", role->GetID(), role->GetAccount().c_str());
}

void CPlatformFriendMgr::DelRolePlatDetailInfo(std::string& account)
{
	PlatFriendList* list = GetPlatFriendListByOpenID(account);
	if (!list)
	{
		return;
	}

	list->DelDetailInfoFromCache();
}

void CPlatformFriendMgr::_SyncUserStatus2CS(CRole* role, bool isLogin)
{
	if (!role)
	{
		return;
	}
	PtcM2S_M2SUserStatusSync ntf; 
	ntf.m_Data.set_openid(role->GetAccount());
	ntf.m_Data.set_serverid(MSConfig::Instance()->GetServerID());
	ntf.m_Data.set_isonline(isLogin);
	ntf.m_Data.set_roleid(role->GetID());
	CSLink::Instance()->SendToAll(ntf);
}

void CPlatformFriendMgr::LoginAndOutSyncFromCS(CRole* role, const KKSG::M2SLoginAndoutSyncRes& roRes)
{
	const std::string& openid = role->GetAccount();
	if (roRes.error() == KKSG::ERR_SUCCESS)
	{
		//if (roRes.has_maxinfo() && strcmp(openid.c_str(), roRes.maxinfo().openid().c_str()) == 0)
		{
			PlatFriendList* list = GetPlatFriendList(role);
			list->OnLoginAndOutSyncFromCS(roRes);
			AddChangedInfo(openid);
		}
	}
}

void CPlatformFriendMgr::OnLoginReceiveDetailPlatInfoFromCs(const std::string& openid)
{
	CPlatFormInfoAddFunctor oAddFunc;
	CPlatFormInfoDelFunctor oDelFunc;
	m_cachedDetailPlatInfo.Put(openid, oAddFunc, oDelFunc);
}

void CPlatformFriendMgr::OnLoginOrOut(CRole* role, bool isLogin)
{
	PlatFriendList* list = GetPlatFriendList(role);
	if (isLogin)
	{
		list->OnLogin(role);
	}
	else
		list->OnLogout(role);

	_SyncUserStatus2CS(role, isLogin);
}

bool PlatUserInfo::operator<(const PlatUserInfo& b)
{
	//return Compare(*this, b);
	return false;
}

PlatUserInfo::PlatUserInfo(const PlatUserInfo& a)
{
	platformID = a.platformID;
	serverID = a.serverID;
	roleID = a.roleID;
	detail = NULL;
	if (a.detail)
	{
		detail = new SPlatUserDetailInfo();
		*detail = *a.detail;
	}
}

PlatUserInfo& PlatUserInfo::operator=(const PlatUserInfo& a)
{
	platformID = a.platformID;
	serverID = a.serverID;
	roleID = a.roleID;
	if (a.detail)
	{
		if (!detail)
		{
			detail = new SPlatUserDetailInfo();
		}
		*detail = *a.detail;
	}

	return *this;
}

PlatUserInfo::~PlatUserInfo()
{
	if (detail)
	{
		delete detail;
		detail = NULL;
	}
}

SPlatUserDetailInfo* PlatUserInfo::CreateDetail()
{
	if (!detail)
	{
		detail = new SPlatUserDetailInfo();
	}

	return detail;
}

void PlatUserInfo::RemoveDetail()
{
	if (detail)
	{
		delete detail;
	}

	detail = NULL;
}

void PlatUserInfo::InitFromRole(CRole* role)
{
	if (!role)
	{
		return;
	}

	openid = role->GetAccount();
	platformID = MSConfig::Instance()->GetZoneID();
	serverID = MSConfig::Instance()->GetServerID();
	roleID = role->GetID();
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
	if (summary)
	{
		CreateDetail()->maxAbility = summary->GetPowerPoint();
		CreateDetail()->viplevel = summary->GetVipLevel();
		CreateDetail()->level = summary->GetLevel();
		CreateDetail()->startType = summary->GetStartUpType();
		CreateDetail()->profession = summary->GetProfession();
	}
	CreateDetail()->isOnline = true;
	CreateDetail()->updatetime = time(0);
	CreateDetail()->sendGiftTime = 0;
	roleName = role->GetName();
}

bool PlatUserInfo::ConvertFromCSKKSGPlatUserAllInfo(const KKSG::PlatUserAllInfo& allInfo)
{
	openid = allInfo.openid();
	if (allInfo.has_platuserdetailinfo())
	{
		auto& ref = allInfo.platuserdetailinfo();
		serverID = ref.serverid();
		roleID = ref.roleid();
		CreateDetail()->maxAbility = ref.maxability();
		CreateDetail()->isOnline = ref.isonline();
		CreateDetail()->viplevel = ref.viplevel();
		CreateDetail()->level = ref.level();
		CreateDetail()->updatetime = ref.updatetime();
		CreateDetail()->startType = ref.starttype();
		CreateDetail()->profession = ref.profession();
		platformID = ref.platformid();
	}

	return true;
}

bool PlatUserInfo::Convert2CSKKSGPlatUserAllInfo(KKSG::PlatUserAllInfo& allInfo)
{
	allInfo.set_openid(openid);
	KKSG::PlatUserDetailInfo* info = allInfo.mutable_platuserdetailinfo();
	info->set_platformid(platformID);
	info->set_serverid(serverID);
	info->set_roleid(roleID);
	if (detail)
	{
		info->set_maxability(detail->maxAbility);
		info->set_isonline(detail->isOnline);
		info->set_viplevel(detail->viplevel);
		info->set_level(detail->level);
		info->set_updatetime(detail->updatetime);
		info->set_sendgifttime(detail->sendGiftTime);
		info->set_starttype(detail->startType);
		info->set_profession(detail->profession);
	}
	info->set_rolename(roleName);

	return true;
}

bool PlatUserInfo::ConvertFromClientKKSGPlatFriend(const KKSG::PlatFriend& platFriendInfo)
{
	bool changed = false;
	openid = platFriendInfo.openid();
	if (strcmp(nickname.c_str(), platFriendInfo.nickname().c_str()) != 0)
	{
		nickname = platFriendInfo.nickname();
		changed = true;
	}
	if (strcmp(bigpic.c_str(), platFriendInfo.bigpic().c_str()) != 0)
	{
		bigpic = platFriendInfo.bigpic();
		changed = true;
	}
	//if (strcmp(midpic.c_str(), platFriendInfo.midpic().c_str()) != 0)
	//{
	//	midpic = platFriendInfo.midpic();
	//	changed = true;
	//}
	//if (strcmp(smallpic.c_str(), platFriendInfo.smallpic().c_str()) != 0)
	//{
	//	smallpic = platFriendInfo.smallpic();
	//	changed = true;
	//}

	return changed;
}

bool PlatUserInfo::Convert2KKSGClient(KKSG::PlatFriendRankInfo2Client& info)
{
	KKSG::PlatFriend* platFriend = info.mutable_platfriendbaseinfo();
	platFriend->set_openid(openid);
	platFriend->set_bigpic(bigpic);
	platFriend->set_midpic(bigpic);
	platFriend->set_smallpic(bigpic);
	platFriend->set_nickname(nickname);

	if (detail)
	{
		info.set_hasgivegift(XCommon::IsDaySame(detail->sendGiftTime, time(0)) ? true : false);
		info.set_maxability(detail->maxAbility);
		info.set_viplevel(detail->viplevel);
		info.set_level(detail->level);
		info.set_isonline(detail->isOnline);
		info.set_starttype(detail->startType);
		info.set_profession(detail->profession);
	}

	return true;
}

bool PlatUserInfo::Load(const KKSG::PlatUserAllInfo2DB& allInfo)
{
	openid = allInfo.openid();
	if (allInfo.has_nickname())
	{
		nickname = allInfo.nickname();
	}

	return true;
}

void PlatUserInfo::Save(KKSG::PlatUserAllInfo2DB& allInfo)
{
	allInfo.set_openid(openid);
	allInfo.set_nickname(nickname);
}

bool PlatFriendList::LoadFromDb(const std::string& info)
{
	if (info.empty())
	{
		return false;
	}
	KKSG::PlatFriendsInfo2DB tempInfo;
	if(!tempInfo.ParseFromString(info))
		return false;
	for (int i = 0; i < tempInfo.platfriendsinfo_size(); ++i)
	{
		PlatUserInfo friendInfo;
		friendInfo.Load(tempInfo.platfriendsinfo(i));

		// m_platFriendList.insert(std::make_pair<std::string, PlatUserInfo>(friendInfo.openid, friendInfo));lgh
		m_platFriendList[friendInfo.openid]=friendInfo;
	}

	if (tempInfo.has_selfinfo())
	{
		//m_selfInfo.Load(tempInfo.selfinfo());
		m_selfInfo.openid = tempInfo.selfinfo().openid();
		m_selfInfo.bigpic = tempInfo.selfinfo().pic();
		m_selfInfo.nickname = tempInfo.selfinfo().nickname();
	}

	m_clientReqTime = tempInfo.clientreqranktime();
	m_updateTimeFromClient = tempInfo.updateplatfriendsfromclient();
	m_syncSelfInfo2CSTime = tempInfo.syncselfinfo2cstime();

	return true;
}

void PlatFriendList::Save2Db(KKSG::PlatFriendsInfo2DB* info)
{
	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end(); ++iter)
	{
		iter->second.Save(*info->add_platfriendsinfo());
	}

	auto selfInfo = info->mutable_selfinfo();
	selfInfo->set_openid(m_selfInfo.openid);
	selfInfo->set_pic(m_selfInfo.bigpic);
	selfInfo->set_nickname(m_selfInfo.nickname);

	info->set_clientreqranktime(m_clientReqTime);
	info->set_updateplatfriendsfromclient(m_updateTimeFromClient);
	info->set_syncselfinfo2cstime(m_syncSelfInfo2CSTime);
}

bool PlatFriendList::SyncPlatSelfInfoFromClient(CRole* role, const KKSG::SyncPlatFriend2MSData& data)
{
	if (data.has_selfinfo())
	{
		bool change = m_selfInfo.ConvertFromClientKKSGPlatFriend(data.selfinfo());
		SetOpenID(role->GetAccount());
		if (m_needSyncSelfInfo)
		{
			if(SyncPlatSelfInfo2CS(role))
				m_needSyncSelfInfo = false;
		}
		LogDebug("self info from client,openid:%s,bigpic:%s,midpic:%s,smallpic:%s,nickname:%s.", \
			data.selfinfo().openid().c_str(), data.selfinfo().bigpic().c_str(), data.selfinfo().midpic().c_str(),\
			data.selfinfo().smallpic().c_str(), data.selfinfo().nickname().c_str());
		return change;
	}

	return false;
}

bool PlatFriendList::SyncPlatSelfInfo2CS(CRole* role)
{
	int now = time(0);
	if (now - m_syncSelfInfo2CSTime < SYNC_PLATSELFINFO_TOCS_TIME)
	{
		return false;
	}
	bool tempOnline = m_selfInfo.CreateDetail()->isOnline;
	InitSelfInfo(role);
	m_selfInfo.CreateDetail()->isOnline = tempOnline;
	if (!m_selfInfo.CreateDetail()->maxAbility)
	{
		LogWarn("%s,%llu,%s, sync selfinfo to cs, ability 0.", role->GetAccount().c_str(), role->GetID(), m_selfInfo.CreateDetail()->isOnline ? "login" : "offline");
	}
	//m_selfInfo = m_selfInfo;
	RpcM2S_M2SLoginAndoutSync* rpc = RpcM2S_M2SLoginAndoutSync::CreateRpc();
	rpc->m_sessionID = role->GetSessionID();
	KKSG::PlatUserAllInfo* info = rpc->m_oArg.mutable_selfinfo();
	m_selfInfo.Convert2CSKKSGPlatUserAllInfo(*info);
	CSLink::Instance()->SendTo(*rpc);

	LogDebug("%s,%llu,%s, sync selfinfo to cs", role->GetAccount().c_str(), role->GetID(), m_selfInfo.CreateDetail()->isOnline ? "login" : "offline");

	m_syncSelfInfo2CSTime = now;

	return true;
}

bool PlatFriendList::SyncPlatFriendListFromClient(const KKSG::SyncPlatFriend2MSData& data)
{
	int now = time(0);
	if (data.friendinfo_size() == 0)
	{
		LogInfo("openid:%s , platfriendlist empty.", m_selfInfo.openid.c_str());
		//return false;
	}
	std::unordered_set<std::string> tempSet;
	bool changed = false;
	for (int i = 0; i < data.friendinfo_size() && i < 200; ++i)
	{
		bool change = false;
		tempSet.insert(data.friendinfo(i).openid());
		if (strcmp(data.friendinfo(i).openid().c_str(), m_selfInfo.openid.c_str()) == 0)
		{
			continue;
		}
		auto find = m_platFriendList.find(data.friendinfo(i).openid());
		if (find == m_platFriendList.end())
		{
			PlatUserInfo temp;
			temp.ConvertFromClientKKSGPlatFriend(data.friendinfo(i));
			// m_platFriendList.insert(std::make_pair<std::string, PlatUserInfo>(temp.openid, temp));lgh
			m_platFriendList[temp.openid]=temp;
			change = true;
		}
		else
		{
			change = find->second.ConvertFromClientKKSGPlatFriend(data.friendinfo(i));
		}
		if (!changed && change)
		{
			changed = change;
		}
		LogDebug("sync friends icon info from client, openid:%s,nickname:%s,bigpic:%s,midpic:%s,smallpic:%s",\
			data.friendinfo(i).openid().c_str(), data.friendinfo(i).nickname().c_str(), data.friendinfo(i).bigpic().c_str(),\
			data.friendinfo(i).midpic().c_str(), data.friendinfo(i).smallpic().c_str());
	}

	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end();)
	{
		if (tempSet.find(iter->first) == tempSet.end())
		{
			m_platFriendList.erase(iter++);
			changed = true;
		}
		else
			++iter;
	}

	m_updateTimeFromClient = now;

	return changed;
}

bool PlatFriendList::SyncPlatFriendListFromCS(const KKSG::M2SReqPlatUserInfoRes& data)
{
	int now = time(0);
	m_clientReqTime = now;

	std::unordered_set<std::string> tempOpenIDs;
	for (int i = 0; i < data.userinfos_size(); ++i)
	{
		tempOpenIDs.insert(data.userinfos(i).openid());
		auto& ref = m_platFriendList[data.userinfos(i).openid()];
		ref.ConvertFromCSKKSGPlatUserAllInfo(data.userinfos(i));

		LogDebug("get friends from cs , %s, ability:%u, platformid:%u, serverid:%u", \
			ref.openid.c_str(), ref.CreateDetail()->maxAbility,
			ref.platformID, ref.serverID);
	}
	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end();)
	{
		if (tempOpenIDs.find(iter->first) == tempOpenIDs.end())
		{
			m_platFriendList.erase(iter++);
		}
		else
			++iter;
	}
	DoLog();
	return true;
}

bool PlatFriendList::IsFirstSync()
{
	if (!m_updateTimeFromClient)
	{
		return true;
	}
	return false;
}

bool PlatFriendList::CheckSyncTimeFromClient(int now)
{
	if (m_needSyncFriendInfo)
	{
		m_needSyncFriendInfo = false;
		return true;
	}
	if (!IsFirstSync())
	{
		if (now - m_updateTimeFromClient < SYNC_PLATFRIENDLIST_FROMCLIENT_TIME)
		{
			return false;
		}
	}

	return true;
}

UINT32 PlatFriendList::GetPlatFriendRankList2Client(CRole* role, KKSG::ReqPlatFriendRankListRes& roRes)
{
	std::set<PlatUserInfo*, Compare<PlatUserInfo*>> tempSet;
	bool needRefresh = false;
	bool isFirstReq = true;
	int now = time(0);

	if (m_selfInfo.CreateDetail()->maxAbility == 0)
	{
		InitSelfInfo(role);
		CPlatformFriendMgr::Instance()->AddChangedInfo(role->GetAccount());
		LogDebug("%s, %llu, self ability 0.", role->GetAccount().c_str(), role->GetID());
		if (m_selfInfo.CreateDetail()->maxAbility == 0)
		{
			LogDebug("%s, %llu, self ability still 0.", role->GetAccount().c_str(), role->GetID());
			return KKSG::ERR_SUCCESS;
		}
	}

	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end(); ++iter)
	{
		if (!iter->second.GetDetail())
		{
			return KKSG::ERR_FAILED;
		}
		if (iter->second.GetDetail()->maxAbility == 0)
		{
			needRefresh = true;
		}
		else
			isFirstReq = false;
		/*if (needRefresh)
		{
			break;
		}*/
		tempSet.insert(&iter->second);
	}

	if (!m_platFriendList.empty() && isFirstReq)
	{
		return KKSG::ERR_FAILED;
	}
	if (!m_platFriendList.empty() && (now - m_clientReqTime > REQCS_PLATFRIENDINFO_TIMEUNIT * REQCS_PLATFRIENDINFO_TIMEUNITCOUNT))
	{
		return KKSG::ERR_FAILED;
	}
	else
	{
		tempSet.insert(&m_selfInfo);
		int rank = 0;
		int selfRank = 0;
		for (auto iter = tempSet.begin(); iter != tempSet.end(); ++iter)
		{
			if ((*iter)->GetDetail()->maxAbility == 0)
			{
				m_platFriendList.erase((*iter)->openid);
				continue;
				//break;
			}
			++rank;
			if (rank > 200)
			{
				m_platFriendList.erase((*iter)->openid);
			}
			else
			{
				KKSG::PlatFriendRankInfo2Client* tempInfo = roRes.add_platfriends();
				(*iter)->Convert2KKSGClient(*tempInfo);
				tempInfo->set_rank(rank);
				if (strcmp(tempInfo->platfriendbaseinfo().openid().c_str(), m_selfInfo.openid.c_str()) == 0)
				{
					selfRank = rank;
				}
			}

		}

		KKSG::PlatFriendRankInfo2Client* selfTempInfo = roRes.mutable_selfinfo();
		m_selfInfo.Convert2KKSGClient(*selfTempInfo);
		selfTempInfo->set_rank(selfRank);
		
		LogInfo("%s, %llu, request platfriendlist local, size:%u",role->GetAccount().c_str(), role->GetID(), roRes.platfriends_size());
		return KKSG::ERR_SUCCESS;
	}
}

UINT32 PlatFriendList::GiveGift2PlatFriend(CRole* role, const std::string& destOpenID)
{
	auto find = m_platFriendList.find(destOpenID);
	if (find == m_platFriendList.end())
	{
		return KKSG::ERR_FAILED;
	}
	if (!find->second.GetDetail())
	{
		CPlatformFriendMgr::Instance()->QueryDetailInfoFromCS(role);
		return KKSG::ERR_FAILED;
	}

	if (!find->second.GetDetail()->maxAbility)
	{
		return KKSG::ERR_FAILED;
	}

	int now = time(0);
	if(XCommon::IsDaySame(find->second.GetDetail()->sendGiftTime, now))
	{
		return KKSG::ERR_FAILED;
	}

	if (m_todaySendGiftCount >= ONEDAY_SENDPLATFRIEND_MAXSIZE)
	{
		return KKSG::ERR_FAILED;
	}

	find->second.CreateDetail()->sendGiftTime = now;

	++m_todaySendGiftCount;

	CPlatformFriendMgr::Instance()->AddChangedInfo(m_selfInfo.openid);

	return KKSG::ERR_SUCCESS;
}

bool PlatFriendList::GetPlatFriendUserAllInfo(const std::string& openid, KKSG::PlatUserAllInfo& allInfo)
{
	auto find = m_platFriendList.find(openid);
	if (find == m_platFriendList.end())
	{
		return false;
	}

	return find->second.Convert2CSKKSGPlatUserAllInfo(allInfo);
}

void PlatFriendList::InitSelfInfo(CRole* role)
{
	m_selfInfo.InitFromRole(role);
}

void PlatFriendList::SetOpenID(const std::string& openid)
{
	m_selfInfo.openid = openid;
}

void PlatFriendList::OnLoginAndOutSyncFromCS(const KKSG::M2SLoginAndoutSyncRes& roRes)
{
	int now = time(0);
	//m_selfInfo.ConvertFromCSKKSGPlatUserAllInfo(roRes.maxinfo());
	
	m_todaySendGiftCount = roRes.sendgifttoday_size();
	LogInfo("account:%s, role:%llu, today send gift size:%u.", m_selfInfo.openid.c_str(), m_selfInfo.roleID, m_todaySendGiftCount);

	for (int i = 0; i < roRes.sendgifttoday_size(); ++i)
	{
		auto find = m_platFriendList.find(roRes.sendgifttoday(i).openid());
		if (find != m_platFriendList.end())
		{
			find->second.CreateDetail()->sendGiftTime = roRes.sendgifttoday(i).sendgifttime();
		}
	}
}

void PlatFriendList::OnLogin(CRole* role)
{
	m_selfInfo.CreateDetail()->isOnline = true;
}

void PlatFriendList::OnLogout(CRole* role)
{
	if (m_selfInfo.openid.empty())
	{
		LogError("user:%s, %llu,has no selfino", role->GetAccount().c_str(), role->GetID());
		return;
	}

	m_needSyncSelfInfo = true;
	m_needSyncFriendInfo = true;
	//if (m_selfInfo.platformID == MSConfig::Instance()->GetZoneID() && m_selfInfo.roleID == role->GetID())
	{
		m_selfInfo.CreateDetail()->isOnline = false;
	}
	SyncPlatSelfInfo2CS(role);
}

void PlatFriendList::DelDetailInfoFromCache()
{
	m_selfInfo.RemoveDetail();

	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end(); ++iter)
	{
		iter->second.RemoveDetail();
	}
}

void PlatFriendList::GetPlatFriends(std::vector<std::string>& friends)
{
	for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end(); ++iter)
	{
		friends.push_back(iter->first);
	}
}

const PlatUserInfo& PlatFriendList::GetSelfInfo()
{
	return m_selfInfo;
}

const PlatUserInfo* PlatFriendList::GetPlatFriendUserInfo(const std::string& openid)
{
	auto find = m_platFriendList.find(openid);
	if (find == m_platFriendList.end())
	{
		return NULL;
	}
	
	return &find->second;
}

void PlatFriendList::DebugSetClientReqTime(int time)
{
	m_clientReqTime = time;
	CPlatformFriendMgr::Instance()->AddChangedInfo(m_selfInfo.openid);
}

void PlatFriendList::DoLog()
{
	if ((m_lastlogtime + 3600) < GameTime::GetTime())
	{
		TPlatformFriendFlow oLog(TLogMgr::Instance()->GetTagString());
		for (auto iter = m_platFriendList.begin(); iter != m_platFriendList.end(); iter ++)
		{
			oLog.m_uRoleID = m_selfInfo.roleID;
			oLog.m_strOpenId = m_selfInfo.openid;
			oLog.m_RoleId2 = iter->second.roleID;
			oLog.m_strOpenId2 = iter->second.openid;
			oLog.m_strNickName = m_selfInfo.nickname;
			oLog.m_strBigPic = m_selfInfo.bigpic;
			oLog.m_strMidPic = m_selfInfo.bigpic;
			oLog.m_strSmallPic = m_selfInfo.bigpic;
			oLog.m_strNickName2 = iter->second.nickname;
			oLog.m_strBigPic2 = iter->second.bigpic;
			oLog.m_strMidPic2 = iter->second.bigpic;
			oLog.m_strSmallPic2 = iter->second.bigpic;
			oLog.Do();
		}
		m_lastlogtime = GameTime::GetTime();
	}
}
