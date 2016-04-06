// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "LiquidFunPawn.generated.h"

class AParticleActor;
UCLASS()
class ORTHOTESTFLEX_API ALiquidFunPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALiquidFunPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Box", meta = (AllowPrivateAccess = "true"))
	virtual FVector GetBoxPosition();

	UFUNCTION(BlueprintCallable, Category = "Box", meta = (AllowPrivateAccess = "true"))
	virtual float GetBoxAngle();

private:
	void LFInitialize();

	void LFUpdate(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* BoxSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fluid", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* FluidSprite;

	TArray<AParticleActor*> ParticleArray;
};
