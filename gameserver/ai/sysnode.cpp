#include "pch.h"
#include "sysnode.h"
#include "util/XRandom.h"
#include "util/XCommon.h"
#include "ainodefactory.h"
#include "aionlinedebugmgr.h"

RandomFloat::RandomFloat( const tinyxml2::XMLElement* elem )
{
	m_minValue = 0.0;
	m_maxValue = 0.0;
	m_inclusive = true;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_MinName");
		if(attr)
		{
			m_sharedMinName = attr->Value();

			if (m_sharedMinName != "")
				m_MinIndex = AINodeFactory::CreateIndexByName(m_sharedMinName.c_str());
			else
				m_MinIndex = -1;
		}
		attr = elem->FindAttribute("minValue");
		if(attr)
		{
			m_minValue = attr->FloatValue();
		}
		attr = elem->FindAttribute("Shared_MaxName");
		if(attr)
		{
			m_sharedMaxName = attr->Value();

			if (m_sharedMaxName != "")
				m_MaxIndex = AINodeFactory::CreateIndexByName(m_sharedMaxName.c_str());
			else
				m_MaxIndex = -1;
		}
		attr = elem->FindAttribute("maxValue");
		if(attr)
		{
			m_maxValue = attr->FloatValue();
		}
		attr = elem->FindAttribute("inclusive");
		if(attr)
		{
			m_inclusive = attr->BoolValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		m_ResultIndex = -1;
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_ResultIndex = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
		}
	}
}

bool RandomFloat::Update( Unit* unit )
{
	//不考虑边界(m_inclusive) 没有影响
	double ret = XRandom::randDouble((double)GetMinValue(unit) , (double)GetMaxValue(unit));
	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << ret << " <= [" << GetMinValue(unit) << ", " << GetMaxValue(unit) << "); Success");

	SharedData* data = unit->AIData();
	if(data)
	{
		data->SetFloatByIndex(m_ResultIndex , (float)ret);
	}
	AIRETURN("RandomFloat", true)
}

float RandomFloat::GetMinValue(Unit* unit)
{
	SharedData* data = unit->AIData();
	if(data)
	{
		return data->GetFloatByIndex(m_MinIndex , m_minValue);
	}
	return 0.0;
}

float RandomFloat::GetMaxValue(Unit* unit)
{
	SharedData* data = unit->AIData();
	if(data)
	{
		return data->GetFloatByIndex(m_MaxIndex , m_maxValue);
	}
	return 0.0;
}


FloatOperator::FloatOperator( const tinyxml2::XMLElement* elem )
{
	m_type = (FloatOperatorType)(0);
	m_float1Value = 0.0;
	m_float2Value = 0.0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("type");
		if(attr)
		{
			m_type = (FloatOperatorType)(attr->IntValue());			
		}
		attr = elem->FindAttribute("Shared_Float1Name");
		if(attr)
		{
			m_sharedFloat1Name = attr->Value();	

			if (m_sharedFloat1Name != "")
				m_FloatIndex1 = AINodeFactory::CreateIndexByName(m_sharedFloat1Name.c_str());
			else
				m_FloatIndex1 = -1;
		}
		attr = elem->FindAttribute("float1Value");
		if(attr)
		{
			m_float1Value = attr->FloatValue();
		}
		attr = elem->FindAttribute("Shared_Float2Name");
		if(attr)
		{
			m_sharedFloat2Name = attr->Value();	

			if (m_sharedFloat2Name != "")
				m_FloatIndex2 = AINodeFactory::CreateIndexByName(m_sharedFloat2Name.c_str());
			else
				m_FloatIndex2 = -1;
		}
		attr = elem->FindAttribute("float2Value");
		if(attr)
		{
			m_float2Value = attr->FloatValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_ResultIndex = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
			else
				m_ResultIndex = -1;
		}
	}
}

