#ifndef __PTCG2C_LOGINACTIVITYSTATUSNTF_H__
#define __PTCG2C_LOGINACTIVITYSTATUSNTF_H__

// generate by ProtoGen at date: 2017/6/2 14:48:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LOGINACTIVITYSTATUSNTF_TYPE 34113

class PtcG2C_LoginActivityStatusNtf : public CProtocol
{
public:
    explicit PtcG2C_LoginActivityStatusNtf() : CProtocol(PTCG2C_LOGINACTIVITYSTATUSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LoginActivityStatusNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LoginActivityStatus m_Data;
};

#endif