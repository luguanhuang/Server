#ifndef __PTCG2M_CLIENTSESSIONCHANGEG2M_H__
#define __PTCG2M_CLIENTSESSIONCHANGEG2M_H__

// generate by ProtoGen at date: 2017/6/21 17:36:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_CLIENTSESSIONCHANGEG2M_TYPE 591

class PtcG2M_ClientSessionChangeG2M : public CProtocol
{
public:
    explicit PtcG2M_ClientSessionChangeG2M() : CProtocol(PTCG2M_CLIENTSESSIONCHANGEG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_ClientSessionChangeG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ClientSessionChangeG2M m_Data;
};

#endif