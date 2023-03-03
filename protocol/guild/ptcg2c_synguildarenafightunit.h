#ifndef __PTCG2C_SYNGUILDARENAFIGHTUNIT_H__
#define __PTCG2C_SYNGUILDARENAFIGHTUNIT_H__

// generate by ProtoGen at date: 2016/7/19 20:00:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SYNGUILDARENAFIGHTUNIT_TYPE 59912

class PtcG2C_SynGuildArenaFightUnit : public CProtocol
{
public:
    explicit PtcG2C_SynGuildArenaFightUnit() : CProtocol(PTCG2C_SYNGUILDARENAFIGHTUNIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SynGuildArenaFightUnit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaFightUnit m_Data;
};

#endif