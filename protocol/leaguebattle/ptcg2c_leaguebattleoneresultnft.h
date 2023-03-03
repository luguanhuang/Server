#ifndef __PTCG2C_LEAGUEBATTLEONERESULTNFT_H__
#define __PTCG2C_LEAGUEBATTLEONERESULTNFT_H__

// generate by ProtoGen at date: 2017/1/9 7:58:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEAGUEBATTLEONERESULTNFT_TYPE 40599

class PtcG2C_LeagueBattleOneResultNft : public CProtocol
{
public:
    explicit PtcG2C_LeagueBattleOneResultNft() : CProtocol(PTCG2C_LEAGUEBATTLEONERESULTNFT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LeagueBattleOneResultNft()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueBattleOneResultNtf m_Data;
};

#endif