// NP Game Developer


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/AuraLogChannel.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// Register tick function
	PrimaryActorTick.bCanEverTick = true;
	
	// Only enable tick later time
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("LevelUpNiagaraSystem"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	LevelUpParticlesMulticast();
}

void AAuraCharacter::LevelUpParticlesMulticast_Implementation() const
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	
	if (IsValid(LevelUpNiagaraComponent) && CameraManager)
	{
		const FVector CameraLocation = CameraManager->GetCameraLocation();
		const FVector LevelUpNiagaraLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - LevelUpNiagaraLocation).Rotation();
		
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetXP_Implementation()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerLevel);
	
	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	if (ASC)
	{
		ASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::EnableFacingTarget_Implementation(float InRotationSpeed)
{
	if (IsLocallyControlled() && GetCharacterMovement() && GetController())
	{
		bUpdateFacingTarget = true;
		RotateToTargetSpeed = InRotationSpeed;
		
		// Align controller's initial rotation with actor
		Controller->SetControlRotation(GetActorForwardVector().Rotation());
		
		// Only rotate yaw
		bUseControllerRotationYaw = true;
		
		// Make sure character not orient toward movement direction
		GetCharacterMovement()->bOrientRotationToMovement = false;
		
		// Start actor ticking
		SetActorTickEnabled(true);
		
		
		
	}
}

void AAuraCharacter::DisableFacingTarget_Implementation()
{
	if (IsLocallyControlled() && GetCharacterMovement() && GetController())
	{
		bUpdateFacingTarget = false;
		SetActorTickEnabled(false);
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AAuraCharacter::SetFacingTarget_Implementation(FVector InFacingTarget)
{
	TargetToFace = InFacingTarget;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/* Init ability actor info for server */
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/* Init ability actor info for client */
	InitAbilityActorInfo();
}

void AAuraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FacingTarget(DeltaTime);
}

void AAuraCharacter::ShockLoopTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::ShockLoopTagChanged(CallbackTag, NewCount);
	
	if (OnShcokLoopStateChangedDelegate.IsBound())
	{
		OnShcokLoopStateChangedDelegate.Broadcast(NewCount > 0);
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	
	/* For player character, assign ASC and AS pointer to the player character */
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	// Listen for stun tag changed
	AbilitySystemComponent->RegisterGameplayTagEvent(
		Debuff_Stun,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &AAuraCharacter::StunTagChanged);
	
	// Listen for shock loop tag changed
	AbilitySystemComponent->RegisterGameplayTagEvent(
		Event_Anim_ShockLoop,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &AAuraCharacter::ShockLoopTagChanged);
	
	// Init UI
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	/* Initialize character default attributes*/
	InitializeDefaultAttributes();
	
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void AAuraCharacter::FacingTarget(float DeltaTime)
{
	if (!bUpdateFacingTarget) return;
	if (Controller != nullptr)
	{
		/*
		 * Find out look rotation to target and zero out pitch and roll
		 */
		FRotator ToRotation = UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(), 
			TargetToFace
		);
		ToRotation.Pitch = 0.0f;
		ToRotation.Roll = 0.0f;
		
		/*
		 * Using Rotation Interpolation Constant to find out 
		 * delta rotation for this frame
		 */
		FRotator DeltaRotation = FMath::RInterpConstantTo(
			Controller->GetControlRotation(),
			ToRotation,
			DeltaTime,
			RotateToTargetSpeed
		);
		
		// Update controller rotation
		Controller->SetControlRotation(DeltaRotation);
	}
}


