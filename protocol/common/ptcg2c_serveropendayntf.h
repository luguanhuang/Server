#ifndef __PTCG2C_SERVEROPENDAYNTF_H__
#define __PTCG2C_SERVEROPENDAYNTF_H__

// generate by ProtoGen at date: 2016/11/11 21:00:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SERVEROPENDAYNTF_TYPE 23820

class PtcG2C_ServerOpenDayNtf : public CProtocol
{
public:
    explicit PtcG2C_ServerOpenDayNtf() : CProtocol(PTCG2C_SERVEROPENDAYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ServerOpenDayNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ServerOpenDay m_Data;
};

#endif