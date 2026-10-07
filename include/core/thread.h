#ifndef THREAD_H
#define THREAD_H
#include <Windows.h>

typedef struct EngineClass EngineClass;

DWORD WINAPI newThread(LPVOID lpParam);

// The name must remain createThread because CreateThread can cause error due to being same name as the Windows API function CreateThread();
HANDLE createThread(EngineClass *engine);

#endif