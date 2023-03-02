#ifndef __SERVER_PROCESS_H__
#define __SERVER_PROCESS_H__

bool Process_Setup(const char *file);
bool Process_Update();
void Process_Cleanup();
void Process_Stop();

#endif