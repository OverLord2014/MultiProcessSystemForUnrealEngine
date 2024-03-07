// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ProcessAsyncAction.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(NativeOnProcessOutput,int32 ProcessID,FString OutString);
/**
 * 
 */
UCLASS()
class MULTIPROCESSSYSTEM_API UProcessAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	NativeOnProcessOutput NativeOnProcessOutput;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "MultiProcessSystem")
	static UProcessAsyncAction* CallProcess(UObject*WorldContext,FString Url, FString Parms, FString workpath, bool bLanchDetached, bool bLanchHidden, bool bLanchReallyHidden);
};
