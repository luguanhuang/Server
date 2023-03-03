#ifndef __PTCG2C_LOGINREWARD2CNTF_H__
#define __PTCG2C_LOGINREWARD2CNTF_H__

// generate by ProtoGen at date: 2016/8/11 15:13:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LOGINREWARD2CNTF_TYPE 51966

class PtcG2C_LoginReward2CNtf : public CProtocol
{
public:
    explicit PtcG2C_LoginReward2CNtf() : CProtocol(PTCG2C_LOGINREWARD2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LoginReward2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LoginRewardRet m_Data;
};

#endif