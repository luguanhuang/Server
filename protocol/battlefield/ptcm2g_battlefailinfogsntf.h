#ifndef __PTCM2G_BATTLEFAILINFOGSNTF_H__
#define __PTCM2G_BATTLEFAILINFOGSNTF_H__

// generate by ProtoGen at date: 2017/9/12 15:23:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_BATTLEFAILINFOGSNTF_TYPE 33023

class PtcM2G_BattleFailInfoGsNtf : public CProtocol
{
public:
    explicit PtcM2G_BattleFailInfoGsNtf() : CProtocol(PTCM2G_BATTLEFAILINFOGSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_BattleFailInfoGsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFailInfo m_Data;
};

#endif