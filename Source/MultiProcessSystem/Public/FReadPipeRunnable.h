#pragma once
#include "ProcessAsyncAction.h"
#include "Misc/SpinLock.h"

UE::FSpinLock MSpinLock;
FRWLock MRWLoac;
FCriticalSection MCriticalSection;
TAtomic<int32> Number;
class FReadPipeRunnable :public FRunnable
{
public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	FReadPipeRunnable(const FProcHandle& inPorcHandle,void* InReadPipe,NativeOnProcessOutput BindOnProcessOutput,int32 inProcID,UObject* InWorldContext=nullptr) : ProcHandle(inPorcHandle),ReadPipe(InReadPipe) ,OnProcessOutput(BindOnProcessOutput),ProcessID(inProcID),WorldContextObject(InWorldContext){}

	FReadPipeRunnable(): ReadPipe(nullptr), ProcessID(0), WorldContextObject(nullptr)
	{};
	FProcHandle ProcHandle;
	void* ReadPipe;
	void* WritePipe;
	FString OutString;
	NativeOnProcessOutput OnProcessOutput;
	int32 ProcessID;
	UObject*WorldContextObject;
	FString ReadThreadName;
	FRunnableThread* BelongThread;
};
