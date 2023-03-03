#ifndef __PTCG2C_SYNGUILDARENAROLEONLINE_H__
#define __PTCG2C_SYNGUILDARENAROLEONLINE_H__

// generate by ProtoGen at date: 2016/7/27 20:32:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDARENAROLEONLINE_TYPE 48528

class PtcG2C_SynGuildArenaRoleOnline : public CProtocol
{
public:
    explicit PtcG2C_SynGuildArenaRoleOnline() : CProtocol(PTCG2C_SYNGUILDARENAROLEONLINE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynGuildArenaRoleOnline()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaRoleOnline m_Data;
};

#endif