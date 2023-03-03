#ifndef __PTCM2C_NOTICEGUILDTERRBATTLEWIN_H__
#define __PTCM2C_NOTICEGUILDTERRBATTLEWIN_H__

// generate by ProtoGen at date: 2017/2/9 11:07:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDTERRBATTLEWIN_TYPE 61655

class PtcM2C_NoticeGuildTerrBattleWin : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildTerrBattleWin() : CProtocol(PTCM2C_NOTICEGUILDTERRBATTLEWIN_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildTerrBattleWin()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildTerrBattleWin m_Data;
};

#endif