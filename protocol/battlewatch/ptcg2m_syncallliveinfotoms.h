#ifndef __PTCG2M_SYNCALLLIVEINFOTOMS_H__
#define __PTCG2M_SYNCALLLIVEINFOTOMS_H__

// generate by ProtoGen at date: 2016/11/24 21:20:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNCALLLIVEINFOTOMS_TYPE 34839

class PtcG2M_SyncAllLiveInfoToMs : public CProtocol
{
public:
    explicit PtcG2M_SyncAllLiveInfoToMs() : CProtocol(PTCG2M_SYNCALLLIVEINFOTOMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SyncAllLiveInfoToMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncAllLiveInfoToMSData m_Data;
};

#endif