#include "pch.h"
#include "flowerop.h"
#include "social.h"
#include "socialmgr.h"
#include "flower.h"

FlowerOp::FlowerOp(UINT64 roleid)
:SocialOp(roleid)
{

}

FlowerOp::~FlowerOp()
{

}

void FlowerOp::SendFlower(UINT64 send, UINT32 itemid, UINT32 count)
{
	if (m_info)
	{
		m_info->flower.AddFlower(send, m_roleid, itemid, count);
	}

	SocialInfo* info = SocialMgr::Instance()->GetSocialInfo(send);
	if (info)
	{
		info->flower.AddSendFlower(send, m_roleid, itemid, count);
	}

	//FriendOp op(send);
	//op.SetOther(m_roleid);
	//op.AddDegree(DegreeFlower, 0, count);

	SocialMgr::Instance()->SetChanged(m_roleid);
	SocialMgr::Instance()->SetChanged(send);

	return;
}

Flower* FlowerOp::GetFlowerInfo()
{
	if (m_info)
	{
		return &m_info->flower;
	}
	else
		return NULL;
}
