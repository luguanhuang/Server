#include "pch.h"
#include "time.h"
#include "frienddef.h"
#include "friendop.h"
#include "friendconfig.h"
#include "friendmgr.h"
#include "role/rolemanager.h"
#include "pb/project.pb.h"
#include "network/gslink.h"
#include "partner/partnermgr.h"
#include "role/rolesummarymgr.h"
#include "friend/ptcm2c_blacklistntfnew.h"
#include "loghelper/tlogr.h"
#include "define/tlogenum.h"
#include "idutil.h"
#include "loghelper/reportdatamgr.h"
#include "reportdata/reportdata_def.h"
#include "goalawards/goalawardsmgr.h"

extern UINT32 ErrorCodeTrans(UINT32 errorcode, bool isself = true);

void FriendNotify::Pack(UINT32 type, UINT64 id)
{
	std::vector<UINT64> ids;
	ids.push_back(id);
	Pack(type, ids);
}

void FriendNotify::Pack(UINT32 type, const std::vector<UINT64>& ids)
{
	ntf.m_Data.set_op(KKSG::FriendOpType(type));
	for (auto i = ids.begin(); i != ids.end(); ++i)
	{
		switch(type)
		{
		case KKSG::Friend_FriendAdd:
		case KKSG::Friend_FriendAll:
			{
				KKSG::Friend2Client info;
				if (Friend::Friend2Client(roleid, *i, &info))
				{
					KKSG::Friend2Client* tempinfo = ntf.m_Data.add_friendlist(); 
					tempinfo->CopyFrom(info);
				}
				break;
			}
		case KKSG::Friend_ApplyAdd:
		case KKSG::Friend_ApplyAll:
			{
				KKSG::Friend2Client info;
				if (Friend::Friend2Client(roleid, *i, &info))
				{
					KKSG::Friend2Client* tempinfo = ntf.m_Data.add_applylist(); 
					tempinfo->CopyFrom(info);
				}
				break;
			}
		case KKSG::Friend_FriendDelete:
			{
				ntf.m_Data.add_deletelist(*i); 
				break;
			}
		case KKSG::Friend_ApplyDelete:
			{
				ntf.m_Data.add_deleteapplylist(*i); 
				break;
			}
		case KKSG::Friend_ReveiveGift:
			{
				ntf.m_Data.add_senderid(*i); 
				ntf.m_Data.add_receivedtime((UINT32)time(NULL)); 
				break;
			}
		default:
			break;
		}
	}
	if (type == KKSG::Friend_FriendAdd)
	{
		gsntf.m_Data.set_isadd(true);
		gsntf.m_Data.set_roleid(roleid);
		for (auto i = ids.begin(); i != ids.end(); ++i)	
		{
			isnotifygs = true;
			gsntf.m_Data.add_friendid(*i);
		}
	}
	else if (type == KKSG::Friend_FriendDelete)
	{
		gsntf.m_Data.set_isadd(false);
		gsntf.m_Data.set_roleid(roleid);
		for (auto i = ids.begin(); i != ids.end(); ++i)	
		{
			isnotifygs = true;
			gsntf.m_Data.add_friendid(*i);
		}
	}
}

void FriendNotify::PackCount(UINT32 send, UINT32 left)
{
	ntf.m_Data.set_op(KKSG::Friend_GiftInfo);
	KKSG::FriendGift* info = ntf.m_Data.mutable_giftcount();
	info->set_sendleft(send);
	info->set_receiveleft(left);
}

void FriendNotify::Send()
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != role)
	{
		role->Send(ntf);
		if (isnotifygs)
		{
			GSLink::Instance()->SendToLine(role->GetGsLine(), gsntf);
		}
	}
}

FriendOp::FriendOp(UINT64 roleid)
{
	m_invalid = false;
	if (NULL == CRoleSummaryMgr::Instance()->GetRoleSummary(roleid))
	{
		SSError<<"invalid roleid:"<<roleid<<END;
		m_invalid = true;
	}
	m_roleid = roleid;
	m_self = FriendMgr::Instance()->GetFriendInfo(roleid);
	m_other = NULL;
}

FriendOp::~FriendOp()
{

}

