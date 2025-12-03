// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemData.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InventorySlots.SetNum(Capacity);
}

bool UInventoryComponent::AddItem(UItemData* ItemData, int32 Quantity)
{
	if (ItemData == nullptr) return false;
	 
	for (FInventorySlot& Item : InventorySlots)
	{
		if (Item.ItemData != nullptr && Item.ItemData->Name.ToString() == ItemData->Name.ToString() && Item.ItemData->MaxStackSize > 1)
		{
			if(Item.ItemData->MaxStackSize >= Item.Quantity + Quantity)
			{
				Item.Quantity += Quantity;
				OnInventoryUpdated.Broadcast(); 
				return true;
			}
			
		}
		else
		{
			if (Item.ItemData == nullptr)
			{
				Item.ItemData = ItemData;
				Item.Quantity = Quantity;
				OnInventoryUpdated.Broadcast(); 
				return true;
			}

		}
			
	}

	return false;
}

FInventorySlot UInventoryComponent::GetSlot(int32 Index) const
{
	if (!InventorySlots.IsValidIndex(Index))
	{
		return FInventorySlot();
	}

	return InventorySlots[Index];
}

int32 UInventoryComponent::FindItemIndex(UItemData* ItemData) const
{
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i].ItemData == ItemData)
		{
			return i;
		}
	}
	return int32();
}

bool UInventoryComponent::RemoveItem(int32 ItemIndex, int32 Quantity)
{
	
	if (!InventorySlots.IsValidIndex(ItemIndex)) return false;

	FInventorySlot& Slot = InventorySlots[ItemIndex];

	if (!Slot.ItemData) return false;

	Slot.Quantity -= Quantity;

	if (Slot.Quantity <= 0)
	{
		Slot.Quantity = 0;
		Slot.ItemData = nullptr;
	}

	OnInventoryUpdated.Broadcast();
	return true;
	
}

void UInventoryComponent::SwapSlots(int32 IndexA, int32 IndexB)
{
	if (!InventorySlots.IsValidIndex(IndexA) || !InventorySlots.IsValidIndex(IndexB))
	{
		return;
	}

	if (IndexA == IndexB) return;

	auto& SlotA = InventorySlots[IndexA];
	auto& SlotB = InventorySlots[IndexB];

	// Eðer iki slot da boþ ? hiçbir þey yapma
	if (!SlotA.ItemData && !SlotB.ItemData)
	{
		return;
	}

	// ?? Eðer A dolu ama B boþ ? taþý
	if (SlotA.ItemData && !SlotB.ItemData)
	{
		SlotB = SlotA;
		SlotA.ItemData = nullptr;
		SlotA.Quantity = 0;
		OnInventoryUpdated.Broadcast();
		return;
	}

	// ?? Eðer B dolu ama A boþ ? taþý
	if (SlotB.ItemData && !SlotA.ItemData)
	{
		SlotA = SlotB;
		SlotB.ItemData = nullptr;
		SlotB.Quantity = 0;
		OnInventoryUpdated.Broadcast();
		return;
	}

	// ?? Her iki slot da doluysa ? Stack Merge veya Swap
	if (SlotA.ItemData->Name.ToString() == SlotB.ItemData->Name.ToString()) 
	{
		int32 TotalQuantity = SlotA.Quantity + SlotB.Quantity;
		int32 MaxStackSize = SlotB.ItemData->MaxStackSize;

		if (TotalQuantity <= MaxStackSize)
		{
			SlotB.Quantity = TotalQuantity;
			SlotA.ItemData = nullptr;
			SlotA.Quantity = 0;
		}
		else
		{
			SlotB.Quantity = MaxStackSize;
			SlotA.Quantity = TotalQuantity - MaxStackSize;
		}

		OnInventoryUpdated.Broadcast();
		return;
	}

	InventorySlots.Swap(IndexA, IndexB);
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::UseItemAtIndex(int32 ItemIndex)
{
	if(InventorySlots.IsValidIndex(ItemIndex))
	{
		if (InventorySlots[ItemIndex].ItemData)
		{
			InventorySlots[ItemIndex].ItemData->Use(GetOwner());
			RemoveItem(ItemIndex, 1);
		}
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	const int32 Total = Width * Height;
	InventorySlots.SetNum(Total);

	for (int32 i = 0; i < Total; ++i)
	{
		InventorySlots[i].ItemData = nullptr;
		InventorySlots[i].Quantity = 0;
	}
	
}


void UInventoryComponent::UseItem(int32 Index)
{
	if (!InventorySlots.IsValidIndex(Index)) return;

	FInventorySlot& Slot = InventorySlots[Index];
	if (!Slot.ItemData) return;

	Slot.ItemData->Use(GetOwner());

	Slot.Quantity -= 1;
	if (Slot.Quantity <= 0)
	{
		Slot.Quantity = 0;
		Slot.ItemData = nullptr;
	}

	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::UseActiveItem()
{
	UseItem(ActiveHotbarIndex);
	return true;
}
