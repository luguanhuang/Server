#ifndef __PTCG2C_RECONNECTSYNCNOTIFY_H__
#define __PTCG2C_RECONNECTSYNCNOTIFY_H__

// generate by ProtoGen at date: 2016/6/16 14:25:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_RECONNECTSYNCNOTIFY_TYPE 42128

class PtcG2C_ReconnectSyncNotify : public CProtocol
{
public:
    explicit PtcG2C_ReconnectSyncNotify() : CProtocol(PTCG2C_RECONNECTSYNCNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ReconnectSyncNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ReconectSync m_Data;
};

#endif