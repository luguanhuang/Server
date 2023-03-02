#ifndef _GUILD_BIND_GROUP_HELPER_H_
#define _GUILD_BIND_GROUP_HELPER_H_

#include <string>

class QQGroupTaskBase;
class BindGroupHelper
{
public:
	static void OnCreateBindSuccess(QQGroupTaskBase* task, const std::string& groupCode, const std::string& groupName);	
	static void OnCreateBindFailed(QQGroupTaskBase* task, INT32 code);

	static void OnGetRelationSuccess(QQGroupTaskBase* task, UINT32 relation);
	static void OnGetRelationFailed(QQGroupTaskBase* task, INT32 code);

	static void OnJoinGroupSuccess(QQGroupTaskBase* task);
	static void OnJoinGroupFailed(QQGroupTaskBase* task, INT32 code, INT32 platCode);

	static void OnUnlinkGroupSuccess(QQGroupTaskBase* task, const std::string& groupCode);
	static void OnUnlinkGroupFailed(QQGroupTaskBase* task, INT32 code);

	static void OnGetGroupDetailSuccess(QQGroupTaskBase* task, const std::string& groupName, const std::string& groupOpenId);
	static void OnGetGroupDetailFailed(QQGroupTaskBase* task, INT32 code, INT32 platCode);
};

#endif