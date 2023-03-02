#ifndef _FUNCTION_OPEN_BASE_H_
#define _FUNCTION_OPEN_BASE_H_

#include "pb/enum.pb.h"

class FunctionOpenBase
{
public:
	FunctionOpenBase();
	virtual ~FunctionOpenBase();

	bool IsOpen(KKSG::FunctionId func);
	void SetOpen(KKSG::FunctionId func, bool isOpen);

	void SetOpenFlag(UINT32 flag) { m_flag = flag; }
	UINT32 GetOpenFlag() { return m_flag; }

protected:
	UINT32 m_flag;
};

#endif