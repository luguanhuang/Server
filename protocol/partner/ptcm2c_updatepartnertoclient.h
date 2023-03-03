#ifndef __PTCM2C_UPDATEPARTNERTOCLIENT_H__
#define __PTCM2C_UPDATEPARTNERTOCLIENT_H__

// generate by ProtoGen at date: 2017/3/15 10:56:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_UPDATEPARTNERTOCLIENT_TYPE 63692

class PtcM2C_UpdatePartnerToClient : public CProtocol
{
public:
    explicit PtcM2C_UpdatePartnerToClient() : CProtocol(PTCM2C_UPDATEPARTNERTOCLIENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_UpdatePartnerToClient()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdatePartnerToClient m_Data;
};

#endif