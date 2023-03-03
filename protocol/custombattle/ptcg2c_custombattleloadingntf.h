#ifndef __PTCG2C_CUSTOMBATTLELOADINGNTF_H__
#define __PTCG2C_CUSTOMBATTLELOADINGNTF_H__

// generate by ProtoGen at date: 2017/5/14 22:28:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CUSTOMBATTLELOADINGNTF_TYPE 34402

class PtcG2C_CustomBattleLoadingNtf : public CProtocol
{
public:
    explicit PtcG2C_CustomBattleLoadingNtf() : CProtocol(PTCG2C_CUSTOMBATTLELOADINGNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CustomBattleLoadingNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CustomBattleLoadingNtf m_Data;
};

#endif