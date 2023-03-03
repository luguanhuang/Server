#ifndef __PTCC2G_QTEOPERATION_H__
#define __PTCC2G_QTEOPERATION_H__

// generate by ProtoGen at date: 2016/2/25 14:13:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_QTEOPERATION_TYPE 11413

class PtcC2G_QTEOperation : public CProtocol
{
public:
    explicit PtcC2G_QTEOperation() : CProtocol(PTCC2G_QTEOPERATION_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_QTEOperation()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QTEOperation m_Data;
};

#endif