#ifndef __PTCM2G_SYNGUILDCHECKINHINT_H__
#define __PTCM2G_SYNGUILDCHECKINHINT_H__

// generate by ProtoGen at date: 2016/9/20 19:30:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDCHECKINHINT_TYPE 53260

class PtcM2G_SynGuildCheckinHint : public CProtocol
{
public:
    explicit PtcM2G_SynGuildCheckinHint() : CProtocol(PTCM2G_SYNGUILDCHECKINHINT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildCheckinHint()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildCheckinHint m_Data;
};

#endif