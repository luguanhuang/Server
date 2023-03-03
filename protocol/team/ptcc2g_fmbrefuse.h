#ifndef __PTCC2G_FMBREFUSE_H__
#define __PTCC2G_FMBREFUSE_H__

// generate by ProtoGen at date: 2016/7/4 11:50:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_FMBREFUSE_TYPE 50821

class PtcC2G_FMBRefuse : public CProtocol
{
public:
    explicit PtcC2G_FMBRefuse() : CProtocol(PTCC2G_FMBREFUSE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_FMBRefuse()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMBRes m_Data;
};

#endif