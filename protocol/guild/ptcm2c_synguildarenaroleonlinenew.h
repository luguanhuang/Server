#ifndef __PTCM2C_SYNGUILDARENAROLEONLINENEW_H__
#define __PTCM2C_SYNGUILDARENAROLEONLINENEW_H__

// generate by ProtoGen at date: 2016/9/8 15:17:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILDARENAROLEONLINENEW_TYPE 26598

class PtcM2C_SynGuildArenaRoleOnlineNew : public CProtocol
{
public:
    explicit PtcM2C_SynGuildArenaRoleOnlineNew() : CProtocol(PTCM2C_SYNGUILDARENAROLEONLINENEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SynGuildArenaRoleOnlineNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaRoleOnline m_Data;
};

#endif