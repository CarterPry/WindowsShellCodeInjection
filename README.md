# ShellCodeInjectionWin10
Simple shell injection using [win32 API](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list)


## 1) Attatch to an exisiting, or newly created process
- Using Windows function [OpenProcess()](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess) 

## 2) Make space in that process
- Enough memory for you shell code to fit inside of
- Can use [VirtualAlloc()](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc)

## 3) Add your own memory to the proccess
-Writing the process memory to embed your code using [WriteProcessMemory()](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory)

## 4) Create a thread in the proccess to run your code that is in the proccess memory
- The thread to deliver your payload using the function [CreateRemoteThread()](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread)
