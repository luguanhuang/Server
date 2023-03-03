#ifndef __PTCM2G_BALANCEINFONTF_H__
#define __PTCM2G_BALANCEINFONTF_H__

// generate by ProtoGen at date: 2016/12/21 23:21:07

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_BALANCEINFONTF_TYPE 11806

class PtcM2G_BalanceInfoNtf : public CProtocol
{
public:
    explicit PtcM2G_BalanceInfoNtf() : CProtocol(PTCM2G_BALANCEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_BalanceInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BalanceInfo m_Data;
};

#endif