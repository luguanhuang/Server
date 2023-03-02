#include "pch.h"
#include "teammemberbase.h"

TeamMemberBase::TeamMemberBase()
{
	m_type = KKSG::TMT_NORMAL;
}

void TeamMemberBase::SetExtraData(const KKSG::TeamSynExtraData& data)
{
	m_extraData.MergeFrom(data);//±ÿ–Î”√MergeFrom
}

void TeamMemberBase::FillTeamMemberFromExtraData(KKSG::TeamMember& member, const KKSG::TeamSynExtraData& extra) const
{
	member.set_leftcount(extra.leftcount());
	member.set_guildid(extra.guildid());
	member.set_sceneid(extra.sceneid());
	member.set_dragonguildid(extra.dragonguildid());
	member.set_kingback(IsKingBack());
}

bool TeamMemberBase::IsKingBack() const
{
	return (m_type != KKSG::TMT_HELPER && m_extraData.kingback());
}
