#ifndef __PTCG2M_SYNCLIVEINFOTOMS_H__
#define __PTCG2M_SYNCLIVEINFOTOMS_H__

// generate by ProtoGen at date: 2016/7/28 17:33:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNCLIVEINFOTOMS_TYPE 39515

class PtcG2M_SyncLiveInfoToMs : public CProtocol
{
public:
    explicit PtcG2M_SyncLiveInfoToMs() : CProtocol(PTCG2M_SYNCLIVEINFOTOMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SyncLiveInfoToMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncLiveInfoToMsData m_Data;
};

#endif