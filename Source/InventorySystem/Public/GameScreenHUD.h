// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameScreenHUD.generated.h"



/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AGameScreenHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	void InitHUD();
	void BindInventory(class AInventorySystemCharacter* Character);

	virtual void BeginPlay() override;

};
