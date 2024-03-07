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
}

EProcState UProcessSubSystem::StartProcess(FString inUrl, FString inParms, FString workpath, bool bLanchDetached, bool bLanchHidden,
                             bool bLanchReallyHidden,bool bAllowMultiProc)
{
	UpdateHandleState();
	if (ProcessHandleMap.Find(inUrl)&&!bAllowMultiProc)
	{
		return EProcState::AlreadyCreate;
	}
	uint32* ProcID=nullptr;
	void* WritePipe=nullptr;
	void* ReadPipe=nullptr;
	FPlatformProcess::CreatePipe(ReadPipe,WritePipe);
	
	FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*inUrl, *inParms, bLanchDetached, bLanchHidden, bLanchReallyHidden, ProcID, 0, *workpath, WritePipe, ReadPipe);
	if (ProcessHandle.IsValid())
	{
		NativeOnProcessOutput NativeProcessOutput;
		NativeProcessOutput.AddUObject(this,&UProcessSubSystem::EventOnProcessOutPut);
		FReadPipeRunnable* ReadPipeRunnable = new FReadPipeRunnable(ProcessHandle, ReadPipe, NativeProcessOutput, *ProcID, nullptr);
		TSharedPtr<FRunnableThread> MyProcessThread = MakeShareable(FRunnableThread::Create(ReadPipeRunnable, TEXT("MyProcessThread")));
		ProcessHandleMap.Add(inUrl,FProcessInfo(*ProcID,ProcessHandle,MyProcessThread));
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
	OnProcessOutput.Broadcast(ProcessID,DataString);
}
