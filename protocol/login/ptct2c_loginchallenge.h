#ifndef __PTCT2C_LOGINCHALLENGE_H__
#define __PTCT2C_LOGINCHALLENGE_H__

// generate by ProtoGen at date: 2017/2/6 23:18:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_LOGINCHALLENGE_TYPE 58495

class PtcT2C_LoginChallenge : public CProtocol
{
public:
    explicit PtcT2C_LoginChallenge() : CProtocol(PTCT2C_LOGINCHALLENGE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2C_LoginChallenge()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LoginChallenge m_Data;
};

#endif