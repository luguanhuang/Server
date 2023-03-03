#ifndef __PTCG2C_ANTIADDICTIONREMINDNTF_H__
#define __PTCG2C_ANTIADDICTIONREMINDNTF_H__

// generate by ProtoGen at date: 2016/12/6 19:34:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ANTIADDICTIONREMINDNTF_TYPE 17999

class PtcG2C_AntiAddictionRemindNtf : public CProtocol
{
public:
    explicit PtcG2C_AntiAddictionRemindNtf() : CProtocol(PTCG2C_ANTIADDICTIONREMINDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_AntiAddictionRemindNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AntiAddictionRemindInfo m_Data;
};

#endif