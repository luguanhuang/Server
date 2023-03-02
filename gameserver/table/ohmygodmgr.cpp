#include "pch.h"
#include "ohmygodmgr.h"

bool OhMyGodMgr::Init()
{
	m_items.LoadFile("table/OhMyGod.txt");
	LogInfo("ohmygod item size: %u", m_items.Table.size());
	return true;
}

void OhMyGodMgr::Uninit()
{

}