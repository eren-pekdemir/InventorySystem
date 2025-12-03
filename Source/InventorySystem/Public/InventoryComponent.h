// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemData;	

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemData* ItemData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Width = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Height = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Capacity = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 ActiveHotbarIndex = 0;

	//FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemData* ItemData, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 ItemIndex, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapSlots(int32 IndexA, int32 IndexB);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemAtIndex(int32 ItemIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventorySlot GetSlot(int32 Index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetTotalSlots() const { return InventorySlots.Num(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 FindItemIndex(UItemData* ItemData) const;

	UFUNCTION(BlueprintCallable)
	void UseItem(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool UseActiveItem();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


		
};
