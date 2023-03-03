#ifndef __PTCG2C_WEEKEND4V4ROLEDATANTF_H__
#define __PTCG2C_WEEKEND4V4ROLEDATANTF_H__

// generate by ProtoGen at date: 2017/6/8 21:25:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WEEKEND4V4ROLEDATANTF_TYPE 54598

class PtcG2C_WeekEnd4v4RoleDataNtf : public CProtocol
{
public:
    explicit PtcG2C_WeekEnd4v4RoleDataNtf() : CProtocol(PTCG2C_WEEKEND4V4ROLEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WeekEnd4v4RoleDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeekEnd4v4BattleAllRoleData m_Data;
};

#endif