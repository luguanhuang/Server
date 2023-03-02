#ifndef GUILDAUCTDBHANDLER_H__
#define GUILDAUCTDBHANDLER_H__

class GABaseData; 

class GADBHandler
{
public:
	static void Add(GABaseData* data);
	static void Update(GABaseData* data);
	static void Del(UINT64 UID);
	static bool Read();
};

#endif