// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcessAsyncAction.h"

#include "FReadPipeRunnable.h"

UProcessAsyncAction* UProcessAsyncAction::CallProcess(UObject*WorldContext,FString Url, FString Parms, FString workpath, bool bLanchDetached,
                                          bool bLanchHidden, bool bLanchReallyHidden)
{
	UProcessAsyncAction* ProcAsyncAction = NewObject<UProcessAsyncAction>();
	
	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;
	FPlatformProcess::CreatePipe(ReadPipe, WritePipe);
	
	FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*Url, *Parms, bLanchDetached, bLanchHidden, bLanchReallyHidden, nullptr, 0, *workpath, WritePipe, ReadPipe);
	
	return ProcAsyncAction;
}
