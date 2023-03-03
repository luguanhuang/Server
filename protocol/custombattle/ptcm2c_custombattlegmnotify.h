#ifndef __PTCM2C_CUSTOMBATTLEGMNOTIFY_H__
#define __PTCM2C_CUSTOMBATTLEGMNOTIFY_H__

// generate by ProtoGen at date: 2017/5/15 20:41:12

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_CUSTOMBATTLEGMNOTIFY_TYPE 65108

class PtcM2C_CustomBattleGMNotify : public CProtocol
{
public:
    explicit PtcM2C_CustomBattleGMNotify() : CProtocol(PTCM2C_CUSTOMBATTLEGMNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_CustomBattleGMNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CustomBattleGMNotify m_Data;
};

#endif