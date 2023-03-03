#ifndef __PTCG2C_RESWARTEAMNTF_H__
#define __PTCG2C_RESWARTEAMNTF_H__

// generate by ProtoGen at date: 2016/11/1 16:16:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RESWARTEAMNTF_TYPE 40701

class PtcG2C_ResWarTeamNtf : public CProtocol
{
public:
    explicit PtcG2C_ResWarTeamNtf() : CProtocol(PTCG2C_RESWARTEAMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ResWarTeamNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarAllTeamBaseInfo m_Data;
};

#endif