// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	//static ConstructorHelpers::FClassFinder<UUserWidget> UI(TEXT("WidgetBlueprint'/Game/_ShootAndLoot/UI/WBP_EnemyHP.WBP_EnemyHP_C'"));
	//if (UI.Succeeded())
	//{
	//	EnemyUI->SetWidgetClass(UI.Class);
	//}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
	if (OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Projectile")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Collision With Projectile"));
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

