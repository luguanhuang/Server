#ifndef __PTCM2G_WEDDINGCARENDM2G_H__
#define __PTCM2G_WEDDINGCARENDM2G_H__

// generate by ProtoGen at date: 2017/7/31 23:37:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_WEDDINGCARENDM2G_TYPE 42520

class PtcM2G_WeddingCarEndM2G : public CProtocol
{
public:
    explicit PtcM2G_WeddingCarEndM2G() : CProtocol(PTCM2G_WEDDINGCARENDM2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_WeddingCarEndM2G()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingCarEndM2G m_Data;
};

#endif