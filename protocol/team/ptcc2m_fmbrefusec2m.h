#ifndef __PTCC2M_FMBREFUSEC2M_H__
#define __PTCC2M_FMBREFUSEC2M_H__

// generate by ProtoGen at date: 2016/8/19 15:52:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_FMBREFUSEC2M_TYPE 44407

class PtcC2M_FMBRefuseC2M : public CProtocol
{
public:
    explicit PtcC2M_FMBRefuseC2M() : CProtocol(PTCC2M_FMBREFUSEC2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_FMBRefuseC2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMBRes m_Data;
};

#endif