#include "pch.h"
#include "weddingrole.h"
#include "wedding.h"
#include "weddingmgr.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"

WeddingRole::WeddingRole(UINT64 roleId)
:m_roleId(roleId)
,m_selfWeddingId(0)
{

}

Wedding* WeddingRole::GetSelfWedding()
{
	if (m_selfWeddingId == 0)
	{
		return NULL;
	}
	return WeddingMgr::Instance()->GetWedding(m_selfWeddingId);
}

void WeddingRole::AddToInviteList(UINT64 roleId)
{
	if (std::find(m_inviteList.begin(), m_inviteList.end(), roleId) == m_inviteList.end())
	{
		m_inviteList.push_back(roleId);
	}
}

void WeddingRole::OnSelfWeddingEnd()
{
	m_selfWeddingId = 0;
	m_inviteList.clear();
	Marriage* pMarr = MarriageMgr::Instance()->GetMarriage(m_roleId);
	if (pMarr)
	{
		pMarr->SetHoldedWedding(true);
	}
}

bool WeddingRole::CanDestory()
{
	if (m_selfWeddingId != 0)
	{
		return false;
	}
	return true;
}
