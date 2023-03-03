#ifndef __PTCC2G_OPERATERECORDNTF_H__
#define __PTCC2G_OPERATERECORDNTF_H__

// generate by ProtoGen at date: 2016/10/27 10:53:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_OPERATERECORDNTF_TYPE 56173

class PtcC2G_OperateRecordNtf : public CProtocol
{
public:
    explicit PtcC2G_OperateRecordNtf() : CProtocol(PTCC2G_OPERATERECORDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_OperateRecordNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OperateRecord m_Data;
};

#endif