#ifndef __PTCG2C_NOTIFYWATCHICONNUM2CLIENT_H__
#define __PTCG2C_NOTIFYWATCHICONNUM2CLIENT_H__

// generate by ProtoGen at date: 2016/8/2 10:34:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYWATCHICONNUM2CLIENT_TYPE 48952

class PtcG2C_NotifyWatchIconNum2Client : public CProtocol
{
public:
    explicit PtcG2C_NotifyWatchIconNum2Client() : CProtocol(PTCG2C_NOTIFYWATCHICONNUM2CLIENT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyWatchIconNum2Client()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IconWatchListNum m_Data;
};

#endif