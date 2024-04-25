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
            location.Y = FMath::Tan(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f;
         
            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
            
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    for (AAgent* agent : Agents) {
        FVector finalVelocity = agent->Velocity;
        //Rule 1
        int count = 0;
        FVector centerOfMass = FVector(0.0f);
        for (AAgent* otherAgent : Agents) {
            if (agent != otherAgent) {
                if (abs((agent->GetActorLocation() - otherAgent->GetActorLocation()).Size()) < 1000) {
                    centerOfMass += otherAgent->GetActorLocation();
                    count++;
                }
            }
        }
        if (count != 0) {
            centerOfMass /= count;
        }
        finalVelocity += (centerOfMass - agent->GetActorLocation()) / 100;

        //Rule 2
        int index = 0;
        FVector distanceBetweenBoids = FVector(0.0f);
        for (AAgent* otherAgent : Agents) {
            if (agent != otherAgent) {
                if (abs((agent->GetActorLocation() - otherAgent->GetActorLocation()).Size()) < 100) {
                    index++;
                    UE_LOG(LogTemp, Warning, TEXT("x:%f, y:%f, z:%f"), (otherAgent->GetActorLocation() - agent->GetActorLocation()).X, (otherAgent->GetActorLocation() - agent->GetActorLocation()).Y, (otherAgent->GetActorLocation() - agent->GetActorLocation()).Z);
                    distanceBetweenBoids -= (otherAgent->GetActorLocation() - agent->GetActorLocation()) / 2;
                }
            }
        }
        finalVelocity += distanceBetweenBoids;


        //Rule 3
        FVector otherBoidVelocities = FVector(0.0f);
        for (AAgent* otherAgent : Agents) {
            if (agent != otherAgent) {
                otherBoidVelocities += otherAgent->Velocity;
            }
        }
        otherBoidVelocities /= (AGENT_COUNT - 1);
        finalVelocity += (distanceBetweenBoids - agent->Velocity) / 8;

        //End
        //agent->setVelocity(finalVelocity);
        FVector loc = agent->GetActorLocation();
        agent->SetActorLocation(loc + finalVelocity);
        //UE_LOG(LogTemp, Warning, TEXT("index:%d, x:%f, y:%f, z:%f"), index, agent->Velocity.X, agent->Velocity.Y, agent->Velocity.Z);
    }
 

    
}