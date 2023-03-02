#include "pch.h"
#include <typeinfo>
#include "ainode.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "component/XAIComponent.h"
#include "aibehaviortree.h"
#include "aitreemgr.h"
#include "util.h"
#include "ainodefactory.h"
#include "aionlinedebugmgr.h"

#define MAP_VALUE_SET(Type,type)\
	void SharedData::Set##Type##ByIndex(UINT32 index, type para)\
	{\
		if (index < 0 || index >= MAX_COUNT_NUM)\
			return;\
		m_##type##sMap[index] = para;\
	}

MAP_VALUE_SET(Float,float)
MAP_VALUE_SET(Int,int)
MAP_VALUE_SET(Bool,bool)

SharedData::SharedData()
{
	for (int i=0; i<MAX_COUNT_NUM; i++)
	{
		m_transMap[i] = NULL;
		m_floatsMap[i] = 0;
		m_intsMap[i] = 0;
		m_stringsMap[i] = "";
		m_boolsMap[i] = false;
		m_vector3sMap[i] = Vector3::zero;
	}
}

void SharedData::SetVector3ByIndex(UINT32 index , const Vector3& para)
{
	if (index < 0 || index >= MAX_COUNT_NUM)
		return;

	m_vector3sMap[index] = para;
}

void SharedData::SetTransformByIndex( UINT32 index, Transform para )
{
	if (index < 0 || index >= MAX_COUNT_NUM)
		return;
	m_transMap[index] = para;

}

void SharedData::SetStringByIndex( UINT32 index, const std::string& para )
{
	if (index < 0 || index >= MAX_COUNT_NUM)
		return;

	m_stringsMap[index] = para;
}

float SharedData::GetFloatByIndex(UINT32 index, float value)
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return value;
	}

	return m_floatsMap[index];
}

bool SharedData::GetBoolByIndex(UINT32 index, bool value)
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return value;
	}
	return m_boolsMap[index];
}

int SharedData::GetIntByIndex(UINT32 index, int value)
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return value;
	}
	return m_intsMap[index];
}

Transform SharedData::GetTransformByIndex( UINT32 index )
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return NULL;
	}
	return m_transMap[index];
}


const std::string& SharedData::GetStringByIndex( UINT32 index , const std::string& dvalue/*=""*/ )
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return dvalue;
	}
	if (m_stringsMap[index] != "")
		return m_stringsMap[index];
	else
		return dvalue;
}

Vector3 SharedData::GetVector3ByIndex(UINT32 index, const Vector3& dvalue /*= Vector3::zero*/)
{
	if (index < 0 || index >= MAX_COUNT_NUM)
	{
		return dvalue;
	}
	return m_vector3sMap[index];
}

void SharedData::ParseVector3FromString(const std::string& strvalue, Vector3& inputVec)
{
	const std::vector<std::string>& svec = Split(strvalue , ":");
	if(3 == (int)svec.size())
	{
		inputVec.x = (float)std::atof(svec[0].c_str());
		inputVec.y = (float)std::atof(svec[1].c_str());
		inputVec.z = (float)std::atof(svec[2].c_str());
	}
}

void SharedData::Print()
{
	SSInfo << typeid(*this).name() << " shared data: " <<END;

	//for(auto it = m_transMap.begin(); it != m_transMap.end(); ++it)
	//{
	//	SSInfo << "Trans: " << it->first <<END;
	//}
	//
	//for(auto it = m_stringsMap.begin(); it != m_stringsMap.end(); ++it)
	//{
	//	SSInfo << "String: " << it->first << " = " << it->second <<END;
	//}

	//for(auto it = m_intsMap.begin(); it != m_intsMap.end(); ++it)
	//{
	//	SSInfo << "Int: " << it->first << " = " <<it->second <<END;
	//}

	//for(auto it = m_floatsMap.begin(); it != m_floatsMap.end(); ++it)
	//{
	//	SSInfo << "Float: " << it->first << " = " <<it->second <<END;
	//}

	//for(auto it = m_boolsMap.begin(); it != m_boolsMap.end(); ++it)
	//{
	//	SSInfo << "Bool: " << it->first << " = " <<it->second <<END;
	//}

}

const std::string SharedData::_DefaultString = "";

void AINodeBase::Print()
{
	//Debug สนำร
	SSInfo<< typeid(*this).name() <<END;
}

void AINodeBase::PostCreate(const tinyxml2::XMLElement* elem)
{
#ifdef AIDEBUG
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("IsBreakPoint");
	if (attr) attr->QueryBoolValue(&m_bBreakPoint);

	attr = elem->FindAttribute("FriendlyName");
	if (attr)
		m_strFriendlyName = attr->Value();
	else
	{
		m_strFriendlyName = typeid(*this).name();
		int pos = m_strFriendlyName.find("class ");
		if (pos >= 0)
			m_strFriendlyName = m_strFriendlyName.replace(pos, 6, "");
	}
#endif
}

RootNode::~RootNode()
{
	delete m_child;
	m_child = NULL;
	delete m_sharedData;
	m_sharedData = NULL;
}

void RootNode::AddChild(AINodeBase* child)
{
	m_child = child;
}

bool RootNode::Update( Unit* unit )
{
	if(m_child)
	{
		//return m_child->Update(unit);
		AIRETURN("RootNode", m_child->Update(unit));
	}
	//return true;
	AIRETURN("RootNode", true);
}

void RootNode::Print()
{
	AINodeBase::Print();

	if(m_sharedData)
	{
		m_sharedData->Print();
	}

	if(m_child)
	{
		m_child->Print();
	}
}

void RootNode::PrintTree()
{
	AINodeBase::Print();

	if(m_child)
	{
		m_child->Print();
	}
}

EntryTaskNode::EntryTaskNode( const tinyxml2::XMLElement* elem )
{
	if(m_sharedData && elem)
	{
		
		const tinyxml2::XMLAttribute* attr = elem->FirstAttribute();
		while(attr)
		{
			std::string name(attr->Name());
			if(name.size() >= 3)
			{
				std::string realName(name.begin()+2,name.end());
				UINT32 realNameIndex = AINodeFactory::CreateIndexByName(realName.c_str());
				if('F' == name[0])//float
				{
					m_sharedData->SetFloatByIndex(realNameIndex , attr->FloatValue());
				}
				else if('S' == name[0])//string
				{
					m_sharedData->SetStringByIndex(realNameIndex , attr->Value());
				}
				else if('I' == name[0])//int
				{
					m_sharedData->SetIntByIndex(realNameIndex , attr->IntValue());
				}else if('B' == name[0])//bool
				{
					m_sharedData->SetBoolByIndex(realNameIndex , attr->BoolValue());
				}else if('T' == name[0])//transform(Unit*)
				{
					m_sharedData->SetTransformByIndex(realNameIndex , NULL);
				}else if('V' == name[0])//Vector3
				{
					const std::vector<std::string>& svec = Split(attr->Value() , ":");
					if(3 == (int)svec.size())
					{
						Vector3 vtmp;
						vtmp.x = (float)std::atof(svec[0].c_str());
						vtmp.y = (float)std::atof(svec[1].c_str());
						vtmp.z = (float)std::atof(svec[2].c_str());
						m_sharedData->SetVector3ByIndex(realNameIndex , vtmp);
					}
				}
			}
			attr = attr->Next();
		}

	}
}


SharedData* GetSharedData( Unit* unit )
{
	return unit->AIData();
}
