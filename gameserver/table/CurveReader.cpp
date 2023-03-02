#include "pch.h"
#include "CurveReader.h"
#include "baseconfig.h"
#include "utility/XActionCurve.h"

XActionCurve *CurveReader::ReadCurve(const char *fileName)
{
	std::string content = Config::GetConfig()->ReadFileContent(fileName);
	if (content.empty())
	{
		SSWarn << "Read XActionCurve " << fileName << " Failed!" << END;
		return NULL;
	}

	std::stringstream ss(content);
	
	XActionCurve *pCurve = new XActionCurve;
	float a, b;
	
	if (ss >> a >> b)
	{
		pCurve->SetMaxValue(a);
		pCurve->SetFirstLandValue(b);
	}
	else
	{
		SSWarn << "Read XActionCurve " << fileName << " Failed!" << END;
		delete pCurve;
		return NULL;
	}
	
	int idx = 0;
	while (true)
	{
		if (ss >> a >> b)
		{
			pCurve->AddKey(idx, a, b);
			++idx;
		}
		else
		{
			break;
		}
	}

	return pCurve;
}