void FriendOp::NotifyFriend()
{
	FriendNotify friendall(m_roleid);
	std::vector<UINT64> friendlist;
	m_self->ToFriendList(friendlist);
	friendall.Pack(KKSG::Friend_FriendAll, friendlist);
	friendall.Send();
}

void FriendOp::NotifyApply()
{
	FriendNotify applyall(m_roleid);
	std::vector<UINT64> applylist;
	m_self->ToApplyList(applylist);
	applyall.Pack(KKSG::Friend_ApplyAll, applylist);
	applyall.Send();
}

void FriendOp::NotifyGiftCount()
{
	m_self->m_record.Update();
	UINT32 sendleft = m_self->m_record.GetSendLeft();
	UINT32 receiveleft = m_self->m_record.GetReceiveLeft();
	FriendNotify gift(m_roleid);
	gift.PackCount(sendleft, receiveleft);
	gift.Send();
}

void FriendOp::NotifyGS(CRole* role)
{
	PtcM2G_UpdateFriendOnGS gsntf;
	gsntf.m_Data.set_isadd(true);
	gsntf.m_Data.set_roleid(role->GetID());

	std::set<FriendInfo> friendlist;
	GetFriendListSorted(friendlist);
	for (auto i = friendlist.begin(); i != friendlist.end(); ++i)
	{
		gsntf.m_Data.add_friendid(i->m_roleid);
	}

	GSLink::Instance()->SendToLine(role->GetGsLine(), gsntf);
}

void FriendOp::SetOther(UINT64 otherid)
{
	if (NULL == CRoleSummaryMgr::Instance()->GetRoleSummary(otherid))
	{
		SSError<<"invalid roleid:"<<otherid<<END;
		m_invalid = true;
	}
	m_otherid = otherid;
	m_other = FriendMgr::Instance()->GetFriendInfo(otherid);
}

bool FriendOp::IsFriendMax()
{
	return m_self->m_friends.size() >= MAX_FRIEND_COUNT;
}

bool FriendOp::IsFriend(UINT64 otherid)
{
	return m_self->IsFriend(otherid);
}

bool FriendOp::DeleteApply(UINT64 otherid)
{
	if (m_self->DeleteApply(otherid))
	{
		FriendNotify ntf(m_roleid);
		ntf.Pack(KKSG::Friend_ApplyDelete, otherid);
		ntf.Send();
		return true;
	}
	return false;
}

void FriendOp::DeleteApplyBoth()
{
	if (m_self->DeleteApply(m_otherid))
	{
		FriendNotify ntf(m_roleid);
		ntf.Pack(KKSG::Friend_ApplyDelete, m_otherid);
		ntf.Send();
	}

	if (m_other->DeleteApply(m_roleid))
	{
		FriendNotify ntf(m_otherid);
		ntf.Pack(KKSG::Friend_ApplyDelete, m_roleid);
		ntf.Send();
	}
}

UINT32 FriendOp::AddApply()
{
	UINT32 errorcode = CanAddFriend();
	if (KKSG::ERR_SUCCESS == errorcode)
	{
		UINT64 deleted = 0;
		if (m_other->AddApply(m_roleid, deleted))
		{
			FriendNotify ntf(m_otherid);
			ntf.Pack(KKSG::Friend_ApplyAdd, m_roleid);
			ntf.Send();
		}
		if (0 != deleted)
		{
			FriendNotify ntf(m_otherid);
			ntf.Pack(KKSG::Friend_ApplyDelete, deleted);
			ntf.Send();
		}
	}
	return errorcode;
}

bool FriendOp::IsInApply()
{
	return m_self->InApply(m_otherid);	
}

UINT32 FriendOp::CanAddFriend()
{
	UINT32 errorcode = m_self->CanAddFriend(m_otherid);
	if (FriendSuccess == errorcode)
	{
		return ErrorCodeTrans(m_other->CanAddFriend(m_roleid), false);
	}
	return ErrorCodeTrans(errorcode);
}

