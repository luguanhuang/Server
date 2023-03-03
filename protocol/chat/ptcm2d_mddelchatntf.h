#ifndef __PTCM2D_MDDELCHATNTF_H__
#define __PTCM2D_MDDELCHATNTF_H__

// generate by ProtoGen at date: 2016/12/14 0:58:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_MDDELCHATNTF_TYPE 56562

class PtcM2D_MDDelChatNtf : public CProtocol
{
public:
    explicit PtcM2D_MDDelChatNtf() : CProtocol(PTCM2D_MDDELCHATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_MDDelChatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MDDelChatData m_Data;
};

#endif