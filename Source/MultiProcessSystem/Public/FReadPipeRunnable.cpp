#include "FReadPipeRunnable.h"
#include "Kismet/KismetSystemLibrary.h"

bool FReadPipeRunnable::Init()
{
	UKismetSystemLibrary::PrintString(WorldContextObject, FString::Printf(TEXT("Process Runable Init")), false,true);
	UE_LOG(LogTemp, Warning, TEXT("ProcessReadInit"))
	return FRunnable::Init();
}

uint32 FReadPipeRunnable::Run()
{
	while (FPlatformProcess::IsProcRunning(ProcHandle))
	{
		FPlatformProcess::Sleep(0.5);
		UE_LOG(LogTemp, Warning, TEXT("ProcessReadRun"))
		FString Data = FPlatformProcess::ReadPipe(ReadPipe);
		// Processing Data
		if (!Data.IsEmpty())
		{
			AsyncTask(ENamedThreads::GameThread, [this,Data]() {
				//TODO 
				OnProcessOutput.Broadcast(ProcessID,Data);
				});

		}
		
	}
	
	//Exit();
	return 0;
}

void FReadPipeRunnable::Stop()
{
	FRunnable::Stop();
	UE_LOG(LogTemp, Warning, TEXT("ProcessReadStop"))
}

void FReadPipeRunnable::Exit()
{
	FRunnable::Exit();
	UE_LOG(LogTemp,Warning,TEXT("ProcessReadExit"))
}
