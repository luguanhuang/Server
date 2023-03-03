#ifndef __PTCG2C_UPDATEPAYINFO_H__
#define __PTCG2C_UPDATEPAYINFO_H__

// generate by ProtoGen at date: 2015/7/9 15:43:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATEPAYINFO_TYPE 22775

class PtcG2C_UpdatePayInfo : public CProtocol
{
public:
    explicit PtcG2C_UpdatePayInfo() : CProtocol(PTCG2C_UPDATEPAYINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdatePayInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayInfo m_Data;
};

#endif