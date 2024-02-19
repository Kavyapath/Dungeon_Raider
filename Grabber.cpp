// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h" // for accessing getworld and other classes that we are seeing while playing the game

#include "DrawDebugHelpers.h" // for drawing the debug line in line tracing


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*FRotator MyRotation=GetComponentRotation();
	FString RotationString=MyRotation.ToCompactString();
	UWorld* World = GetWorld();
	float Time = World->TimeSeconds;*/
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent() ) {
		//here we are setting up the location and the rotation of the object when we are holding our object from the world
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());//we want the object rotatio as same as our main  view 
	}
	// ...
}
/*void UGrabber::PrintDamage(const float& Damage)//using const in front means now that you cannot change or modify the value of reference variable in the function so that when we are passing the refernce we are ensuring that our main variable data is safe and contain its original value
{
	UE_LOG(LogTemp, Display, TEXT("Damage : %f"), Damage);

}
bool UGrabber::HasDamage(float& OutDamage) {
	OutDamage = 5;
	return true;
}*/
void UGrabber::Release() {
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	//if our player has grabbed something

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* GetGrabbed = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GetGrabbed->Tags.Remove("Grabbed");//physicshandle is the object(component) and by using function getOwner we can find the main Actor to which component is attached and then apply tags on that
		PhysicsHandle->ReleaseComponent();//release the component that you have picked

		
	}
}
bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const {
	FVector Start = GetComponentLocation();

	FVector End = Start + GetForwardVector() * MaxGrabDistance;//in order to get the location of the end vector we will take a forward vector  using funtion GetForwardVector and multiply with our maxDistance varible then add up with the start vector which will give the final location of end vector
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);
	FHitResult HitResult;
	//Making sphere coliision
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	//sweeping the line trace
	return   GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
	
}
void UGrabber::Grab() {


	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) {
		return;

	}
	FHitResult HitResult;
	bool hasHit = GetGrabbableInReach(HitResult);

	if (hasHit) {//if line trace is hit we gone make our draw debug sphere
	/*	AActor* HitActor = HitResult.GetActor();//it hitresult will tell us which actor our sweep hit and return it the the AActor pointer name HitActor
		DrawDebugSphere(GetWorld(), HitResult.Location,10,10,FColor::Green,false,5 );
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
*/

		//to grab any object from the physical world
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);// to simulate physics in the golden statue
		AActor* HitActor = HitResult.GetActor();
		HitComponent->WakeAllRigidBodies();//waking up all rigid bodies
		HitActor->Tags.Add("Grabbed");//Tags are the Tarray add  is the functionality of tarray go through the documentation
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()



			);
		
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No Name"));
	}
}
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const{

		
		UPhysicsHandleComponent * Result=GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if (Result == nullptr) {

		}
		return Result;

}


