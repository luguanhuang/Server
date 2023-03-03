#ifndef __PTCM2G_NOTIFYWATCHICONNUM_H__
#define __PTCM2G_NOTIFYWATCHICONNUM_H__

// generate by ProtoGen at date: 2016/8/2 10:33:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTIFYWATCHICONNUM_TYPE 12471

class PtcM2G_NotifyWatchIconNum : public CProtocol
{
public:
    explicit PtcM2G_NotifyWatchIconNum() : CProtocol(PTCM2G_NOTIFYWATCHICONNUM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NotifyWatchIconNum()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IconWatchListNum m_Data;
};

#endif