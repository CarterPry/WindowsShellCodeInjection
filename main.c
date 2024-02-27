#include <windows.h>
#include <stdio.h>

// Gotta use those macros
//#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
//#define warn(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)
//#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
const char* k = "[+]"; // successful indidication 
const char* i = "[*]"; // working indidication 
const char* e = "[-]"; // error indidication 

DWORD PID;
//DWORD is pretty much a 32 bit place holder

HANDLE hProcess = NULL;
HANDLE hThread = NULL;
// "Hungarian" notation used with the h infront because Microsoft adapted that for their varailbes where it is the varialbe type of what is being returned

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

/*
    hProcess = OpenProcess(
                PROCESS_ALL_ACCESS,
                FALSE,  
                PID
                );
    */

    if (hProcess = NULL){
        printf("%s couldn't get (%ld), error %ld", e, PID, GetLastError());
        return 1;
    }

    

    return 0;
}

