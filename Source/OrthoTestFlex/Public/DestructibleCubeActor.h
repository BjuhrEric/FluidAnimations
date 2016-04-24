// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DestructibleCubeActor.generated.h"

const int32 TYPE_DESTRUCTIBLE_CELL = 0;
const int32 TYPE_EMPTY_CELL = 1;
const int32 TYPE_INDESTRUCTIBLE_CELL = 2;

UCLASS()
class ORTHOTESTFLEX_API ADestructibleCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleCubeActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Type") void SetType(int32 type);

	UFUNCTION(BlueprintCallable, Category = "Type") int32 GetType();

private:
	int32 type;
};
