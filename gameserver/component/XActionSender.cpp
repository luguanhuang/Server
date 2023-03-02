#include "pch.h"
#include "XActionSender.h"
#include "scene/scene.h"
#include "scene/linehandler.h"
#include "scene/sceneline.h"
#include "broadcast/broadcast.h"

using namespace KKSG;

PtcG2C_SyncStepNotify XActionSender::sm_oPtc;

XActionSender::XActionSender()
{
	Recycle();
}

XActionSender::~XActionSender()
{
	Recycle();
}

void XActionSender::Broadcast(const std::vector<Role*>& roles)
{
	if (!m_ischanged) return;

	/*for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		role->Send(sm_oPtc);
	}*/

	//替换新广播接口
	CBroadCastSender::Broadcast(roles, sm_oPtc);
}

void XActionSender::Broadcast(const std::unordered_map<UINT64, RoleViewSet*>& roles)
{
	if (!m_ischanged) return;

	/*for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = i->second->m_role;
		role->Send(sm_oPtc);
	}*/

	//替换新广播接口
	CBroadCastSender::Broadcast(roles, sm_oPtc);
}

void XActionSender::Broadcast(const std::unordered_map<UINT64, Role*>& roles)
{
	if (!m_ischanged) return;

	/*for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = i->second;
		role->Send(sm_oPtc);
	}*/

	//替换新广播接口
	CBroadCastSender::Broadcast(roles, sm_oPtc);
}

void XActionSender::PackageData(const std::unordered_map<UINT64, Unit*>& units)
{
	std::unordered_map<UINT64, Unit*>::const_iterator it = units.begin();
	while(it != units.end())
	{
		m_ischanged |= Package(it->second->GetXObject());
		++it;
	}
}

void XActionSender::PackageData(const std::unordered_map<UINT64, RoleViewSet*>& roles)
{
	std::unordered_map<UINT64, RoleViewSet*>::const_iterator it = roles.begin();
	while(it != roles.end())
	{
		m_ischanged |= Package(it->second->m_role->GetXObject());
		++it;
	}
}

void XActionSender::PackageData(Role* roles[], int count, Role* self)
{
	for (int i = 0; i < count; ++i)
	{
		Role* role = roles[i];
		if(role) m_ischanged |= Package(role->GetXObject());
	}

	if (NULL != self)
	{
		m_ischanged |= Package(self->GetXObject());
	}
}

void XActionSender::PackageData(const std::list<Role*>& roles, Role* self)
{
	std::list<Role*>::const_iterator it = roles.begin();
	while(it != roles.end())
	{
		m_ischanged |= Package((*it)->GetXObject());
		++it;
	}
	if (NULL != self)
	{
		m_ischanged |= Package(self->GetXObject());
	}
}
