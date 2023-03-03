#ifndef __PTCG2C_RESWARTEAMRESONE_H__
#define __PTCG2C_RESWARTEAMRESONE_H__

// generate by ProtoGen at date: 2016/11/1 18:28:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RESWARTEAMRESONE_TYPE 8869

class PtcG2C_ResWarTeamResOne : public CProtocol
{
public:
    explicit PtcG2C_ResWarTeamResOne() : CProtocol(PTCG2C_RESWARTEAMRESONE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ResWarTeamResOne()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ResWarAllTeamBaseInfo m_Data;
};

#endif