#ifndef __PTCK2M_SYNCROLESUMMARYTOMS_H__
#define __PTCK2M_SYNCROLESUMMARYTOMS_H__

// generate by ProtoGen at date: 2017/2/2 22:16:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_SYNCROLESUMMARYTOMS_TYPE 57915

class PtcK2M_SyncRoleSummaryToMS : public CProtocol
{
public:
    explicit PtcK2M_SyncRoleSummaryToMS() : CProtocol(PTCK2M_SYNCROLESUMMARYTOMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_SyncRoleSummaryToMS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SyncRoleSummaryToMS m_Data;
};

#endif