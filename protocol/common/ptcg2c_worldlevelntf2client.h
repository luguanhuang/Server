#ifndef __PTCG2C_WORLDLEVELNTF2CLIENT_H__
#define __PTCG2C_WORLDLEVELNTF2CLIENT_H__

// generate by ProtoGen at date: 2017/10/27 12:07:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORLDLEVELNTF2CLIENT_TYPE 63449

class PtcG2C_WorldLevelNtf2Client : public CProtocol
{
public:
    explicit PtcG2C_WorldLevelNtf2Client() : CProtocol(PTCG2C_WORLDLEVELNTF2CLIENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WorldLevelNtf2Client()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldLevel m_Data;
};

#endif