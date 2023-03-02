#include "pch.h"
#include <time.h>
#include "broadcast.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "gamelogic/firstpassconfig.h"
#include "scene/scene.h"
#include "scene/sceneteam.h"
#include "foreach.h"
#include "network/mslink.h"
#include "protocol.h"
#include "broadcast/ptcg2m_broadcast.h"
#include "network/mslink.h"
#include "unit/role.h"
#include "scene/sceneline.h"
#include "broadcast/ptcg2t_broadcastg2t.h"
#include "network/gatelink.h"


BroadCastMgr::BroadCastMgr()
{

}

BroadCastMgr::~BroadCastMgr()
{

}

bool BroadCastMgr::Init()
{
	return true;
}

void BroadCastMgr::Uninit()
{

}

void BroadCastMgr::BroadCast(const CProtocol& roPtc, KKSG::BroadCastType type)
{
	MemoryStream *stream = MemoryStream::Get();
	// 发送给客户端的包头，不包含Session字段
	const int HeadSize = sizeof(ClientProtocolHead);
	stream->Seek(HeadSize);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%u] failed.", roPtc.m_dwType);
		return;
	}

	stream->Advance(len);

	ClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roPtc.m_dwType;
	head.dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	PtcG2M_BroadCast ntf;
	ntf.m_Data.set_broadcastdata(stream->GetBuffer(), stream->GetLength());
	ntf.m_Data.set_datalen(stream->GetLength());
	ntf.m_Data.set_broadcasttype(type);
	MSLink::Instance()->SendTo(ntf);
}

///

void CBroadCastSender::Broadcast(const std::vector<Role*>& roles, const CProtocol& roPtc)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			role->Send(roPtc);
		}
	}
	else
	{
		std::string data;
		if(!SerializePtc(roPtc, data) || data.empty())
		{
			return;
		}

		std::map<UINT32, KKSG::BroadCastG2TData> oLineMap;
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			if (!role->CanSendTo())
			{
				continue;
			}
			UINT32 dwLine = GetGateLine(role->GetSessionID());

			KKSG::BroadCastG2TData& roBroad = oLineMap[dwLine];
			roBroad.add_sessionlist(role->GetSessionID());
		}
		for(std::map<UINT32, KKSG::BroadCastG2TData>::iterator it = oLineMap.begin(); it != oLineMap.end(); ++it)
		{
			KKSG::BroadCastG2TData& roBroad = it->second;
			if(roBroad.sessionlist_size() == 0) continue;

			PtcG2T_BroadCastG2T ptc;
			ptc.m_Data.mutable_sessionlist()->CopyFrom(roBroad.sessionlist());
			ptc.m_Data.set_data(data);
			GatetLink::Instance()->SendToLine(it->first, ptc);
		}
	}
}

void CBroadCastSender::Broadcast(const std::unordered_map<UINT64, Role*>& roles, const CProtocol& roPtc)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = i->second;
			role->Send(roPtc);
		}
	}
	else
	{
		std::string data;
		if(!SerializePtc(roPtc, data) || data.empty())
		{
			return;
		}

		std::map<UINT32, KKSG::BroadCastG2TData> oLineMap;
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = i->second;
			if (!role->CanSendTo())
			{
				continue;
			}
			UINT32 dwLine = GetGateLine(role->GetSessionID());

			KKSG::BroadCastG2TData& roBroad = oLineMap[dwLine];
			roBroad.add_sessionlist(role->GetSessionID());
		}
		for(std::map<UINT32, KKSG::BroadCastG2TData>::iterator it = oLineMap.begin(); it != oLineMap.end(); ++it)
		{
			KKSG::BroadCastG2TData& roBroad = it->second;
			if(roBroad.sessionlist_size() == 0) continue;

			PtcG2T_BroadCastG2T ptc;
			ptc.m_Data.mutable_sessionlist()->CopyFrom(roBroad.sessionlist());
			ptc.m_Data.set_data(data);
			GatetLink::Instance()->SendToLine(it->first, ptc);
		}
	}
}

void CBroadCastSender::Broadcast(const std::unordered_map<UINT64, RoleViewSet*>& roles, const CProtocol& roPtc, Unit* poExlude)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = i->second->m_role;
			if(poExlude != NULL && role == poExlude) continue;

			role->Send(roPtc);
		}
	}
	else
	{
		std::string data;
		if(!SerializePtc(roPtc, data) || data.empty())
		{
			return;
		}

		std::map<UINT32, KKSG::BroadCastG2TData> oLineMap;
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = i->second->m_role;
			if(poExlude != NULL && role == poExlude) continue;
			if (!role->CanSendTo())
			{
				continue;
			}

			UINT32 dwLine = GetGateLine(role->GetSessionID());
			KKSG::BroadCastG2TData& roBroad = oLineMap[dwLine];
			roBroad.add_sessionlist(role->GetSessionID());
		}
		for(std::map<UINT32, KKSG::BroadCastG2TData>::iterator it = oLineMap.begin(); it != oLineMap.end(); ++it)
		{
			KKSG::BroadCastG2TData& roBroad = it->second;
			if(roBroad.sessionlist_size() == 0) continue;

			PtcG2T_BroadCastG2T ptc;
			ptc.m_Data.mutable_sessionlist()->CopyFrom(roBroad.sessionlist());
			ptc.m_Data.set_data(data);
			GatetLink::Instance()->SendToLine(it->first, ptc);
		}
	}
}

bool CBroadCastSender::SerializePtc(const CProtocol& roPtc, std::string& strData)
{
	strData.clear();
	MemoryStream *stream = MemoryStream::Get();

	// 发送给客户端的包头，不包含Session字段
	const int HeadSize = sizeof(ClientProtocolHead);
	stream->Seek(HeadSize);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%s] failed.", CProtocol::GetProtoName(roPtc.m_dwType));
		return false;
	}

	stream->Advance(len);

	ClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roPtc.m_dwType;
	head.dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	strData.assign(stream->GetBuffer(), stream->GetLength());
	return true;
}