#ifndef __PTCM2D_NOTIFYROUTERINFO2DB_H__
#define __PTCM2D_NOTIFYROUTERINFO2DB_H__

// generate by ProtoGen at date: 2017/3/3 16:55:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_NOTIFYROUTERINFO2DB_TYPE 26305

class PtcM2D_NotifyRouterInfo2DB : public CProtocol
{
public:
    explicit PtcM2D_NotifyRouterInfo2DB() : CProtocol(PTCM2D_NOTIFYROUTERINFO2DB_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_NotifyRouterInfo2DB()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RouterListenInfo m_Data;
};

#endif