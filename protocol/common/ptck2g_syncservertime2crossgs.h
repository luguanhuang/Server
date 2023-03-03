#ifndef __PTCK2G_SYNCSERVERTIME2CROSSGS_H__
#define __PTCK2G_SYNCSERVERTIME2CROSSGS_H__

// generate by ProtoGen at date: 2017/2/5 16:53:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2G_SYNCSERVERTIME2CROSSGS_TYPE 36075

class PtcK2G_SyncServerTime2CrossGs : public CProtocol
{
public:
    explicit PtcK2G_SyncServerTime2CrossGs() : CProtocol(PTCK2G_SYNCSERVERTIME2CROSSGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2G_SyncServerTime2CrossGs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncServerTime m_Data;
};

#endif