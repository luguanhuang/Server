#ifndef __CUTSCENEREADER_H__
#define __CUTSCENEREADER_H__

#include <vector>
#include <string>
#include <limits.h>
#include "define.h"

using namespace std;

class CutSceneReader
{
public:
	static int LoadCutSceneTime(const char *fileName);
};

#endif // __CUTSCENEREADER_H__