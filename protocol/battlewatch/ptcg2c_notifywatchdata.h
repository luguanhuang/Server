#ifndef __PTCG2C_NOTIFYWATCHDATA_H__
#define __PTCG2C_NOTIFYWATCHDATA_H__

// generate by ProtoGen at date: 2016/7/23 15:59:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYWATCHDATA_TYPE 16154

class PtcG2C_NotifyWatchData : public CProtocol
{
public:
    explicit PtcG2C_NotifyWatchData() : CProtocol(PTCG2C_NOTIFYWATCHDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyWatchData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OneLiveRecordInfo m_Data;
};

#endif