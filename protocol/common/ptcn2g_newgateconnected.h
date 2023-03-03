#ifndef __PTCN2G_NEWGATECONNECTED_H__
#define __PTCN2G_NEWGATECONNECTED_H__

// generate by ProtoGen at date: 2016/11/2 10:28:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2G_NEWGATECONNECTED_TYPE 45791

class PtcN2G_NewGateConnected : public CProtocol
{
public:
    explicit PtcN2G_NewGateConnected() : CProtocol(PTCN2G_NEWGATECONNECTED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2G_NewGateConnected()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NewGateConnectedData m_Data;
};

#endif