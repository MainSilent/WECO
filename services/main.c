#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <wingdi.h>
#include <winuser.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SVCNAME TEXT("eco_service")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv[]);
VOID WINAPI ServiceCtrlHandler(DWORD ctrlCode);
void WINAPI ServiceWorkerThread(LPVOID lpParam);

SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

int main() {
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {SVCNAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        return GetLastError();
    }

    return 0;
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv[]) {
    g_StatusHandle = RegisterServiceCtrlHandler(SVCNAME, ServiceCtrlHandler);

    if (g_StatusHandle == NULL) {
        return;
    }

    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        return;
    }

    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServiceWorkerThread, NULL, 0, NULL);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(g_ServiceStopEvent);

    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

VOID WINAPI ServiceCtrlHandler(DWORD ctrlCode) {
    switch (ctrlCode) {
        case SERVICE_CONTROL_STOP:
            if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING) {
                break;
            }

            g_ServiceStatus.dwControlsAccepted = 0;
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            g_ServiceStatus.dwWin32ExitCode = 0;
            g_ServiceStatus.dwCheckPoint = 4;

            SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

            SetEvent(g_ServiceStopEvent);
            return;

        default:
            break;
    }

    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

void WINAPI ServiceWorkerThread(LPVOID lpParam) {

}
