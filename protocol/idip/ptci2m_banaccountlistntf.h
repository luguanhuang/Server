#ifndef __PTCI2M_BANACCOUNTLISTNTF_H__
#define __PTCI2M_BANACCOUNTLISTNTF_H__

// generate by ProtoGen at date: 2017/4/10 22:25:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCI2M_BANACCOUNTLISTNTF_TYPE 46748

class PtcI2M_BanAccountListNtf : public CProtocol
{
public:
    explicit PtcI2M_BanAccountListNtf() : CProtocol(PTCI2M_BANACCOUNTLISTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcI2M_BanAccountListNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BanAccountList m_Data;
};

#endif