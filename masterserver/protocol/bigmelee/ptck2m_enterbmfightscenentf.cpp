#include "pch.h"
#include "bigmelee/ptck2m_enterbmfightscenentf.h"
#include "scene/sceneswitch.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "mayhem/mayhemmgr.h"



// generate by ProtoGen at date: 2017/6/14 9:15:01

void PtcK2M_EnterBMFightSceneNtf::Process(UINT32 dwConnID)
{
	MayhemMsMgr::Instance()->AddCutScene(m_Data);
}
