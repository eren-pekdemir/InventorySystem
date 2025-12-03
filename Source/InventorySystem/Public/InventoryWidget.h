// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
class UInventorySlotWidget;
class USizeBox;
class UTextBlock;


/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    // BP'den spawn ederken verilecek
    UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
    UInventoryComponent* InventoryRef;

    // Grid panel (Designer'da BindWidget ile baðlayacaksýn)
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* InventoryGrid;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* HotbarGrid;

    UPROPERTY(meta = (BindWidget))
    USizeBox* InventorySizeBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* InventoryText;

    
    // Slot widget blueprint class'ý
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

    // Oluþturduðumuz slot widget'larýn ref'leri
    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<UInventorySlotWidget*> InventorySlotWidgets;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<UInventorySlotWidget*> HotbarSlotWidgets;

    void CreateInventorySlots();

    void CreateHotbarSlots();

	void ToggleInventoryVisibility();

    UFUNCTION()
    void HandleInventoryUpdated();

protected:
    virtual void NativeConstruct() override;
};
