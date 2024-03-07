// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPL_Command.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPROCESSSYSTEM_API UBPL_Command : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)
	static bool CallCommand(FString inUrl, FString inParms, FString workpath, bool bLanchDetached, bool bLanchHidden, bool bLanchReallyHidden);
	UFUNCTION(BlueprintCallable)
	FString GetEnvironment(const FString& VariableName);
};
