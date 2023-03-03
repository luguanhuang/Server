#ifndef __PTCK2M_BATTLEFIELDREADYSCENENTF_H__
#define __PTCK2M_BATTLEFIELDREADYSCENENTF_H__

// generate by ProtoGen at date: 2017/9/6 12:55:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_BATTLEFIELDREADYSCENENTF_TYPE 48118

class PtcK2M_BattleFieldReadySceneNtf : public CProtocol
{
public:
    explicit PtcK2M_BattleFieldReadySceneNtf() : CProtocol(PTCK2M_BATTLEFIELDREADYSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_BattleFieldReadySceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldReadyScene m_Data;
};

#endif