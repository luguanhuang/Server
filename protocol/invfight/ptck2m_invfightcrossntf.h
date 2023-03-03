#ifndef __PTCK2M_INVFIGHTCROSSNTF_H__
#define __PTCK2M_INVFIGHTCROSSNTF_H__

// generate by ProtoGen at date: 2017/4/15 15:32:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_INVFIGHTCROSSNTF_TYPE 25954

class PtcK2M_InvFightCrossNtf : public CProtocol
{
public:
    explicit PtcK2M_InvFightCrossNtf() : CProtocol(PTCK2M_INVFIGHTCROSSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_InvFightCrossNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightCrossNtfData m_Data;
};

#endif