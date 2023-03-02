#include "pch.h"
#include "functionopenbase.h"

FunctionOpenBase::FunctionOpenBase()
:m_flag(0)
{

}

FunctionOpenBase::~FunctionOpenBase()
{

}

bool FunctionOpenBase::IsOpen(KKSG::FunctionId func)
{
	if (func < 0 || func > 31)
	{
		SSError << "invalid function id: " << func << END;
		return false;
	}

	return (m_flag & (1 << func)) != 0;
}

void FunctionOpenBase::SetOpen(KKSG::FunctionId func, bool isOpen)
{
	if (func < 0 || func > 31)
	{
		SSError << "invalid function id: " << func << END;
		return;
	}

	if (isOpen)
	{
		m_flag |= (1 << func);	
	}
	else
	{
		m_flag &= (~(1 << func));	
	}
}