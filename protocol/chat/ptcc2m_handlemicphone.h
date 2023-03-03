#ifndef __PTCC2M_HANDLEMICPHONE_H__
#define __PTCC2M_HANDLEMICPHONE_H__

// generate by ProtoGen at date: 2016/11/29 16:36:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_HANDLEMICPHONE_TYPE 50175

class PtcC2M_HandleMicphone : public CProtocol
{
public:
    explicit PtcC2M_HandleMicphone() : CProtocol(PTCC2M_HANDLEMICPHONE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_HandleMicphone()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HandleMicphoneArg m_Data;
};

#endif