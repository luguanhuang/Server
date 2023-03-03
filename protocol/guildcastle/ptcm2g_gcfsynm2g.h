#ifndef __PTCM2G_GCFSYNM2G_H__
#define __PTCM2G_GCFSYNM2G_H__

// generate by ProtoGen at date: 2016/12/19 10:34:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_GCFSYNM2G_TYPE 13528

class PtcM2G_GCFSynM2G : public CProtocol
{
public:
    explicit PtcM2G_GCFSynM2G() : CProtocol(PTCM2G_GCFSYNM2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_GCFSynM2G()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GCFSynPara m_Data;
};

#endif