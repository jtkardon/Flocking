// Fill out your copyright notice in the Description page of Project Settings.

#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 10    

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

    World = world;

    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;
         
            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
            
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    //Rule 1

    for (AAgent* agent : Agents) {
        FVector centerOfMass = FVector(0.0f);
        for (AAgent* otherAgent : Agents) {
            if (agent != otherAgent) {
                centerOfMass += otherAgent->GetActorLocation();
            }
        }

        centerOfMass /= AGENT_COUNT - 1;
        agent->setVelocity((centerOfMass - agent->GetActorLocation()) / 100);
        UE_LOG(LogTemp, Warning, TEXT("x:%f, y:%f, z:%f"), centerOfMass.X, centerOfMass.Y, centerOfMass.Z);
    }
 

    
    //UE_LOG(LogTemp, Warning, TEXT("x:%f, y:%f, z:%f"), centerOfMass.X, centerOfMass.Y, centerOfMass.Z);
}