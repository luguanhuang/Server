#include "pch.h"
#include "util/jsonutil.h"
#include "util/UtilEncode.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"


const rapidjson::Value& JsonUtil::GetJsonValue(const rapidjson::Value& roJson, const char* pcMember)
{
	static rapidjson::Value soEmpty;
	if(pcMember == NULL)
	{
		return soEmpty;
	}

	if(!roJson.HasMember(pcMember))
	{
		return soEmpty;
	}

	return roJson[pcMember];
}

INT32 JsonUtil::GetJsonInt(const rapidjson::Value& roJson, const char* pcMember)
{
	if(roJson.IsNull()) return 0;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return 0;

	if(!roMember.IsInt()) return 0;

	return roMember.GetInt();
}

UINT32 JsonUtil::GetJsonUint(const rapidjson::Value& roJson, const char* pcMember)
{
	if(roJson.IsNull()) return 0;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return 0;

	if(!roMember.IsUint()) return 0;

	return roMember.GetUint();
}

INT64 JsonUtil::GetJsonInt64(const rapidjson::Value& roJson, const char* pcMember)
{
	if(roJson.IsNull()) return 0;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return 0;

	if(!roMember.IsInt64()) return 0;

	return roMember.GetInt64();
}

UINT64 JsonUtil::GetJsonUint64(const rapidjson::Value& roJson, const char* pcMember)
{
	if(roJson.IsNull()) return 0;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return 0;

	if(!roMember.IsUint64()) return 0;

	return roMember.GetUint64();
}

double JsonUtil::GetJsonDouble(const rapidjson::Value& roJson, const char* pcMember)
{
	if(roJson.IsNull()) return 0.0;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return 0.0;

	if(!roMember.IsDouble()) return 0.0;

	return roMember.GetDouble();
}

std::string JsonUtil::GetJsonString(const rapidjson::Value& roJson, const char* pcMember, bool decode)
{
	if(roJson.IsNull()) return "";

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return "";

	if(!roMember.IsString()) return "";

	if (decode)
	{
		return UtilEncode::UrlDecode(roMember.GetString());
	}
	return roMember.GetString();
}

bool JsonUtil::GetJsonBool(const rapidjson::Value& roJson, const char* pcMember)
{
	if (roJson.IsNull()) return false;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return false;

	if (!roMember.IsBool()) return false;

	return roMember.GetBool();
}

void JsonUtil::GetJsonArrayUint(const rapidjson::Value& roJson, const char* pcMember, std::vector<UINT32>& vList)
{
	if(roJson.IsNull()) return ;

	const rapidjson::Value& roMember = GetJsonValue(roJson, pcMember);
	if(roMember.IsNull()) return ;

	if(!roMember.IsArray()) return ;

	for (UINT32 i = 0; i < roMember.Size(); i ++)
	{
		if (!roMember[i].IsUint())
		{
			return;
		}
		vList.push_back(roMember[i].GetUint());
	}
}


JsonBuilder::JsonBuilder()
:m_doc(rapidjson::kObjectType)
{
}

JsonBuilder::~JsonBuilder()
{
}

void JsonBuilder::AddMember_Int(const char* pcName, INT32 nVal)
{
	rapidjson::Value v(rapidjson::kNumberType);
	v.SetInt(nVal);
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator());
}

void JsonBuilder::AddMember_Uint(const char* pcName, UINT32 dwVal)
{
	rapidjson::Value v(rapidjson::kNumberType);
	v.SetUint(dwVal);
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator());
}

void JsonBuilder::AddMember_Int64(const char* pcName, INT64 llVal)
{
	rapidjson::Value v(rapidjson::kNumberType);
	v.SetInt64(llVal);
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator()); 
}

void JsonBuilder::AddMember_Uint64(const char* pcName, UINT64 qwVal)
{
	rapidjson::Value v(rapidjson::kNumberType);
	v.SetUint64(qwVal);
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator()); 
}

void JsonBuilder::AddMember_Double(const char* pcName, double dVal)
{
	rapidjson::Value v(rapidjson::kNumberType);
	v.SetDouble(dVal);
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator()); 
}

void JsonBuilder::AddMember_String(const char* pcName, const char* pcVal)
{
	rapidjson::Value v(rapidjson::kStringType);
	v.SetString(pcVal, Allocator());
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), v, Allocator()); 
}

void JsonBuilder::AddMember_String(const std::string& sName, const std::string& sValue) 
{
	rapidjson::Value v(rapidjson::kStringType);
	v.SetString(sValue.c_str(), sValue.length(), Allocator());
	m_doc.AddMember(rapidjson::Value(sName, Allocator()), v, Allocator()); 
}

void JsonBuilder::AddMember_Value(const char* pcName, rapidjson::Value& roVal)
{
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), roVal, Allocator()); 
}

void JsonBuilder::AddMember_Null(const char* pcName)
{
	m_doc.AddMember(rapidjson::Value(pcName, Allocator()), rapidjson::Value(rapidjson::kNullType), Allocator()); 
}

std::string JsonBuilder::GetString()
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_doc.Accept(writer);
	return buffer.GetString();
}