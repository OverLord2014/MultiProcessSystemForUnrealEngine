#include "FReadPipeRunnable.h"
#include "Kismet/KismetSystemLibrary.h"

bool FReadPipeRunnable::Init()
{
	return FRunnable::Init();
}

uint32 FReadPipeRunnable::Run()
{
	while (FPlatformProcess::IsProcRunning(ProcHandle))
	{
		FPlatformProcess::Sleep(0.1);
		FString Data = FPlatformProcess::ReadPipe(ReadPipe);
		// Processing Data
		if (!Data.IsEmpty())
		{
			AsyncTask(ENamedThreads::GameThread, [this,Data]() {
				//TODO 
				//UKismetSystemLibrary::PrintString(WorldContextObject,FString::Printf(TEXT("PipeData:%s"),*OutString),false);
				OnProcessOutput.Broadcast(ProcessID,Data);
				});
		}
	}
	Stop();
	Exit();
	return 0;
}

void FReadPipeRunnable::Stop()
{
	FRunnable::Stop();
}

void FReadPipeRunnable::Exit()
{
	FRunnable::Exit();
}
