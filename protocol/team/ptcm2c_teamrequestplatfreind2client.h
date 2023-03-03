#ifndef __PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_H__
#define __PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_H__

// generate by ProtoGen at date: 2017/5/4 20:50:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_TYPE 37841

class PtcM2C_TeamRequestPlatFreind2Client : public CProtocol
{
public:
    explicit PtcM2C_TeamRequestPlatFreind2Client() : CProtocol(PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TeamRequestPlatFreind2Client()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamRequestPlatFreind2ClientData m_Data;
};

#endif