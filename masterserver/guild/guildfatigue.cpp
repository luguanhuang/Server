#include "pch.h"
#include "guildfatigue.h"
#include "pb/project.pb.h"
#include "role/role.h"
//#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"
//#include "guild/guildrecord.h"
#include "guild/guild.h"
#include "role/rolemanager.h"
#include "guild/ptcm2g_synfatigueresult.h"
#include <numeric>
//#include "gamelogic/systemiddef.h"

#define GUILDFATIGUE_HINT 813

GuildFatigue::GuildFatigue(Guild *pGuild) : m_pGuild(pGuild)
{

}

GuildFatigue::~GuildFatigue()
{

}

bool GuildFatigue::IsChanged()
{
	return false;
}

bool GuildFatigue::Load(const KKSG::GuildAllDBData & guilddata)
{
	return true;
}

void GuildFatigue::Save(KKSG::GuildAllDBData & guilddata)
{
}

const std::string GuildFatigue::ToBinary()
{
	return "";
}

struct FatigueSenderCMP
{
	UINT64 m_qwSender;
	FatigueSenderCMP(UINT64 qwSender) : m_qwSender(qwSender) {}
	bool operator()(const GuildFatigue::SendFatigueInfo &info)
	{
		return info.qwSender == m_qwSender;
	}
};

struct AccFatigue
{
	int operator()(int v, const GuildFatigue::SendFatigueInfo &info)
	{
		if (info.IsRecved == false)
		{
			v += 2;
		}

		return v;
	}
};

static int GetFatigueCanRecv(const std::list<GuildFatigue::SendFatigueInfo> &list)
{
	AccFatigue acc;
	return std::accumulate(list.begin(), list.end(), 0, acc);
}

int GuildFatigue::SendFatigueTo(UINT64 qwRole, UINT64 ToRole, int &totalSend)
{
	if (qwRole == ToRole)
	{
		return KKSG::ERR_FAILED;
	}

	auto it = m_SendFatigueInfo.find(ToRole);
	if (it == m_SendFatigueInfo.end())
	{
		SendFatigueInfo info;
		info.qwSender = qwRole;
		info.IsRecved = false;
		m_SendFatigueInfo[ToRole].push_back(info);
		totalSend += 2;
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		auto ik = std::find_if(it->second.begin(), it->second.end(), FatigueSenderCMP(qwRole));
		if (ik == it->second.end())
		{
			SendFatigueInfo info;
			info.qwSender = qwRole;
			info.IsRecved = false;
			it->second.push_back(info);
			totalSend += 2;

			// 可以接受的体力大于20点，通知客户端红点
			if (GetFatigueCanRecv(it->second) >= 20)
			{
				CRole *pRole = CRoleManager::Instance()->GetByRoleID(ToRole);
				if (pRole)
				{
					//pRole->HintNotify(GuildFatigueId, false);
				}
			}

			return KKSG::ERR_SUCCESS;
		}
		else
		{
			return KKSG::ERR_FAILED;
		}
	}
}


int GuildFatigue::SendFatigue(CRole *pRole, UINT64 ToRole, int &totalSend)
{
	if (ToRole == 0)
	{
		SendAll(pRole, totalSend);
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		return SendFatigueTo(pRole->GetID(), ToRole, totalSend);
	}
}

