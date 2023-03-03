#ifndef __PTCN2D_ONLINENUMNTF_H__
#define __PTCN2D_ONLINENUMNTF_H__

// generate by ProtoGen at date: 2016/11/3 10:24:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCN2D_ONLINENUMNTF_TYPE 37014

class PtcN2D_OnlineNumNtf : public CProtocol
{
public:
    explicit PtcN2D_OnlineNumNtf() : CProtocol(PTCN2D_ONLINENUMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcN2D_OnlineNumNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OnlineNumNtf m_Data;
};

#endif