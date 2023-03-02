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
	//��Ҫͬ��������һЩ������Ϣ(���� ���� ʣ�����...)from ms/gs
	KKSG::TeamSynExtraData m_extraData;//(������Ҫ�Դ��������⴦��)
	KKSG::TeamMemberType m_type;//pveʹ��
};

#endif