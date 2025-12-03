// Fill out your copyright notice in the Description page of Project Settings.


#include "GameScreenHUD.h"
#include "InventoryWidget.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "InventoryComponent.h"


void AGameScreenHUD::BeginPlay()
{
    Super::BeginPlay();
    InitHUD();
}

void AGameScreenHUD::InitHUD()
{
    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
        InventoryWidget->AddToViewport(10);
        InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AGameScreenHUD::BindInventory(AInventorySystemCharacter* Character)
{
    if (!Character || !Character->InventoryComp) return;

    if (InventoryWidget)
    {
        InventoryWidget->InventoryRef = Character->InventoryComp;
		InventoryWidget->InventoryRef->OnInventoryUpdated.AddDynamic(InventoryWidget, &UInventoryWidget::HandleInventoryUpdated);
		InventoryWidget->CreateInventorySlots();
		InventoryWidget->CreateHotbarSlots();
        InventoryWidget->HandleInventoryUpdated();
    }
                
    // HUD artýk inventory güncellemelerini dinlesin
    Character->InventoryComp->OnInventoryUpdated.Broadcast();
}