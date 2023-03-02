#include "pch.h"
#include "action/ptcc2g_moveoperationreq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"
#include "timeutil.h"
#include "util/XCommon.h"
#include "component/XActionReceiver.h"
#include "LogInit.h"

// generate by ProtoGen at date: 2014/11/11 12:02:00

void PtcC2G_MoveOperationReq::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		bool inertia = (m_Data.common() & 0x80000000) > 0;
		float face = m_Data.common() & 0xFFFF;
		float velocity = ((m_Data.common() & 0x7FFF0000) >> 16);

		float posx = (m_Data.posxz() >> 16) / 100.0f;
		float posz = (m_Data.posxz() & 0xFFFF) / 100.0f;

		if(face < 0 || face > 360)
		{
			LogError("Role [%llu] report invalid face: %f", pRole->GetID(), face);
			return;
		}

		if(velocity < 0)
		{
			LogError("Role [%llu] report invalid velocity: %f", pRole->GetID(), velocity);
			return;
		}

		if(m_Data.has_desxz())
		{
			float desx = (m_Data.desxz() >> 16) / 100.0f;
			float desz = (m_Data.desxz() & 0xFFFF) / 100.0f;

			XActionReceiver::OnMoveReceived(pRole, velocity / 10.0f, face, posx, posz, desx, desz, inertia);
		}
		else
			XActionReceiver::OnMoveReceived(pRole, velocity / 10.0f, face, posx, posz, inertia);
	}
}