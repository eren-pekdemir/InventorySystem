// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryDragDropOperation.h"
#include "InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventorySystem/Public/ItemData.h"



void UInventorySlotWidget::UpdateSlot_Implementation(const FInventorySlot& SlotData)
{
    

}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (!InventoryRef || InventoryIndex < 0)
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // Sağ tık → item kullan
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        InventoryRef->UseItem(InventoryIndex);
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    }

    // Sol tık → drag başlangıcı
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton);
        return EventReply.NativeReply;
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

    if (!InventoryRef || InventoryIndex < 0) return;

    if (!InventoryRef->InventorySlots.IsValidIndex(InventoryIndex)) return;

    const FInventorySlot& InventorySlot = InventoryRef->InventorySlots[InventoryIndex];
    if (!InventorySlot.ItemData) return;

    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->SourceIndex = InventoryIndex;

    // Ghost görsel
    UInventorySlotWidget* DragVisual = CreateWidget<UInventorySlotWidget>(GetWorld(), GetClass());
    DragVisual->InventoryIndex = InventoryIndex;
    DragVisual->InventoryRef = InventoryRef;
    DragVisual->UpdateSlot(InventorySlot);
    DragVisual->SetVisibility(ESlateVisibility::Visible);

    DragOp->DefaultDragVisual = DragVisual;
    DragOp->Pivot = EDragPivot::MouseDown;

    OutOperation = DragOp;
}


bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
    SetRenderOpacity(1.0f);

    UInventoryDragDropOperation* DragOp = Cast<UInventoryDragDropOperation>(Operation);
    if (!DragOp || !InventoryRef || InventoryIndex < 0)
        return false;

    int32 FromIndex = DragOp->SourceIndex;
    int32 ToIndex = InventoryIndex;

    if (FromIndex == ToIndex)
        return false;

    SetVisibility(ESlateVisibility::Visible);
    InventoryRef->SwapSlots(FromIndex, ToIndex);
    return true;
   
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    SetVisibility(ESlateVisibility::Visible);
    SetRenderOpacity(1.0f);
}
