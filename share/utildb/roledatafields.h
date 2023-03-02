#ifndef _H_Role_Data_Fields_H__
#define _H_Role_Data_Fields_H__

#include "pb/project.pb.h"
class CRoleDataFields
{
public:
	CRoleDataFields(KKSG::RoleAllInfo* poRoleInfo);
	~CRoleDataFields();

	UINT32 GetCount() const { return m_oFields.size(); }

	std::string GetFieldName(UINT32 dwIndex) const;
	google::protobuf::MessageLite* GetField(UINT32 dwIndex) const { return dwIndex < m_oFields.size() ? m_oFields[dwIndex] : NULL; }


	//helper func
	static std::string GetDataTypeName(const google::protobuf::MessageLite* poDataField);
private:
	typedef std::vector<google::protobuf::MessageLite*>	CFieldList;
	CFieldList				m_oFields;
};


class CRoleDataFieldCleanup
{
	CRoleDataFieldCleanup();
	~CRoleDataFieldCleanup();

	CRoleDataFieldCleanup(const CRoleDataFieldCleanup&);
	CRoleDataFieldCleanup& operator = (const CRoleDataFieldCleanup&);
public:

	static CRoleDataFieldCleanup& Instance();
	bool ClearField(KKSG::RoleAllInfo* poRoleInfo, const google::protobuf::MessageLite* poField);
private:
	typedef void (KKSG::RoleAllInfo::*PField_ClearFunc)();
	void RegisterFunc(const google::protobuf::MessageLite* poField, PField_ClearFunc pFunc);

	typedef std::unordered_map<std::string, PField_ClearFunc>	CFieldClearFuncMap;
	CFieldClearFuncMap	m_oFuncMap;
};


class CRoleDataSaveUtil
{
public:
	//save all
	static bool SaveAll(const CRoleDataFields& roFields, KKSG::SaveRoleDataArg& roArg);
	static bool SaveField(const google::protobuf::MessageLite* poDataField, KKSG::SaveRoleDataArg& roArg);
};

#endif