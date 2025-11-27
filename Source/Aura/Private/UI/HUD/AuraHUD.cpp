// NP Game Developer


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = CreateWidgetController<UOverlayWidgetController>(
			OverlayWidgetControllerClass,
			WCParams
			);
		/*OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();*/
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = CreateWidgetController<UAttributeMenuWidgetController>(
			AttributeMenuWidgetControllerClass,
			WCParams
			);
		/*AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();*/
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out in BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out in BP_AuraHUD"));

	/* Create widget */
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	/* Create widget controller */
	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	/* Connect widget controller to widget */
	OverlayWidget->SetWidgetController(WidgetController);

	/* Tell widget controller to broadcast initial values */
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

template <typename T>
T* AAuraHUD::CreateWidgetController(TSubclassOf<UAuraWidgetController> WidgetControllerClass,
	const FWidgetControllerParams& WCParams)
{
	check(WidgetControllerClass);
	
	UAuraWidgetController* WidgetController = NewObject<UAuraWidgetController>(this, WidgetControllerClass);
	WidgetController->SetWidgetControllerParams(WCParams);
	WidgetController->BindCallbacksToDependencies();
	return Cast<T>(WidgetController);
}