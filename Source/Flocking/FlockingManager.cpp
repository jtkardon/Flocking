// Fill out your copyright notice in the Description page of Project Settings.

#include "FlockingManager.h"
#include "Agent.h"
#include "FlockingGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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
        finalVelocity += (centerOfMass - agent->GetActorLocation()) / 1000;

        //Rule 2
        int index = 0;
        FVector distanceBetweenBoids = FVector(0.0f);
        for (AAgent* otherAgent : Agents) {
            if (agent != otherAgent) {
                if (abs((agent->GetActorLocation() - otherAgent->GetActorLocation()).Size()) < 200) {
                    index++;
                    distanceBetweenBoids -= (otherAgent->GetActorLocation() - agent->GetActorLocation()) / 3;
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
        finalVelocity += (otherBoidVelocities - agent->Velocity) / 8;

        //Stay in Bounding Box
        FVector distanceFromBox = FVector(0.0f);
        if (agent->GetActorLocation().X > 950) {
            distanceFromBox.X = -25;
        }
        else if ((agent->GetActorLocation().X < -950)) {
            distanceFromBox.X = 25;
        }
        if (agent->GetActorLocation().Y > 950) {
            distanceFromBox.Y = -25;
        }
        else if ((agent->GetActorLocation().Y < -950)) {
            distanceFromBox.Y = 25;
        }
        if (agent->GetActorLocation().Z > 950) {
            distanceFromBox.Z = -25;
        }
        else if ((agent->GetActorLocation().Z < -950)) {
            distanceFromBox.Z = 25;
        }

        finalVelocity += distanceFromBox;

        //End
        //agent->setVelocity(finalVelocity);
        agent->direction = finalVelocity;
        AFlockingGameModeBase* gmb = Cast<AFlockingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
        //direction.Normalize();
        //Velocity = direction * gmb->Speed;
        FVector loc = agent->GetActorLocation();
        agent->SetActorLocation(loc + finalVelocity);
        //UE_LOG(LogTemp, Warning, TEXT("index:%d, x:%f, y:%f, z:%f"), index, agent->Velocity.X, agent->Velocity.Y, agent->Velocity.Z);
    }
 

    
}