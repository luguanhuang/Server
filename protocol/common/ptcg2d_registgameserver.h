#ifndef __PTCG2D_REGISTGAMESERVER_H__
#define __PTCG2D_REGISTGAMESERVER_H__

// generate by ProtoGen at date: 2016/12/16 16:00:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_REGISTGAMESERVER_TYPE 26354

class PtcG2D_RegistGameServer : public CProtocol
{
public:
    explicit PtcG2D_RegistGameServer() : CProtocol(PTCG2D_REGISTGAMESERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_RegistGameServer()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RegistServer m_Data;
};

#endif