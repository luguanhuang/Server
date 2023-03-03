#ifndef __PTCM2I_UPDATESELFSERVER_H__
#define __PTCM2I_UPDATESELFSERVER_H__

// generate by ProtoGen at date: 2016/8/24 20:55:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2I_UPDATESELFSERVER_TYPE 26469

class PtcM2I_UpdateSelfServer : public CProtocol
{
public:
    explicit PtcM2I_UpdateSelfServer() : CProtocol(PTCM2I_UPDATESELFSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2I_UpdateSelfServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateSelfServer m_Data;
};

#endif