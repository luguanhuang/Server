#ifndef _TABLE_HELPER_H_
#define _TABLE_HELPER_H_

#include "util/utilsingleton.h"

template <typename T>
class TableHelper : public Singleton<T>
{
public:
	virtual bool DoLoadFile() = 0;
	virtual void ClearFile() = 0;
	virtual void OnLoaded() {};

	bool LoadFile()
	{
		T tmp;
		if (!tmp.DoLoadFile())
		{
			return false;
		}
		ClearFile();
		DoLoadFile();
		OnLoaded();
		return true;
	}
};

#endif
