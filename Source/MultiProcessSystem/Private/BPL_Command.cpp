// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\BPL_Command.h"


bool UBPL_Command::CallCommand(FString inUrl, FString inParms, FString workpath, bool bLanchDetached,
                               bool bLanchHidden, bool bLanchReallyHidden)
{
	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;
	FPlatformProcess::CreatePipe(ReadPipe, WritePipe);
	FProcHandle ProcessHandle = FPlatformProcess::CreateProc(*inUrl, *inParms, bLanchDetached, bLanchHidden, bLanchReallyHidden, nullptr, 0, *workpath, WritePipe, ReadPipe);
	if (ProcessHandle.IsValid())
	{
		return true;
	}
	else
	{
		return false;
	}
}

FString UBPL_Command::GetEnvironment(const FString& VariableName)
{
	FString VariableValue;
	//TODO CrossPlatform
	return VariableValue;
}


