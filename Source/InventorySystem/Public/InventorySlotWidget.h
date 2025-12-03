// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UItemData;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

    // Bu slotun InventoryComponent içindeki GERÇEK index’i
    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
    int32 InventoryIndex = -1;

    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
    class UInventoryComponent* InventoryRef;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void UpdateSlot(const FInventorySlot& SlotData);
    virtual void UpdateSlot_Implementation(const FInventorySlot& SlotData);

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;

    virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;
};