bool FloatOperator::Update( Unit* unit )
{
	SharedData* data = unit->AIData();
	if(data)
	{
		float f1 = GetFloat1(unit);
		float f2 = GetFloat2(unit);
		float ret = 0;
		DEFINE_AI_DEBUG_HELPER(this, unit);

		switch(m_type)
		{
			case FOTYPE_ADD:
				{
					ret = f1 + f2;
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " + " << f2 << "; ");
				}
				break;
			case FOTYPE_SUBTRACT:
				{
					ret = f1 - f2;
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " - " << f2 << "; ");
				}
				break;
			case FOTYPE_MULTIPLY:
				{
					ret = f1 * f2;
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " * " << f2 << "; ");
				}
				break;
			case FOTYPE_DIVIDE:
				{
					if(f2 != 0.0)
					{
						ret = f1 / f2;
						AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " / " << f2 << "; ");
					}
				}
				break;
			case FOTYPE_MIN:
				{
					if(f1 < f2)
					{
						ret = f1;
					}
					else
					{
						ret = f2;
					}
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = min(" << f1 << ", " << f2 << "); ");
				}
				break;
			case FOTYPE_MAX:
				{
					if(f1 > f2)
					{
						ret = f1;
					}
					else
					{
						ret = f2;
					}
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = max(" << f1 << ", " << f2 << "); ");
				}
				break;
			default:
				{
					SSWarn << "error float operator type =  " << ((int)m_type) << END;
					AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatOperator", true)
				}
				break;
		}

		if (m_sharedStoredResultName == "ForwardProb")
		{
			//printf("ForwardProb");
		}

		if (m_sharedStoredResultName == "TotalProb")
		{
			//printf("TotalProb");
		}
		data->SetFloatByIndex(m_ResultIndex , ret);
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatOperator", true)
}

float FloatOperator::GetFloat1(Unit* unit)
{
	SharedData* data = unit->AIData();
	if(data)
	{
		return data->GetFloatByIndex(m_FloatIndex1 , m_float1Value);
	}
	return 0.0;
}

float FloatOperator::GetFloat2(Unit* unit)
{
	SharedData* data = unit->AIData();
	if(data)
	{
		return data->GetFloatByIndex(m_FloatIndex2 , m_float2Value);
	}
	return 0.0;
}

IntOperator::IntOperator( const tinyxml2::XMLElement* elem )
{
	m_type = (FloatOperatorType)(0);
	m_int1Value = 0;
	m_int2Value = 0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("type");
		if(attr)
		{
			m_type = (FloatOperatorType)(attr->IntValue());			
		}
		attr = elem->FindAttribute("Shared_Int1Name");
		if(attr)
		{
			m_sharedInt1Name = attr->Value();	

			if (m_sharedInt1Name != "")
				m_IntIndex1 = AINodeFactory::CreateIndexByName(m_sharedInt1Name.c_str());
			else
				m_IntIndex1 = -1;
		}
		attr = elem->FindAttribute("int1Value");
		if(attr)
		{
			m_int1Value = attr->IntValue();
		}
		attr = elem->FindAttribute("Shared_Int2Name");
		if(attr)
		{
			m_sharedInt2Name = attr->Value();	

			if (m_sharedInt2Name != "")
				m_IntIndex2 = AINodeFactory::CreateIndexByName(m_sharedInt2Name.c_str());
			else
				m_IntIndex2 = -1;
		}
		attr = elem->FindAttribute("int2Value");
		if(attr)
		{
			m_int2Value = attr->IntValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_StoreValue = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
			else
				m_StoreValue = -1;
		}
	}
}

bool IntOperator::Update( Unit* unit )
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	SharedData* data = unit->AIData();
	if(data)
	{
		int f1 = GetInt1(data);
		int f2 = GetInt2(data);
		int ret = 0;
		switch(m_type)
		{
		case FOTYPE_ADD:
			{
				ret = f1 + f2;
				AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " + " << f2 << "; ");
			}
			break;
		case FOTYPE_SUBTRACT:
			{
				ret = f1 - f2;
				AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " - " << f2 << "; ");
			}
			break;
		case FOTYPE_MULTIPLY:
			{
				ret = f1 * f2;
				AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " * " << f2 << "; ");
			}
			break;
		case FOTYPE_DIVIDE:
			{
				if(f2 != 0)
				{
					ret = f1 / f2;
					AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = " << f1 << " / " << f2 << "; ");
				}
			}
			break;
		case FOTYPE_MIN:
			{
				if(f1 < f2)
				{
					ret = f1;
				}
				else
				{
					ret = f2;
				}
				AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = min(" << f1 << ", " << f2 << "); ");
			}
			break;
		case FOTYPE_MAX:
			{
				if(f1 > f2)
				{
					ret = f1;
				}
				else
				{
					ret = f2;
				}
				AI_DEBUG_STATEMENT(m_DebugInfo << ret << " = max(" << f1 << ", " << f2 << "); ");
			}
			break;
		default:
			{
				SSWarn << "error int operator type =  " << ((int)m_type) << END;
				AI_DEBUG_RETURN_WITHOUT_DEFINE("IntOperator", false)
			}
			break;
		}
		data->SetIntByIndex(m_StoreValue, ret);
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("IntOperator", true)
}

int IntOperator::GetInt1( SharedData* data )
{
	return data->GetIntByIndex(m_IntIndex1 , m_int1Value);
}

