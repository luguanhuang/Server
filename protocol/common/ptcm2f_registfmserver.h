#ifndef __PTCM2F_REGISTFMSERVER_H__
#define __PTCM2F_REGISTFMSERVER_H__

// generate by ProtoGen at date: 2016/11/23 12:51:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_REGISTFMSERVER_TYPE 35213

class PtcM2F_RegistFMServer : public CProtocol
{
public:
    explicit PtcM2F_RegistFMServer() : CProtocol(PTCM2F_REGISTFMSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_RegistFMServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif