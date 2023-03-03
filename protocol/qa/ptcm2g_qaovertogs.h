#ifndef __PTCM2G_QAOVERTOGS_H__
#define __PTCM2G_QAOVERTOGS_H__

// generate by ProtoGen at date: 2016/10/11 11:03:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_QAOVERTOGS_TYPE 31351

class PtcM2G_QAOverToGS : public CProtocol
{
public:
    explicit PtcM2G_QAOverToGS() : CProtocol(PTCM2G_QAOVERTOGS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_QAOverToGS()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QAOverToGS m_Data;
};

#endif