int IntOperator::GetInt2( SharedData* data )
{
	return data->GetIntByIndex(m_IntIndex2 , m_int2Value);
}

FloatComparison::FloatComparison( const tinyxml2::XMLElement* elem )
{
	m_type = (ComparisonType)(0);
	m_float1Value = 0.0;
	m_float2Value = 0.0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("type");
		if(attr)
		{
			m_type = (ComparisonType)(attr->IntValue());			
		}
		attr = elem->FindAttribute("Shared_Float1Name");
		if(attr)
		{
			m_sharedFloat1Name = attr->Value();	

			if (m_sharedFloat1Name != "")
				m_floatIndex1 = AINodeFactory::CreateIndexByName(m_sharedFloat1Name.c_str());
			else
				m_floatIndex1 = -1;
		}
		attr = elem->FindAttribute("float1Value");
		if(attr)
		{
			m_float1Value = attr->FloatValue();
		}
		attr = elem->FindAttribute("Shared_Float2Name");
		if(attr)
		{
			m_sharedFloat2Name = attr->Value();	

			if (m_sharedFloat2Name != "")
				m_floatIndex2 = AINodeFactory::CreateIndexByName(m_sharedFloat2Name.c_str());
			else
				m_floatIndex2 = -1;
		}
		attr = elem->FindAttribute("float2Value");
		if(attr)
		{
			m_float2Value = attr->FloatValue();
		}
		
	}
}

bool FloatComparison::Update( Unit* unit )
{
	SharedData* data = unit->AIData();
	float f1 = GetFloat1(data);
	float f2 = GetFloat2(data);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	switch(m_type)
	{
	case FCTYPE_LESS_THAN:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " < " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_LESS_THAN",  f1 < f2);
		}
		break;
	case  FCTYPE_LESS_OR_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " <= " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_LESS_OR_EQUAL_TO",  f1 <= f2);
		}
		break;
	case FCTYPE_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " == " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_EQUAL_TO",  f1 == f2);
		}
		break;
	case FCTYPE_NOT_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " != " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_NOT_EQUAL_TO",  f1 != f2);
		}
		break;
	case FCTYPE_GREATER_THAN_OR_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " >= " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_GREATER_THAN_OR_EQUAL_TO",  f1 >= f2);
		}
		break;
	case FCTYPE_GREATER_THAN:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " > " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison FCTYPE_GREATER_THAN", f1 > f2);
		}
		break;
	default:
		{
			SSWarn<< "FloatComparison error type = " << ((int)m_type) << END;
			AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison", false);
		}
		break;;
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("FloatComparison", false);
}

float FloatComparison::GetFloat1(SharedData* data)
{
	return data->GetFloatByIndex(m_floatIndex1 , m_float1Value);
}

float FloatComparison::GetFloat2(SharedData* data)
{
	return data->GetFloatByIndex(m_floatIndex2 , m_float2Value);
}

IntComparison::IntComparison( const tinyxml2::XMLElement* elem )
{
	m_type = (ComparisonType)(0);
	m_int1Value = 0;
	m_int2Value = 0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("type");
		if(attr)
		{
			m_type = (ComparisonType)(attr->IntValue());			
		}
		attr = elem->FindAttribute("Shared_Int1Name");
		if(attr)
		{
			m_sharedInt1Name = attr->Value();	

			if (m_sharedInt1Name != "")
				m_intIndex1 = AINodeFactory::CreateIndexByName(m_sharedInt1Name.c_str());
			else
				m_intIndex1 = -1;
		}
		attr = elem->FindAttribute("int1Value");
		if(attr)
		{
			m_int1Value = attr->IntValue();
		}
		attr = elem->FindAttribute("Shared_Int2Name");
		if(attr)
		{
			m_sharedInt2Name = attr->Value();	

			if (m_sharedInt2Name != "")
				m_intIndex2 = AINodeFactory::CreateIndexByName(m_sharedInt2Name.c_str());
			else
				m_intIndex2 = -1;
		}
		attr = elem->FindAttribute("int2Value");
		if(attr)
		{
			m_int2Value = attr->IntValue();
		}

	}
}

