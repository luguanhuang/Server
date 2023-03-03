#ifndef __PTCM2K_TRANSCUSTOMBATTLEMATCH_H__
#define __PTCM2K_TRANSCUSTOMBATTLEMATCH_H__

// generate by ProtoGen at date: 2017/5/9 22:11:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_TRANSCUSTOMBATTLEMATCH_TYPE 53445

class PtcM2K_TransCustomBattleMatch : public CProtocol
{
public:
    explicit PtcM2K_TransCustomBattleMatch() : CProtocol(PTCM2K_TRANSCUSTOMBATTLEMATCH_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_TransCustomBattleMatch()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TransCustomBattleMatch m_Data;
};

#endif