int GuildFatigue::RecvFatigue(CRole *pRole, UINT64 FromRole, int &totalRecv, UINT32 recvNum)
{
	if (recvNum >= m_pGuild->GetMemberCount())
	{
		return KKSG::ERR_FAILED;
	}

	if (FromRole == 0)
	{
		RecvAll(pRole, totalRecv, recvNum);
		return KKSG::ERR_SUCCESS;
	}

	auto it = m_SendFatigueInfo.find(pRole->GetID());
	if (it == m_SendFatigueInfo.end())
	{
		return KKSG::ERR_FAILED;
	}

	auto ik = std::find_if(it->second.begin(), it->second.end(), FatigueSenderCMP(FromRole));
	if (ik == it->second.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (ik->IsRecved)
	{
		return KKSG::ERR_FAILED;
	}

	PtcM2G_SynFatigueResult msg;
	msg.m_Data.set_roleid(pRole->GetID());
	msg.m_Data.set_totalrecv(1);
	msg.m_Data.set_addtimes(1);
	msg.m_Data.set_isfromrole(FromRole);
	pRole->SendMsgToGS(msg);

	//pRole->Get<CGuildRecord>()->AddRecvFatigue();

	ik->IsRecved = true;

	/*
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Fatigue);
	transition.GiveItem(FATIGUE, GetGlobalConfig().GetInt("GuildSendFatigue", 2));
	transition.NotifyClient();
	*/

	totalRecv = 2;
	return KKSG::ERR_SUCCESS;
}

bool GuildFatigue::CanSendFatigueTo(UINT64 qwRole, UINT64 ToRole)
{
	auto it = m_SendFatigueInfo.find(ToRole);
	if (it == m_SendFatigueInfo.end())
	{
		return true;
	}
	else
	{
		auto ik = std::find_if(it->second.begin(), it->second.end(), FatigueSenderCMP(qwRole));
		if (ik == it->second.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool GuildFatigue::CanRecvFatigueFrom(UINT64 qwRole, UINT64 FromRole, bool &hasRecved)
{
	hasRecved = false;
	auto it = m_SendFatigueInfo.find(qwRole);
	if (it == m_SendFatigueInfo.end())
	{
		return false;
	}

	auto ik = std::find_if(it->second.begin(), it->second.end(), FatigueSenderCMP(FromRole));
	if (ik == it->second.end())
	{
		return false;
	}

	if (ik->IsRecved)
	{
		hasRecved = true;
		return false;
	}

	return true;
}

void GuildFatigue::Clear()
{
	m_SendFatigueInfo.clear();
}

struct TempParam
{
	GuildFatigue *pGF;
	CRole *pRole;
	int totalSend;
};

static void SendFatigueCB(GuildMember *pMember, void *arg)
{
	TempParam *p = (TempParam *)arg;
	p->pGF->SendFatigue(p->pRole, pMember->qwRoleID, p->totalSend);
}

void GuildFatigue::SendAll(CRole *pRole, int &totalSend)
{
	TempParam p;
	p.pGF = this;
	p.pRole = pRole;
	p.totalSend = 0;
	m_pGuild->ForeachMember(SendFatigueCB, &p);
	totalSend = p.totalSend;
}

void GuildFatigue::RecvAll(CRole *pRole, int &totalRecv, UINT32 recvNum)
{
	if (pRole == NULL)
	{
		return ;
	}

	auto it = m_SendFatigueInfo.find(pRole->GetID());
	if (it == m_SendFatigueInfo.end())
	{
		return ;
	}

	//CGuildRecord *pRecord = pRole->Get<CGuildRecord>();
	UINT32 addNum = 0;
	UINT32 member = m_pGuild->GetMemberCount();
	for (auto i = it->second.begin(); i != it->second.end(); ++i)
	{
		if (i->IsRecved)
		{
			continue;
		}

		if (recvNum + addNum >= member)
		{
			break;
		}

		++addNum;
		//pRecord->AddRecvFatigue();
		i->IsRecved = true;
		totalRecv += 2;
	}

	PtcM2G_SynFatigueResult msg;
	msg.m_Data.set_roleid(pRole->GetID());
	msg.m_Data.set_totalrecv(totalRecv);
	msg.m_Data.set_addtimes(addNum);
	msg.m_Data.set_isfromrole(0);
	pRole->SendMsgToGS(msg);

	/*
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Fatigue);
	transition.GiveItem(FATIGUE, GetGlobalConfig().GetInt("GuildSendFatigue", 0) * totalRecv);
	transition.NotifyClient();
	*/
}

void GuildFatigue::Hint(CRole *pRole)
{
	auto it = m_SendFatigueInfo.find(pRole->GetID());
	if (it == m_SendFatigueInfo.end())
	{
		return ;
	}

	if (GetFatigueCanRecv(it->second) >= 20)
	{
		pRole->HintNotify(GuildFatigueId, false);
	}
}
