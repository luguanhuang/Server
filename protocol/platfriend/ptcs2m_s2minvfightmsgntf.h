#ifndef __PTCS2M_S2MINVFIGHTMSGNTF_H__
#define __PTCS2M_S2MINVFIGHTMSGNTF_H__

// generate by ProtoGen at date: 2017/4/14 19:31:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCS2M_S2MINVFIGHTMSGNTF_TYPE 53925

class PtcS2M_S2MInvFightMsgNtf : public CProtocol
{
public:
    explicit PtcS2M_S2MInvFightMsgNtf() : CProtocol(PTCS2M_S2MINVFIGHTMSGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcS2M_S2MInvFightMsgNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::S2MInvFightMsgNtfData m_Data;
};

#endif