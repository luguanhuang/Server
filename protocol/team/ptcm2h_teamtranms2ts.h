#ifndef __PTCM2H_TEAMTRANMS2TS_H__
#define __PTCM2H_TEAMTRANMS2TS_H__

// generate by ProtoGen at date: 2017/7/10 21:21:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2H_TEAMTRANMS2TS_TYPE 57050

class PtcM2H_TeamTranMs2Ts : public CProtocol
{
public:
    explicit PtcM2H_TeamTranMs2Ts() : CProtocol(PTCM2H_TEAMTRANMS2TS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2H_TeamTranMs2Ts()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamTransData m_Data;
};

#endif