bool IntComparison::Update( Unit* unit )
{
	SharedData* data = unit->AIData();
	int f1 = GetInt1(data);
	int f2 = GetInt2(data);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	switch(m_type)
	{
	case FCTYPE_LESS_THAN:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " < " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_LESS_THAN ", f1 < f2);
		}
		break;
	case  FCTYPE_LESS_OR_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " <= " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_LESS_OR_EQUAL_TO ", f1 <= f2);
		}
		break;
	case FCTYPE_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " == " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_EQUAL_TO ", f1 == f2);
		}
		break;
	case FCTYPE_NOT_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " != " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_NOT_EQUAL_TO ", f1 != f2);
		}
		break;
	case FCTYPE_GREATER_THAN_OR_EQUAL_TO:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " >= " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_NOT_EQUAL_TO ", f1 >= f2);
		}
		break;
	case FCTYPE_GREATER_THAN:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " > " << f2 << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison, FCTYPE_NOT_EQUAL_TO ", f1 > f2);
		}
		break;
	default:
		{
			SSWarn<< "IntComparison error type = " << ((int)m_type) << END;
			AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison", false);
		}
		break;;
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("IntComparison", false);
}

int IntComparison::GetInt1(SharedData* data)
{
	return data->GetIntByIndex(m_intIndex1 , m_int1Value);
}

int IntComparison::GetInt2(SharedData* data)
{
	return data->GetIntByIndex(m_intIndex2 , m_int2Value);
}

SetFloat::SetFloat( const tinyxml2::XMLElement* elem )
{
	m_value = 0.0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_ValueName");
		if(attr)
		{
			m_sharedValueName = attr->Value();

			if (m_sharedValueName != "")
				m_valueIndex = AINodeFactory::CreateIndexByName(m_sharedValueName.c_str());
			else
				m_valueIndex = -1;
		}
		attr = elem->FindAttribute("value");
		if(attr)
		{
			m_value = attr->FloatValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_resultIndex = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
			else
				m_resultIndex = -1;
		}
	}
}

bool SetFloat::Update( Unit* unit )
{
	SharedData * data = unit->AIData();
	DEFINE_AI_DEBUG_HELPER(this, unit);
	if(data)
	{
		float ret = GetValue(data);
		AI_DEBUG_STATEMENT(m_DebugInfo << ret << "; ");
		data->SetFloatByIndex(m_resultIndex , ret);
	}else
	{
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SetFloat", false);
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SetFloat", true);
}

float SetFloat::GetValue(SharedData* data)
{
	return data->GetFloatByIndex(m_valueIndex , m_value);
}

SetBool::SetBool( const tinyxml2::XMLElement* elem )
{
	m_value = false;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_ValueName");
		if(attr)
		{
			m_sharedValueName = attr->Value();
			if (m_sharedValueName != "")
				m_valueIndex = AINodeFactory::CreateIndexByName(m_sharedValueName.c_str());
			else
				m_valueIndex = -1;
		}
		attr = elem->FindAttribute("value");
		if(attr)
		{
			m_value = attr->BoolValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_resultIndex = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
			else
				m_resultIndex = -1;
		}
	}
}

bool SetBool::Update( Unit* unit )
{
	SharedData* data = unit->AIData();
	DEFINE_AI_DEBUG_HELPER(this, unit);
	if(data)
	{
		bool tmp = data->GetBoolByIndex(m_valueIndex , m_value);
		AI_DEBUG_STATEMENT(m_DebugInfo << tmp << "; ");
		data->SetBoolByIndex(m_resultIndex , tmp);

	}else
	{
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SetBool", false);
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SetBool", true);
}

BoolComparison::BoolComparison( const tinyxml2::XMLElement* elem )
{	
	m_bool1Value = false;
	m_bool2Value = false;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_Bool1Name");
		if(attr)
		{
			m_sharedBool1Name = attr->Value();	
			if (m_sharedBool1Name != "")
				m_boolIndex1 = AINodeFactory::CreateIndexByName(m_sharedBool1Name.c_str());
			else
				m_boolIndex1 = -1;
		}
		attr = elem->FindAttribute("bool1Value");
		if(attr)
		{
			m_bool1Value = attr->BoolValue();
		}
		attr = elem->FindAttribute("Shared_Bool2Name");
		if(attr)
		{
			m_sharedBool2Name = attr->Value();	

			if (m_sharedBool2Name != "")
				m_boolIndex2 = AINodeFactory::CreateIndexByName(m_sharedBool2Name.c_str());
			else
				m_boolIndex2 = -1;
		}
		attr = elem->FindAttribute("bool2Value");
		if(attr)
		{
			m_bool2Value = attr->BoolValue();
		}

	}
}

bool BoolComparison::Update( Unit* unit )
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();
	if(data)
	{
		bool f1 = data->GetBoolByIndex(m_boolIndex1 , m_bool1Value);
		bool f2 = data->GetBoolByIndex(m_boolIndex2 , m_bool2Value);
		AI_DEBUG_STATEMENT(m_DebugInfo << f1 << " == " << f2 << "; ");
		AI_DEBUG_RETURN_WITHOUT_DEFINE("BoolComparision", (f1 == f2));
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("BoolComparison", false);
}

