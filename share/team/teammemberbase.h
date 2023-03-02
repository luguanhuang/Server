#ifndef __TEAMMEMBERBASE_H__
#define __TEAMMEMBERBASE_H__

#include "pb/project.pb.h"
#include "pb/enum.pb.h"

class TeamMemberBase
{
public:
	TeamMemberBase();

	void SetExtraData(const KKSG::TeamSynExtraData& data);

	bool IsKingBack() const;

protected:
	void FillTeamMemberFromExtraData(KKSG::TeamMember& member, const KKSG::TeamSynExtraData& extra) const;

protected:
	//需要同步过来的一些额外信息(公会 场景 剩余次数...)from ms/gs
	KKSG::TeamSynExtraData m_extraData;//(机器人要对此数据特殊处理)
	KKSG::TeamMemberType m_type;//pve使用
};

#endif