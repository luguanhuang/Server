#include "pch.h"
#include "friend.h"
#include "friendmgr.h"
#include "util.h"
#include "timeutil.h"
#include "network/dblink.h"
#include "db/mysqlmgr.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "friend/ptcm2d_updatefriendondb.h"
#include "notice/noticemgr.h"
#include "role/rolemanager.h"
#include "common/ptcm2c_mseventnotify.h"
#include "table/StringTableMgr.h"

INSTANCE_SINGLETON(FriendMgr);

#define FRIEND_TIMEUPDATE 60*1000

FriendMgr::FriendMgr()
{

}

FriendMgr::~FriendMgr()
{
	for (auto i = m_id2friend.begin(); i != m_id2friend.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
}

bool FriendMgr::Init()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream ss;
	ss<< "select _id,name,friend from friend";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table friend failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	std::map<UINT64, std::vector<UINT64>> m_roleid2delete;
	std::vector<UINT64> m_roleids;

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// roleid
		UINT64 roleid;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			roleid = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		// name
		std::string name;
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			name.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'name' column failed");
			return false;
		}

		// friend
		std::string strData;
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'friend' column failed");
			return false;
		}

		KKSG::Friend2Db info;
		if(!info.ParseFromString(strData))
		{
			LogError("Parse friend failed, name:%s", name.c_str());
			return false;
		}
		if (IsExpire(roleid))
		{
			// record to delete
			m_roleids.push_back(roleid);
			for (int i = 0; i < info.roleid_size(); ++i)
			{
				m_roleid2delete[roleid].push_back(info.roleid(i));
			}
		}
		else
		{
			Friend* friend_ = new Friend();
			friend_->LoadFromDb(info);
			AddFriendInfo(roleid, friend_);
		}
	}
	poRes->Release();

	SSInfo<<"friend delete count:"<<m_roleid2delete.size()<<END;
	// delete
	for (auto i = m_roleids.begin(); i != m_roleids.end(); ++i)
	{
		UINT64 roleid = *i;
		auto j = m_roleid2delete.find(roleid);
		if (j != m_roleid2delete.end())
		{
			for (auto k = j->second.begin(); k != j->second.end(); ++k)
			{
				Friend* otherinfo = GetFriendInfo(*k);
				if (NULL != otherinfo)
				{
					otherinfo->RemoveFriend(roleid);
				}
			}
			j->second.clear();
		}

		// sql delete 
		ss.str("");
		ss << "delete from friend where _id=" << roleid;
		poMySqlConn->Query(ss.str().c_str(), ss.str().size(), NULL);
		LogInfo("Delete expired friend [%llu] from db", roleid);
	}
	m_roleids.clear();
	m_roleid2delete.clear();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all friend data from db use time: %llu ms", qwTimeUsed);

	m_timehandle = CTimerMgr::Instance()->SetTimer(this ,0, FRIEND_TIMEUPDATE, -1, __FILE__, __LINE__);

	return true;
}

void FriendMgr::Uninit()
{
	if (INVALID_HTIMER != m_timehandle)
	{
		CTimerMgr::Instance()->KillTimer(m_timehandle);
		m_timehandle = INVALID_HTIMER;
	}
}

Friend* FriendMgr::GetFriendInfo(UINT64 uid)
{
	auto i = m_id2friend.find(uid);
	if (i == m_id2friend.end())
	{
		Friend* friend_ = new Friend();
		m_id2friend[uid] = friend_;
		return friend_;
	}
	return i->second;
}

void FriendMgr::AddFriendInfo(UINT64 uid, Friend* friend_)
{
	auto i = m_id2friend.find(uid);
	if (i == m_id2friend.end())
	{
		m_id2friend[uid] = friend_;
	}
	else
	{
		delete friend_;
		SSWarn<<"friend exist uid:"<<uid<<END;
	}
}

void FriendMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	int count = 0;
	int max = GetGlobalConfig().FriendUpdateCount;
	for (auto i = m_id2friend.begin(); i != m_id2friend.end(); ++i)
	{
		if (i->second->IsChanged() || i->second->m_record.IsChanged())
		{
			if (++count >= max)
			{
				break;
			}
			// save to db...
			PtcM2D_UpdateFriendOnDB ntf;
			UINT64 roleid = i->first;
			ntf.m_Data.set_roleid(roleid);
			ntf.m_Data.set_name(CRoleSummaryMgr::Instance()->GetRoleName(roleid));
			KKSG::Friend2Db* info = ntf.m_Data.mutable_info();
			i->second->SaveToDb(*info);
			DBLink::Instance()->SendTo(ntf);
		}
	}
}

void FriendMgr::SendNoNotifyAddFriend(UINT64 uid)
{
	if(!CRoleManager::Instance()->IsOnline(uid))
	{
		return;
	}
	if(m_id2friend.find(uid) == m_id2friend.end())
	{
		return;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(uid);
	if(NULL == pRole)
	{
		LogError("CRole is null, roleid = %lld", uid);
		return;
	}
	Friend* pFriend = m_id2friend[uid];
	for (auto i = pFriend->m_noNotifyAddFriend.begin(); i != pFriend->m_noNotifyAddFriend.end(); i++)
	{
		CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(*i);
		if(NULL == role)
		{
			LogError("CRoleSummary is null, roleid = %lld", *i);
			continue;
		}
		NoticeMgr::Instance()->SendAddFriendNotice(uid, role);
		PtcM2C_MSEventNotify oEventNotify;
		std::string notifyData = role->GetName() + StringTableMgr::Instance()->GetText("NewFriendNotify");
		oEventNotify.m_Data.set_notify(notifyData);
		pRole->Send(oEventNotify);
	}

	pFriend->ClearNoNotifyAddFriend();
}

bool FriendMgr::IsExpire(UINT64 uid)
{
	///> ¿ª¹Ø
	if (!MSConfig::Instance()->IsDataClear(DataClear_Friend))
	{
		return false;
	}
	return CRoleSummaryMgr::Instance()->IsExpire(uid, GetGlobalConfig().FriendExpireTime, GetGlobalConfig().FriendExpireLevel);
}
