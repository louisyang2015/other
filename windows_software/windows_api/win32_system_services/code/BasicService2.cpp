// beepserv.cpp

#include <windows.h>
#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>

// Global variables

// The name of the service
char *SERVICE_NAME = "BeepService";
// Event used to hold ServiceMain from completing
HANDLE terminateEvent = NULL;
// Handle used to communicate status info with
// the SCM. Created by RegisterServiceCtrlHandler
SERVICE_STATUS_HANDLE serviceStatusHandle;
// The beep interval in ms.
int beepDelay = 2000;
// Flags holding current state of service
BOOL pauseService = FALSE;
BOOL runningService = FALSE;
// Thread for the actual work
HANDLE threadHandle = 0;

void ErrorHandler(char *s, DWORD err)
{
	cout << s << endl;
	cout << "Error number: " << err << endl;
	ExitProcess(err);
}

DWORD ServiceThread(LPDWORD param)
{
	while (1)
	{
		Beep(200,200);
		Sleep(beepDelay);
	}
	return 0;
}

// Initializes the service by starting its thread
BOOL InitService()
{
	DWORD id;

	// Start the service's thread
	threadHandle = CreateThread(0, 0,
		(LPTHREAD_START_ROUTINE) ServiceThread,
		0, 0, &id);

	if (threadHandle==0)
		return FALSE;
	else
	{
		runningService = TRUE;
		return TRUE;
	}
}

// Resumes a paused service
VOID ResumeService()
{
	pauseService=FALSE;
	ResumeThread(threadHandle);
}

// Pauses the service
VOID PauseService()
{
	pauseService = TRUE;
	SuspendThread(threadHandle);
}

// Stops the service by allowing ServiceMain to
// complete
VOID StopService() 
{
	runningService=FALSE;
	// Set the event that is holding ServiceMain
	// so that ServiceMain can return
	SetEvent(terminateEvent);
}

// This function consolidates the activities of 
// updating the service status with
// SetServiceStatus
BOOL SendStatusToSCM (DWORD dwCurrentState,
	DWORD dwWin32ExitCode, 
	DWORD dwServiceSpecificExitCode,
	DWORD dwCheckPoint,
	DWORD dwWaitHint)
{
	BOOL success;
	SERVICE_STATUS serviceStatus;

	// Fill in all of the SERVICE_STATUS fields
	serviceStatus.dwServiceType =
		SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwCurrentState;

	// If in the process of something, then accept
	// no control events, else accept anything
	if (dwCurrentState == SERVICE_START_PENDING)
		serviceStatus.dwControlsAccepted = 0;
	else
		serviceStatus.dwControlsAccepted = 
			SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN;

	// if a specific exit code is defines, set up
	// the win32 exit code properly
	if (dwServiceSpecificExitCode == 0)
		serviceStatus.dwWin32ExitCode =
			dwWin32ExitCode;
	else
		serviceStatus.dwWin32ExitCode = 
			ERROR_SERVICE_SPECIFIC_ERROR;
	serviceStatus.dwServiceSpecificExitCode =
		dwServiceSpecificExitCode;

	serviceStatus.dwCheckPoint = dwCheckPoint;
	serviceStatus.dwWaitHint = dwWaitHint;

	// Pass the status record to the SCM
	success = SetServiceStatus (serviceStatusHandle,
		&serviceStatus);
	if (!success)
		StopService();

	return success;
}

// Dispatches events received from the service 
// control manager
VOID ServiceCtrlHandler (DWORD controlCode) 
{
	DWORD  currentState = 0;
	BOOL success;

	switch(controlCode)
	{
		// There is no START option because
		// ServiceMain gets called on a start

		// Stop the service
		case SERVICE_CONTROL_STOP:
			currentState = SERVICE_STOP_PENDING;
			// Tell the SCM what's happening
			success = SendStatusToSCM(
				SERVICE_STOP_PENDING,
				NO_ERROR, 0, 1, 5000);
			// Not much to do if not successful

			// Stop the service
			StopService();
			return;

		// Pause the service
		case SERVICE_CONTROL_PAUSE:
			if (runningService && !pauseService)
			{
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_PAUSE_PENDING,
					NO_ERROR, 0, 1, 1000);
				PauseService();
				currentState = SERVICE_PAUSED;
			}
			break;

		// Resume from a pause
		case SERVICE_CONTROL_CONTINUE:
			if (runningService && pauseService)
			{
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_CONTINUE_PENDING,
					NO_ERROR, 0, 1, 1000);
					ResumeService();
					currentState = SERVICE_RUNNING;
			}
			break;

		// Update current status
		case SERVICE_CONTROL_INTERROGATE:
			// it will fall to bottom and send status
			break;

		// Do nothing in a shutdown. Could do cleanup
		// here but it must be very quick.
		case SERVICE_CONTROL_SHUTDOWN:
			// Do nothing on shutdown
			return;
		default:
 			break;
	}
	SendStatusToSCM(currentState, NO_ERROR,
		0, 0, 0);
}

// Handle an error from ServiceMain by cleaning up
// and telling SCM that the service didn't start.
VOID terminate(DWORD error)
{
	// if terminateEvent has been created, close it.
	if (terminateEvent)
		CloseHandle(terminateEvent);

	// Send a message to the scm to tell about
	// stopage
	if (serviceStatusHandle)
		SendStatusToSCM(SERVICE_STOPPED, error,
			0, 0, 0);

	// If the thread has started kill it off
	if (threadHandle)
		CloseHandle(threadHandle);

	// Do not need to close serviceStatusHandle
}

// ServiceMain is called when the SCM wants to
// start the service. When it returns, the service
// has stopped. It therefore waits on an event
// just before the end of the function, and
// that event gets set when it is time to stop. 
// It also returns on any error because the
// service cannot start if there is an eror.
VOID ServiceMain(DWORD argc, LPTSTR *argv) 
{
	BOOL success;

	// immediately call Registration function
	serviceStatusHandle =
		RegisterServiceCtrlHandler(
			SERVICE_NAME,
			(LPHANDLER_FUNCTION) ServiceCtrlHandler);
	if (!serviceStatusHandle)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 1, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// create the termination event
	terminateEvent = CreateEvent (0, TRUE, FALSE,
		0);
	if (!terminateEvent)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 2, 1000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Check for startup params
	if (argc == 2)
	{
		int temp = atoi(argv[1]);
		if (temp < 1000)
			beepDelay = 2000;
		else
			beepDelay = temp;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 3, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Start the service itself
	success = InitService();
	if (!success)
	{
		terminate(GetLastError());
		return;
	}

	// The service is now running. 
	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_RUNNING,
		NO_ERROR, 0, 0, 0);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Wait for stop signal, and then terminate
	WaitForSingleObject (terminateEvent, INFINITE);

	terminate(0);
}

VOID main(VOID)
{
	SERVICE_TABLE_ENTRY serviceTable[] = 
	{ 
	{ SERVICE_NAME,
		(LPSERVICE_MAIN_FUNCTION) ServiceMain},
	{ NULL, NULL }
	};
	BOOL success;

	// Register with the SCM
	success = 
		StartServiceCtrlDispatcher(serviceTable);
	if (!success)
		ErrorHandler("In StartServiceCtrlDispatcher",
			GetLastError());
}
