// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProcessSubSystem.generated.h"

class FReadPipeRunnable;
/**
 * 
 */
UENUM(BlueprintType)
enum class EProcState:uint8
{
	CreateSuccess,
	AlreadyCreate,
	Failed
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProcessOutput,int32,ProcessID,FString,OutString);
USTRUCT()
struct FProcessInfo
{
	GENERATED_BODY()
	uint32 ProcessID;
	FProcHandle ProcessHandle;
	TSharedPtr<FRunnableThread> ProcessThread;
	FProcessInfo(): ProcessID(0){}
	FProcessInfo(uint32 InProcessID, FProcHandle InProcessHandle, TSharedPtr<FRunnableThread> InProcessThread)
		: ProcessID(InProcessID),
		  ProcessHandle(InProcessHandle),
		  ProcessThread(InProcessThread){}
};
UCLASS()
class MULTIPROCESSSYSTEM_API UProcessSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	EProcState StartProcess(FString inUrl, FString inParms, FString workpath, bool bLanchDetached, bool bLanchHidden, bool bLanchReallyHidden,bool bAllowMultiProc);
    
	UFUNCTION()
	void UpdateHandleState();

	UFUNCTION()
	void EventOnProcessOutPut(int32 ProcessID,FString DataString);
protected:
	UPROPERTY(BlueprintAssignable)
	FOnProcessOutput OnProcessOutput;
	TMap<FString,FProcessInfo>ProcessHandleMap;
	//TMap<FString,FReadPipeRunnable>ProcessReadMap;
};
