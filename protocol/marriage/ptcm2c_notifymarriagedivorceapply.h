#ifndef __PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_H__
#define __PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_H__

// generate by ProtoGen at date: 2017/7/27 10:19:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_TYPE 32886

class PtcM2C_NotifyMarriageDivorceApply : public CProtocol
{
public:
    explicit PtcM2C_NotifyMarriageDivorceApply() : CProtocol(PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyMarriageDivorceApply()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyMarriageDivorceApplyData m_Data;
};

#endif