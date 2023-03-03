#ifndef __PTCG2W_REGISTGAMESERVERTOWEBSERVER_H__
#define __PTCG2W_REGISTGAMESERVERTOWEBSERVER_H__

// generate by ProtoGen at date: 2016/2/22 20:03:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2W_REGISTGAMESERVERTOWEBSERVER_TYPE 23920

class PtcG2W_RegistGameServerToWebServer : public CProtocol
{
public:
    explicit PtcG2W_RegistGameServerToWebServer() : CProtocol(PTCG2W_REGISTGAMESERVERTOWEBSERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2W_RegistGameServerToWebServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif