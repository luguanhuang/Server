#ifndef __PTCM2C_TASKREFRESHNTF_H__
#define __PTCM2C_TASKREFRESHNTF_H__

// generate by ProtoGen at date: 2017/10/12 14:11:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TASKREFRESHNTF_TYPE 40464

class PtcM2C_TaskRefreshNtf : public CProtocol
{
public:
    explicit PtcM2C_TaskRefreshNtf() : CProtocol(PTCM2C_TASKREFRESHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TaskRefreshNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TaskRefreshNtf m_Data;
};

#endif