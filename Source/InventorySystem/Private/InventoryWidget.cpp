// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"



void UInventoryWidget::NativeConstruct()
{

    InventorySizeBox->SetVisibility(ESlateVisibility::Collapsed);
	InventoryText->SetVisibility(ESlateVisibility::Collapsed);

    // Eðer InventoryRef henüz yoksa slot oluþturma!!
    if (!InventoryRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryRef is null — waiting bind."));
        return;
    }

    InventoryRef->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::HandleInventoryUpdated);
    HandleInventoryUpdated();
}

void UInventoryWidget::HandleInventoryUpdated()
{
    if (!InventoryRef) return;

    for (auto* HotbarSlot : HotbarSlotWidgets)
    {
        HotbarSlot->UpdateSlot(InventoryRef->GetSlot(HotbarSlot->InventoryIndex));
    }

    for (auto* InventorySlot : InventorySlotWidgets)
    {
        InventorySlot->UpdateSlot(InventoryRef->GetSlot(InventorySlot->InventoryIndex));
    }
}

void UInventoryWidget::CreateInventorySlots()
{
    InventoryGrid->ClearChildren();
    InventorySlotWidgets.Empty();

    const int32 StartIndex = 5;
    const int32 Total = InventoryRef->Width * (InventoryRef->Height - 1);

    const int32 Width = 5;

    for (int32 i = 0; i < Total; ++i)
    {
        UInventorySlotWidget* InventorySlot =
            CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);

        InventorySlot->InventoryIndex = StartIndex + i;
        InventorySlot->InventoryRef = InventoryRef;

        InventorySlotWidgets.Add(InventorySlot);

        int32 Row = i / Width;
        int32 Col = i % Width;

        InventoryGrid->AddChildToUniformGrid(InventorySlot, Row, Col);
    }
}

void UInventoryWidget::CreateHotbarSlots()
{
    HotbarGrid->ClearChildren();
    HotbarSlotWidgets.Empty();

    const int32 StartIndex = 0;

    for (int32 i = 0; i < 5; ++i)
    {
        UInventorySlotWidget* InventorySlot =
            CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);

        InventorySlot->InventoryIndex = StartIndex + i; 
        InventorySlot->InventoryRef = InventoryRef;

        HotbarSlotWidgets.Add(InventorySlot);

        HotbarGrid->AddChildToUniformGrid(InventorySlot, 0, i);
    }
}

void UInventoryWidget::ToggleInventoryVisibility()
{
    if (InventorySizeBox->GetVisibility() == ESlateVisibility::Visible && InventoryText->GetVisibility() == ESlateVisibility::Visible)
    {
        InventorySizeBox->SetVisibility(ESlateVisibility::Collapsed);
		InventoryText->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        InventorySizeBox->SetVisibility(ESlateVisibility::Visible);
		InventoryText->SetVisibility(ESlateVisibility::Visible);
	}
}
