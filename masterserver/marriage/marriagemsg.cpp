#include "pch.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"
#include "table/globalconfig.h"
#include "marriagemsg.h"
#include "marriage.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "util/gametime.h"


MarriageMsg::MarriageMsg(Marriage* pMarriage)
{
	m_pMarriage = pMarriage;
}

MarriageMsg::~MarriageMsg()
{

}

void MarriageMsg::Update()
{
	int now = GameTime::GetTime();
	for (auto iter = m_applyList.begin(); iter != m_applyList.end();)
	{
		if (now > iter->second.applyTime && now - iter->second.applyTime > 3600)
		{
			m_applyList.erase(iter++);
		}
		else
		{
			++iter;
		}
	}

}

void MarriageMsg::AddApplyMsg(UINT64 roleID, KKSG::WeddingType type)
{
	MarriageApplyInfo info;
	info.applyTime = GameTime::GetTime();
	info.applyType = type;
	m_applyList[roleID] = info;
}

bool MarriageMsg::CheckApplyInfoExist(UINT64 srcID, UINT64 destID, KKSG::WeddingType& type)
{
	UINT64 ownerID = m_pMarriage->GetOwnerID();
	if (srcID == ownerID)
	{
		auto find = m_applyList.find(destID);
		if (find == m_applyList.end())
		{
			return false;
		}

		type = find->second.applyType;

	}
	if (destID == ownerID)
	{
		LogError("%llu, check dest be apply msg", ownerID);
	}

	return true;
}

void MarriageMsg::RemoveApplyInfo(UINT64 srcID, UINT64 destID)
{
	UINT64 ownerID = m_pMarriage->GetOwnerID();
	if (srcID == ownerID)
	{
		m_applyList.erase(destID);
	}
	if (destID == ownerID)
	{
		LogError("%llu, remove dest be apply msg", ownerID);
	}
}

