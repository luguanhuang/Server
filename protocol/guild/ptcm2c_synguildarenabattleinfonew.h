#ifndef __PTCM2C_SYNGUILDARENABATTLEINFONEW_H__
#define __PTCM2C_SYNGUILDARENABATTLEINFONEW_H__

// generate by ProtoGen at date: 2016/9/8 15:15:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILDARENABATTLEINFONEW_TYPE 3680

class PtcM2C_SynGuildArenaBattleInfoNew : public CProtocol
{
public:
    explicit PtcM2C_SynGuildArenaBattleInfoNew() : CProtocol(PTCM2C_SYNGUILDARENABATTLEINFONEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SynGuildArenaBattleInfoNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildArenaBattleInfo m_Data;
};

#endif