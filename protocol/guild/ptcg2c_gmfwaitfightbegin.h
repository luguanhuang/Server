#ifndef __PTCG2C_GMFWAITFIGHTBEGIN_H__
#define __PTCG2C_GMFWAITFIGHTBEGIN_H__

// generate by ProtoGen at date: 2016/8/2 0:34:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFWAITFIGHTBEGIN_TYPE 59721

class PtcG2C_GmfWaitFightBegin : public CProtocol
{
public:
    explicit PtcG2C_GmfWaitFightBegin() : CProtocol(PTCG2C_GMFWAITFIGHTBEGIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfWaitFightBegin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfWaitFightArg m_Data;
};

#endif