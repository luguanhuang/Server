#ifndef _H_Log_Util_H__
#define _H_Log_Util_H__


inline std::string Uint64List2Str(const google::protobuf::RepeatedField<google::protobuf::uint64>& roUint64List)
{
	std::stringstream ss;
	for(INT32 i = 0; i < roUint64List.size(); ++i)
	{
		if(i > 0) ss << ", ";
		ss << roUint64List.Get(i);
	}
	return ss.str();
}

inline std::string RoleInfoList2Str(const google::protobuf::RepeatedPtrField<KKSG::RoleAllInfo>& roRoleInfoList)
{
	std::stringstream ss;
	for(INT32 i = 0; i < roRoleInfoList.size(); ++i)
	{
		if(i > 0) ss << ", ";
		ss << roRoleInfoList.Get(i).brief().roleid();
	}
	return ss.str();
}

inline const char* GetGsPrefix(bool isCrossGs)
{
	return isCrossGs ? "cross_gs" : "gs";
}

#endif
