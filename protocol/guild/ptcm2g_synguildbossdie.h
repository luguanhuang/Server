#ifndef __PTCM2G_SYNGUILDBOSSDIE_H__
#define __PTCM2G_SYNGUILDBOSSDIE_H__

// generate by ProtoGen at date: 2017/1/14 11:44:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDBOSSDIE_TYPE 45172

class PtcM2G_SynGuildBossDie : public CProtocol
{
public:
    explicit PtcM2G_SynGuildBossDie() : CProtocol(PTCM2G_SYNGUILDBOSSDIE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildBossDie()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildBossDie m_Data;
};

#endif