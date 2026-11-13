// NP Game Developer


#include "Actors/ActorSphereDetector.h"

#include "Components/SphereComponent.h"


AActorSphereDetector::AActorSphereDetector()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create sphere collision component
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetSphereRadius(Radius);
	SphereCollisionComponent->SetGenerateOverlapEvents(true);
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
			SphereCollisionComponent->SetSphereRadius(Radius);
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
	
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->SetSphereRadius(Radius);
		SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AActorSphereDetector::EnableDetection(const TArray<AActor*>& InActorsToIgnore)
{
	bBroadcast = true;
	ActorsToIgnore.Empty();
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.Append(InActorsToIgnore);
	
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->SetSphereRadius(Radius);
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

