#ifndef __PTCK2H_WORLDMASTERINFONTF_H__
#define __PTCK2H_WORLDMASTERINFONTF_H__

// generate by ProtoGen at date: 2017/5/21 20:13:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2H_WORLDMASTERINFONTF_TYPE 62412

class PtcK2H_WorldMasterInfoNtf : public CProtocol
{
public:
    explicit PtcK2H_WorldMasterInfoNtf() : CProtocol(PTCK2H_WORLDMASTERINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2H_WorldMasterInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::W2TMasterInfo m_Data;
};

#endif