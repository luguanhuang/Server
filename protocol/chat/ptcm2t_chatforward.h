#ifndef __PTCM2T_CHATFORWARD_H__
#define __PTCM2T_CHATFORWARD_H__

// generate by ProtoGen at date: 2016/7/25 15:31:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2T_CHATFORWARD_TYPE 59684

class PtcM2T_chatforward : public CProtocol
{
public:
    explicit PtcM2T_chatforward() : CProtocol(PTCM2T_CHATFORWARD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2T_chatforward()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChatForward m_Data;
};

#endif