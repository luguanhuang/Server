#ifndef __PTCM2T_NOTIFYROUTERINFO2GATE_H__
#define __PTCM2T_NOTIFYROUTERINFO2GATE_H__

// generate by ProtoGen at date: 2017/3/3 16:55:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2T_NOTIFYROUTERINFO2GATE_TYPE 4039

class PtcM2T_NotifyRouterInfo2Gate : public CProtocol
{
public:
    explicit PtcM2T_NotifyRouterInfo2Gate() : CProtocol(PTCM2T_NOTIFYROUTERINFO2GATE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2T_NotifyRouterInfo2Gate()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RouterListenInfo m_Data;
};

#endif