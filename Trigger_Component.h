// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "Trigger_Component.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTrigger_Component : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTrigger_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Blueprintcallable)
		//dependencis injection means it want that object to run its mechanism but its unable to find by its own;
		void SetMover(UMover* Mover);
private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	bool ShouldMove = false;

	UMover* Mover;

	AActor* GetAcceptableActor() const;

};
