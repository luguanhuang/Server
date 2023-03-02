#include "pch.h"
#include "common/ptci2d_registloginserver.h"
#include "common/ptci2p_registerloginntf.h"
#include "common/ptci2d_changeserverstatentf.h"
#include "server/ptci2m_serverinfontf.h"
#include "server/ptci2k_serverinfontf2world.h"
#include "idip/ptci2m_banaccountlistntf.h"
#include "idip/ptci2m_delbanaccountntf.h"

void PtcI2D_RegistLoginServer::Process(UINT32 dwConnID) {}
void PtcI2P_RegisterLoginNtf::Process(UINT32 dwConnID) {}
void PtcI2D_ChangeServerStateNtf::Process(UINT32 dwConnID) {}
void PtcI2M_ServerInfoNtf::Process(UINT32 dwConnID) {}
void PtcI2K_ServerInfoNtf2World::Process(UINT32 dwConnID) {}
void PtcI2M_BanAccountListNtf::Process(UINT32 dwConnID) {}
void PtcI2M_DelBanAccountNtf::Process(UINT32 dwConnID) {}

