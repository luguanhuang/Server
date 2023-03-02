#ifndef _H_Json_Util_H__
#define _H_Json_Util_H__

#include "rapidjson/document.h"


class JsonUtil
{
public:
	static const rapidjson::Value& GetJsonValue(const rapidjson::Value& roJson, const char* pcMember);


	static INT32		GetJsonInt(const rapidjson::Value& roJson, const char* pcMember);
	static UINT32		GetJsonUint(const rapidjson::Value& roJson, const char* pcMember);
	static INT64		GetJsonInt64(const rapidjson::Value& roJson, const char* pcMember);
	static UINT64		GetJsonUint64(const rapidjson::Value& roJson, const char* pcMember);

	static double		GetJsonDouble(const rapidjson::Value& roJson, const char* pcMember);
	static std::string	GetJsonString(const rapidjson::Value& roJson, const char* pcMember, bool decode = true);
	static bool			GetJsonBool(const rapidjson::Value& roJson, const char* pcMember);
	static void			GetJsonArrayUint(const rapidjson::Value& roJson, const char* pcMember, std::vector<UINT32>& vList);
};


class JsonBuilder
{
public:
	JsonBuilder();
	~JsonBuilder();
	
	void AddMember_Int(const char* pcName, INT32 nVal);
	void AddMember_Uint(const char* pcName, UINT32 dwVal);
	void AddMember_Int64(const char* pcName, INT64 llVal);
	void AddMember_Uint64(const char* pcName, UINT64 qwVal);
	void AddMember_Double(const char* pcName, double dVal);
	void AddMember_String(const char* pcName, const char* pcVal);
	void AddMember_String(const std::string& sName, const std::string& sValue);
	void AddMember_Value(const char* pcName, rapidjson::Value& roVal);		//×Ô¶¨ÒåValue
	void AddMember_Null(const char* pcName);

	std::string GetString();
	rapidjson::Document::AllocatorType& Allocator() { return m_doc.GetAllocator(); }
private:
	rapidjson::Document	m_doc; 
};

#endif