#ifndef __PTCG2C_TAJIEHELPNOTIFY_H__
#define __PTCG2C_TAJIEHELPNOTIFY_H__

// generate by ProtoGen at date: 2017/7/11 19:43:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TAJIEHELPNOTIFY_TYPE 36521

class PtcG2C_TajieHelpNotify : public CProtocol
{
public:
    explicit PtcG2C_TajieHelpNotify() : CProtocol(PTCG2C_TAJIEHELPNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TajieHelpNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TajieHelpData m_Data;
};

#endif