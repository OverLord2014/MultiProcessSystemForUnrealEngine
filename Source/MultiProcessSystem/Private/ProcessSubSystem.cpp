// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\ProcessSubSystem.h"
#include "FReadPipeRunnable.h"


void UProcessSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UProcessSubSystem::Deinitialize()
{
	Super::Deinitialize();
	KillAllProcess();
}

EProcState UProcessSubSystem::StartProcess(FString inUrl, FString inParms, FString workpath, bool bLanchDetached, bool bLanchHidden,
                             bool bLanchReallyHidden,bool bAllowMultiProc)
{
	UpdateHandleState();
	if (ProcessHandleMap.Find(inUrl)&&!bAllowMultiProc)
	{
		return EProcState::AlreadyCreate;
	}
	uint32 ProcID=-1;
	
	void* StdOutReadHandle = nullptr;
	void* StdOutWriteHandle = nullptr;
	void* StdInReadHandle = nullptr;
	void* StdInWriteHandle = nullptr;
	FPlatformProcess::CreatePipe(StdOutReadHandle, StdOutWriteHandle);
	FPlatformProcess::CreatePipe(StdInReadHandle, StdInWriteHandle,true);
	
	const FProcHandle& ProcessHandle = FPlatformProcess::CreateProc(*inUrl, *inParms, bLanchDetached, bLanchHidden, bLanchReallyHidden, &ProcID, 0, *workpath, StdOutWriteHandle, StdOutReadHandle);
	if (ProcessHandle.IsValid())
	{
		NativeOnProcessOutput NativeProcessOutput;
		NativeProcessOutput.AddUObject(this,&UProcessSubSystem::EventOnProcessOutPut);
		const FString ThreadName=TEXT("TestThread");
		
		FReadPipeRunnable* ReadPipeRunnable = new FReadPipeRunnable(ProcessHandle, StdOutReadHandle,NativeProcessOutput,ProcID,GetWorld());
		TSharedPtr<FRunnableThread> ReadProcessThread = MakeShareable(FRunnableThread::Create(ReadPipeRunnable, TEXT("ReadProcessThread")));
		ProcessHandleMap.Add(inUrl,FProcessInfo(ProcID,ProcessHandle, ReadProcessThread));
		return EProcState::CreateSuccess;
	}
	return EProcState::Failed;
}

void UProcessSubSystem::UpdateHandleState()
{
	TArray<FString> KeysToRemove;
	for (const auto& Elem : ProcessHandleMap)
	{
		FString Key = Elem.Key;
		FProcHandle Value = Elem.Value.ProcessHandle;
		
		if (!Value.IsValid()||!FPlatformProcess::IsProcRunning(Value))
		{
			KeysToRemove.Add(Key);
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		ProcessHandleMap.Remove(Key);
	}
}

void UProcessSubSystem::EventOnProcessOutPut(int32 ProcessID, FString DataString)
{
	UE_LOG(LogTemp, Warning, TEXT("OnProcessOutPut"))
	OnProcessOutput.Broadcast(ProcessID,DataString);
}

void UProcessSubSystem::KillAllProcess()
{
	TArray<FString>keys;
	ProcessHandleMap.GetKeys(keys);
	for (int i = 0; i < keys.Num(); i++)
	{
		FProcessInfo Info=*ProcessHandleMap.Find(keys[i]);
		FPlatformProcess::TerminateProc(Info.ProcessHandle);
	}
}