UINT32 FriendOp::AddFriend()
{
	UINT32 errorcode = CanAddFriend();

	if (KKSG::ERR_SUCCESS == errorcode)
	{
		m_self->AddFriend(m_otherid);
		m_self->Changed();

		m_other->AddFriend(m_roleid);
		m_other->Changed();

		FriendNotify me(m_roleid);
		me.Pack(KKSG::Friend_FriendAdd, m_otherid);
		me.Send();

		FriendNotify other(m_otherid);
		other.Pack(KKSG::Friend_FriendAdd, m_roleid);
		other.Send();

		m_other->AddNoNotifyAddFriend(m_roleid);
		FriendMgr::Instance()->SendNoNotifyAddFriend(m_otherid);

		DoTxLog(m_roleid, m_otherid, TX_AddFriend);
		DoTxLog(m_otherid, m_roleid, TX_AddFriend);
	}

	return errorcode;
}

UINT32 FriendOp::RemoveFriend()
{
	if (PartnerMgr::Instance()->IsPartner(m_roleid, m_otherid))
	{
		return KKSG::ERR_FRIEND_IS_PARTNER;
	}
	UINT32 errorcode1 = m_self->RemoveFriend(m_otherid);
	if (errorcode1 == FriendSuccess)
	{
		FriendNotify me(m_roleid);
		me.Pack(KKSG::Friend_FriendDelete, m_otherid);
		me.Send();

		m_self->Changed();
		DoTxLog(m_roleid, m_otherid, TX_DelFriend);
	}

	UINT32 errorcode2 = m_other->RemoveFriend(m_roleid);
	if (errorcode2 == FriendSuccess)
	{
		FriendNotify other(m_otherid);
		other.Pack(KKSG::Friend_FriendDelete, m_roleid);
		other.Send();

		m_other->Changed();
		DoTxLog(m_otherid, m_roleid, TX_DelFriend);
	}

	return ErrorCodeTrans(FriendSuccess);
}

const std::map<UINT64, FriendInfo>* FriendOp::GetFriendList()
{
	return &m_self->m_friends;
}

void FriendOp::GetFriendListSorted(std::set<FriendInfo>& friendlist)
{
	for (auto i = m_self->m_friends.begin(); i != m_self->m_friends.end(); ++i)
	{
		friendlist.insert(i->second);	
	}
}

UINT32 FriendOp::GetFriendCount() const
{
	return m_self->GetFriendCount();
}

UINT32 FriendOp::GetDegree(UINT64 otherid)
{
	return m_self->GetDegree(otherid);
}

UINT32 FriendOp::GetDayDegree(UINT64 otherid)
{
	return m_self->GetDayDegree(otherid);
}

UINT32 FriendOp::AddDegree(UINT32 type, UINT32 degree, UINT32 param)
{
	UINT32 adddegree = (DegreeGM == type) ? degree : (UINT32)(param * FriendConfig::Instance()->GetDegree(type));
	UINT32 countlimit = (DegreeGM == type) ? 0 : FriendConfig::Instance()->GetLimit(type);

	UINT32 ret = m_self->AddDegree(m_roleid, m_otherid, adddegree, type, countlimit);
	if (ret == (UINT32)(-1))
	{
		return ret;
	}
	FriendConfig::Instance()->OnLevelUp(m_roleid, m_otherid, m_self->GetDegree(m_otherid), m_self->GetDayDegree(m_otherid));		
	m_self->Changed();

	// two way
	m_other->AddDegree(m_otherid, m_roleid, adddegree, type, countlimit);
	FriendConfig::Instance()->OnLevelUp(m_otherid, m_roleid, m_other->GetDegree(m_roleid), m_other->GetDayDegree(m_roleid));		
	m_other->Changed();

	PartnerMgr::Instance()->AddFriendDegree(m_roleid, m_otherid, adddegree);

    GoalAwards(m_roleid, m_self);
    GoalAwards(m_otherid, m_other);

	return adddegree; 
}

// Ä¿±ê½±Àø
void FriendOp::GoalAwards(UINT64 xroleid, Friend* xfriend)
{
    UINT64 nTotalFriendDegree = 0;
    for(auto itor = xfriend->m_friends.begin(); itor != xfriend->m_friends.end(); ++itor)
    {
        nTotalFriendDegree += xfriend->GetDegree(itor->first);
    }
    GoalAwardsMgr::Instance()->FreshValue(xroleid, GOAL_AWARD_ID_602, nTotalFriendDegree, 0);
}

