#ifndef __PTCK2M_BATTLEFAILINFOMSNTF_H__
#define __PTCK2M_BATTLEFAILINFOMSNTF_H__

// generate by ProtoGen at date: 2017/9/12 15:23:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_BATTLEFAILINFOMSNTF_TYPE 27457

class PtcK2M_BattleFailInfoMsNtf : public CProtocol
{
public:
    explicit PtcK2M_BattleFailInfoMsNtf() : CProtocol(PTCK2M_BATTLEFAILINFOMSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_BattleFailInfoMsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFailInfo m_Data;
};

#endif