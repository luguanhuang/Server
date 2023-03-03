#ifndef __PTCM2G_TEAMTRANMS2GS_H__
#define __PTCM2G_TEAMTRANMS2GS_H__

// generate by ProtoGen at date: 2017/7/10 21:23:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_TEAMTRANMS2GS_TYPE 59361

class PtcM2G_TeamTranMs2Gs : public CProtocol
{
public:
    explicit PtcM2G_TeamTranMs2Gs() : CProtocol(PTCM2G_TEAMTRANMS2GS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_TeamTranMs2Gs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamTransData m_Data;
};

#endif