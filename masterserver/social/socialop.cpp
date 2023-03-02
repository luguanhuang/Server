#include "pch.h"
#include "socialop.h"
#include "socialmgr.h"

SocialOp::SocialOp(UINT64 roleid)
{
	m_roleid = roleid;
	m_info = SocialMgr::Instance()->GetSocialInfo(roleid);
}

SocialOp::~SocialOp()
{

}

