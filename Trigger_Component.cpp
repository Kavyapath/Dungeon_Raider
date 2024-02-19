// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Component.h"


UTrigger_Component::UTrigger_Component() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTrigger_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...


}
void UTrigger_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* Actor = GetAcceptableActor();
	if (Actor!= nullptr) {
		UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		UPrimitiveComponent* Component=Cast<UPrimitiveComponent>(Actor->GetRootComponent());//casting this pointer in to the uprimitive component because our actor does not know that it contain uprimitive component or not
		if (Component != nullptr) {
			Component->SetSimulatePhysics(false);
		}
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		Mover->SetShouldMove(true);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Relocking"));
		Mover->SetShouldMove(false);
	}
	
}
AActor* UTrigger_Component:: GetAcceptableActor() const {

	
	TArray<AActor*> Actors;  //contain the address of the array elememnt
	//TSubclassOf< AActor > ClassFilter;
	GetOverlappingActors(Actors);
	

	for (AActor* Actor : Actors) {

		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);//means unlock1 on goggvil
		bool HasGrabbed = Actor->ActorHasTag("Grabbed");
		if (HasAcceptableTag && !HasGrabbed) {
			
			return Actor;

		}
	}

	return nullptr;
}
void UTrigger_Component::SetMover(UMover* NewMover) {
	Mover = NewMover;
}