CompareTo::CompareTo( const tinyxml2::XMLElement* elem )
{
	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_FirstStringName");	
		if(attr)
		{
			m_sharedString1Name = attr->Value();
			if (m_sharedString1Name != "")
				m_stringIndex1 = AINodeFactory::CreateIndexByName(m_sharedString1Name.c_str());
			else
				m_stringIndex1 = -1;
		}

		attr = elem->FindAttribute("firstString");
		if(attr)
		{
			m_string1Value = attr->Value();
		}

		attr = elem->FindAttribute("Shared_SecondStringName");
		if(attr)
		{
			m_sharedString2Name = attr->Value();
			if (m_sharedString2Name != "")
				m_stringIndex2 = AINodeFactory::CreateIndexByName(m_sharedString2Name.c_str());
			else
				m_stringIndex2 = -1;
		}
		
		attr = elem->FindAttribute("secondString");
		if(attr)
		{
			m_string2Value = attr->Value();
		}

		attr = elem->FindAttribute("Shared_ResultName");
		if(attr)
		{
			m_sharedResultName = attr->Value();

			if (m_sharedResultName != "")
				m_resultIndex = AINodeFactory::CreateIndexByName(m_sharedResultName.c_str());
			else
				m_resultIndex = -1;
		}
	}
}

bool CompareTo::Update( Unit* unit )
{
	SharedData *data = unit->AIData();

	const std::string& ss1 = data ? data->GetStringByIndex(m_stringIndex1 , m_string1Value) : "";
	const std::string& ss2 = data ? data->GetStringByIndex(m_stringIndex2 , m_string2Value) : "";

	DEFINE_AI_DEBUG_HELPER(this, unit);

	int ret = 0;
	if(ss1 > ss2)
	{
		ret = 1;
	}else if(ss1 < ss2)
	{
		ret = -1;
	}else if(ss1 == ss2)
	{
		ret = 0;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << ret << " <= " << ss1 << " ?? " << ss2 << "; ");

	if(data)
	{
		data->SetIntByIndex(m_resultIndex , ret);
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("CompareTo", true);
}


SetInt::SetInt( const tinyxml2::XMLElement* elem )
{
	m_value = 0;

	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_ValueName");
		if(attr)
		{
			m_sharedValueName = attr->Value();
			if (m_sharedValueName != "")
				m_intIndex = AINodeFactory::CreateIndexByName(m_sharedValueName.c_str());
			else
				m_intIndex = -1;
		}
		attr = elem->FindAttribute("value");
		if(attr)
		{
			m_value = attr->IntValue();
		}
		attr = elem->FindAttribute("Shared_StoredResultName");
		if(attr)
		{
			m_sharedStoredResultName = attr->Value();

			if (m_sharedStoredResultName != "")
				m_resultIndex = AINodeFactory::CreateIndexByName(m_sharedStoredResultName.c_str());
			else
				m_resultIndex = -1;
		}
	}
}

bool SetInt::Update( Unit* unit )
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	SharedData* data = unit->AIData();
	if(data)
	{
		int tmp = data->GetIntByIndex(m_intIndex , m_value);
		AI_DEBUG_STATEMENT(m_DebugInfo << tmp << "; ");
		data->SetIntByIndex(m_resultIndex , tmp);

	}else
	{
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SetInt", false);
	}
	 
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SetInt", true);
}

GetRealtimeSinceStartup::GetRealtimeSinceStartup(const tinyxml2::XMLElement* elem)
{
	m_nameIndex = -1;
	if (elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_FloatstoreResultName");
		if (attr)
		{
			m_sharedValueName = attr->Value();
			if (m_sharedValueName != "")
				m_nameIndex = AINodeFactory::CreateIndexByName(m_sharedValueName);
		}
	}
}

bool GetRealtimeSinceStartup::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	UINT64 curtime = TimeUtil::GetMilliSecond();
	float timef = (float)(curtime*1.0/1000);
	
	AI_DEBUG_STATEMENT(m_DebugInfo << timef << "; ");

	if (m_nameIndex != -1)
	{
		unit->AIData()->SetFloatByIndex(m_nameIndex, timef);
		AI_DEBUG_RETURN_WITHOUT_DEFINE("GetRealtimeSinceStartup", true);
	}
	else
		AI_DEBUG_RETURN_WITHOUT_DEFINE("GetRealtimeSinceStartup", false);
}