void FriendOp::CleanDegree()
{
	m_self->CleanDegree(m_otherid);	
	m_other->CleanDegree(m_roleid);	
}

bool FriendOp::IsInBlackList(UINT64 otherid)
{
	return m_self->IsBlack(otherid);
}

bool FriendOp::IsInOtherBlackList(UINT64 roleid)
{
	return m_other->IsBlack(roleid);
}

UINT32 FriendOp::AddBlackList()
{
	if (PartnerMgr::Instance()->IsPartner(m_roleid, m_otherid))
	{
		return KKSG::ERR_FRIEND_IS_PARTNER;
	}

	UINT32 errorcode = m_self->AddBlackList(m_otherid);

	if (BlackSuccess == errorcode)
	{
		// remove friend
		RemoveFriend();
		m_self->Changed();
		DoTxLog(m_roleid, m_otherid, TX_BlackList_DoMode);
	}

	return ErrorCodeTrans(errorcode);
}

UINT32 FriendOp::RemoveBlackList(UINT64 otherid)
{
	UINT32 errorcode = m_self->RemoveBlackList(otherid);
	if (BlackSuccess == errorcode)
	{
		m_self->Changed();
		DoTxLog(m_roleid, otherid, TX_BlackList_UnDoMode);
	}
	return ErrorCodeTrans(errorcode); 
}

void FriendOp::SendBlackList(CRole* role)
{
	const std::deque<UINT64>* roleids = &m_self->m_blacklist;
	if (NULL != roleids)
	{
		PtcM2C_BlackListNtfNew ntf;
		for (auto i = roleids->begin(); i != roleids->end(); ++i)
		{
			KKSG::Friend2Client* friend2client = ntf.m_Data.add_blacklist();
			Friend::Friend2Client(m_roleid, *i, friend2client);		
		}
		role->Send(ntf);
	}
}

UINT32 FriendOp::GetReceiveAll(UINT64 otherid)
{
	return m_self->GetReceiveAll(otherid);
}

UINT32 FriendOp::GetReceiveTime(UINT64 otherid)
{
	return m_self->GetReceiveTime(otherid);
}

UINT32 FriendOp::GetReceive(UINT64 otherid)
{
	return m_self->GetReceive(otherid);
}

UINT32 FriendOp::GetSend(UINT64 otherid)
{
	return m_self->GetSend(otherid);
}

bool FriendOp::SetReceive(UINT64 otherid, UINT32 state)
{
	if (m_self->SetReceive(otherid, state))
	{
		m_self->Changed();
		return true;
	}
	return false;
}

bool FriendOp::SetSend(UINT64 otherid, UINT32 state)
{
	if (m_self->SetSend(otherid, state))
	{
		m_self->Changed();

		if (m_other->SetReceive(m_roleid, KKSG::FriendGift_Received))
		{
			m_other->Changed();

			FriendNotify receive(m_otherid);
			receive.Pack(KKSG::Friend_ReveiveGift, m_roleid);
			receive.Send();
			return true;
		}
	}
	return false;
}

void FriendOp::FillFriendList(KKSG::FriendList* list)
{
	for (auto i = m_self->m_friends.begin(); i != m_self->m_friends.end(); ++i)
	{
		list->add_friends(i->first);
	}
}

void FriendOp::DoTxLog(UINT64 roleid, UINT64 otherid, INT32 Op)
{
	TFriendFlow oLog1(TLogMgr::Instance()->GetTagString());
	Friend* pFriend = FriendMgr::Instance()->GetFriendInfo(roleid);
	if (pFriend)
	{
		oLog1.m_FriendNum = pFriend->GetFriendCount();
	}
	oLog1.m_Op = Op;
	oLog1.m_uRoleID = roleid;
	oLog1.m_Roleid2 = otherid;
	oLog1.Do();

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (pRole)
	{
		CReportDataMgr::Instance()->Common(pRole, Tx_Reportdata_FriendCount, ToString(oLog1.m_FriendNum));
	}
}
