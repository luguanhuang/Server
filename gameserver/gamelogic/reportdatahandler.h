#ifndef __REPORTDATA_HANDLER_H__
#define __REPORTDATA_HANDLER_H__

#include "roleeventlistener.h"
class ReportDataHandler : public RoleEventListener
{
public:
	static ReportDataHandler GlobalHandler;
	virtual void OnLevelup(Role *poRole);	
	virtual void OnLogin(Role* poRole);
	virtual void OnChangeVirtualItem(Role* poRole, UINT32 item, INT64 count);
	virtual void OnPay(Role* poRole, UINT32 totalPay, UINT32 PayNum);
	virtual void OnChangeVip(Role* poRole);
};
#endif
