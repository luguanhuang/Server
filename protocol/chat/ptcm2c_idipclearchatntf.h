#ifndef __PTCM2C_IDIPCLEARCHATNTF_H__
#define __PTCM2C_IDIPCLEARCHATNTF_H__

// generate by ProtoGen at date: 2016/9/29 20:17:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_IDIPCLEARCHATNTF_TYPE 47934

class PtcM2C_IdipClearChatNtf : public CProtocol
{
public:
    explicit PtcM2C_IdipClearChatNtf() : CProtocol(PTCM2C_IDIPCLEARCHATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_IdipClearChatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::IdipClearChatInfo m_Data;
};

#endif