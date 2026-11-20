// NP Game Developer


#include "Actors/ActorSphereDetector.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AActorSphereDetector::AActorSphereDetector()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create sphere collision component
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetSphereRadius(GetRadius(bUseScaledRadius));
	SphereCollisionComponent->SetGenerateOverlapEvents(true);
}

float AActorSphereDetector::GetRadius(bool bScaled) const
{
	if (bScaled)
	{
		// Return scaled radius by dividing collision component's shape scale by radius 
		return Radius / SphereCollisionComponent->GetShapeScale();
	}
	return Radius;
}

#if WITH_EDITOR
void AActorSphereDetector::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.GetPropertyName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AActorSphereDetector, Radius))
	{
		if (SphereCollisionComponent)
		{
			SphereCollisionComponent->SetSphereRadius(GetRadius(bUseScaledRadius));
		}
	}
}
#endif

void AActorSphereDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ActorToFollow)
	{
		SetActorLocation(ActorToFollow->GetActorLocation());
	}
}

void AActorSphereDetector::BeginPlay()
{
	Super::BeginPlay();
	
	ActorsToIgnore.Empty();
	ActorsToIgnore.AddUnique(this);
	
	if (ActorToFollow)
	{
		SetActorLocation(ActorToFollow->GetActorLocation());
	}
	
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->SetSphereRadius(GetRadius(bUseScaledRadius));
		SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AActorSphereDetector::EnableDetection(const TArray<AActor*>& InActorsToIgnore)
{
	bBroadcast = true;
	ActorsToIgnore.Empty();
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.Append(InActorsToIgnore);
	
	if (ActorToFollow)
	{
		SetActorLocation(ActorToFollow->GetActorLocation());
	}
	
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->SetSphereRadius(GetRadius(bUseScaledRadius));
		SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AActorSphereDetector::OnBeginOverlap);
		SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AActorSphereDetector::OnEndOverlap);
		SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AActorSphereDetector::DisableDetection()
{
	bBroadcast = false;
	ActorsToIgnore.Empty();
	ActorsToIgnore.AddUnique(this);
	
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
		SphereCollisionComponent->OnComponentEndOverlap.RemoveAll(this);
		SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

AActor* AActorSphereDetector::AddActorToIgnore(AActor* Actor)
{
	if (!ActorsToIgnore.Contains(Actor))
	{
		ActorsToIgnore.AddUnique(Actor);
		return Actor;
	}
	return nullptr;
}

AActor* AActorSphereDetector::RemoveActorFromIgnore(AActor* Actor)
{
	if (!ActorsToIgnore.Contains(Actor)) return nullptr;
	ActorsToIgnore.Remove(Actor);
	return Actor;
}

void AActorSphereDetector::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bBroadcast) return;
	if (ActorsToIgnore.Contains(OtherActor)) return;
	if (FilterClass && !OtherActor->IsA(FilterClass)) return;
	if (!OtherActor->GetClass()->ImplementsInterface(ConformToInterface)) return;
	
	if (OnActorEntered.IsBound())
	{
		OnActorEntered.Broadcast(OtherActor);
	}
}

void AActorSphereDetector::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bBroadcast) return;
	if (ActorsToIgnore.Contains(OtherActor)) return;
	if (FilterClass && !OtherActor->IsA(FilterClass)) return;
	if (!OtherActor->GetClass()->ImplementsInterface(ConformToInterface)) return;
	
	if (OnActorLeave.IsBound())
	{
		OnActorLeave.Broadcast(OtherActor);
	}
}

