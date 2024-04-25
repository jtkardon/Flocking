// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "FlockingGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AAgent::AAgent() {
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AgentMesh"));
	RootComponent = Mesh;
	Velocity = FVector(0.f);
	
}

void AAgent::BeginPlay() {
	Super::BeginPlay();
}

void AAgent::Init(UStaticMeshComponent* mesh, int id) {
	UE_LOG(LogTemp, Warning, TEXT("Agent initialized."));
	Mesh->SetStaticMesh(mesh->GetStaticMesh());
	direction = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	direction.Normalize();
}

void AAgent::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AFlockingGameModeBase* gmb = Cast<AFlockingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	direction.Normalize();
	Velocity = direction * gmb->Speed;


}

void AAgent::setVelocity(FVector newVelocity) {
	Velocity = newVelocity;
}
