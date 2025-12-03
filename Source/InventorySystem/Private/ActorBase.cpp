// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase.h"
#include "Components/StaticMeshComponent.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "ItemData.h"
#include "InventoryComponent.h"

// Sets default values
AActorBase::AActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void AActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AActorBase::OnComponentBeginOverlap);
	MeshComponent->SetStaticMesh(ItemData->WorldMesh);

}

// Called every frame
void AActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* Character = Cast<AInventorySystemCharacter>(OtherActor);
	if (Character && ItemData)
	{
		Character->InventoryComp->AddItem(ItemData, 1);
		Destroy();
	}
}

