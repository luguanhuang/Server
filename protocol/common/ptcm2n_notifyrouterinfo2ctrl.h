#ifndef __PTCM2N_NOTIFYROUTERINFO2CTRL_H__
#define __PTCM2N_NOTIFYROUTERINFO2CTRL_H__

// generate by ProtoGen at date: 2017/3/3 16:56:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2N_NOTIFYROUTERINFO2CTRL_TYPE 22364

class PtcM2N_NotifyRouterInfo2Ctrl : public CProtocol
{
public:
    explicit PtcM2N_NotifyRouterInfo2Ctrl() : CProtocol(PTCM2N_NOTIFYROUTERINFO2CTRL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2N_NotifyRouterInfo2Ctrl()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RouterListenInfo m_Data;
};

#endif