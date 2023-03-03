#ifndef __PTCM2C_SYNGUILDARENAFIGHTUNITNEW_H__
#define __PTCM2C_SYNGUILDARENAFIGHTUNITNEW_H__

// generate by ProtoGen at date: 2016/9/8 15:15:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILDARENAFIGHTUNITNEW_TYPE 34513

class PtcM2C_SynGuildArenaFightUnitNew : public CProtocol
{
public:
    explicit PtcM2C_SynGuildArenaFightUnitNew() : CProtocol(PTCM2C_SYNGUILDARENAFIGHTUNITNEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SynGuildArenaFightUnitNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaFightUnit m_Data;
};

#endif