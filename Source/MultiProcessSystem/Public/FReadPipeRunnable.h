#pragma once
#include "ProcessAsyncAction.h"

class FReadPipeRunnable :public FRunnable
{
public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	FReadPipeRunnable(FProcHandle inPorcHandle,void* InReadPipe,NativeOnProcessOutput BindOnProcessOutput,int32 inProcID,UObject* InWorldContext) : ProcHandle(inPorcHandle),ReadPipe(InReadPipe) ,OnProcessOutput(BindOnProcessOutput),ProcessID(inProcID),WorldContextObject(InWorldContext){}
protected:
	FProcHandle ProcHandle;
	void* ReadPipe;
	FString OutString;
	NativeOnProcessOutput OnProcessOutput;
	int32 ProcessID;
	UObject*WorldContextObject;
};
