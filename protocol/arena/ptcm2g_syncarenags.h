#ifndef __PTCM2G_SYNCARENAGS_H__
#define __PTCM2G_SYNCARENAGS_H__

// generate by ProtoGen at date: 2016/9/11 18:07:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNCARENAGS_TYPE 65026

class PtcM2G_SyncArenaGs : public CProtocol
{
public:
    explicit PtcM2G_SyncArenaGs() : CProtocol(PTCM2G_SYNCARENAGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SyncArenaGs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncArenaGs m_Data;
};

#endif