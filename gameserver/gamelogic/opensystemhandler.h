#ifndef __OPENSYSTEMHANDLER_H__
#define __OPENSYSTEMHANDLER_H__

#include "roleeventlistener.h"

class OpenSystemHandler : public RoleEventListener
{
public:
	static OpenSystemHandler GlobaHandler;

	virtual void OnLevelup(Role *poRole);
	virtual void OnTaskComplete(Role *poRole, int TaskID);

	bool DoOpenSystem(Role* role, const std::vector<int> &systemids);

	void OnSystemOpened(Role* role, INT32 nSystemID);
};

#endif // __OPENSYSTEMHANDLER_H__