#ifndef __PTCM2P_REGISTMSSERVER_H__
#define __PTCM2P_REGISTMSSERVER_H__

// generate by ProtoGen at date: 2016/10/28 22:32:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2P_REGISTMSSERVER_TYPE 51480

class PtcM2P_RegistMSServer : public CProtocol
{
public:
    explicit PtcM2P_RegistMSServer() : CProtocol(PTCM2P_REGISTMSSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2P_RegistMSServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif