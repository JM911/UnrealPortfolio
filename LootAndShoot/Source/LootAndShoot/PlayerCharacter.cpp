// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Projectile.h"

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "Components/TextBlock.h"

#include "PlayerInventoryWidget.h"

#include "PlayerStatComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// ī�޶� ����
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	//FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	// �� �޽� ����
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetupAttachment(FirstPersonCamera);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;

	// �� �޽� ����
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	// ����ü �߻� ��ġ ����� ������Ʈ ����
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);

	// ���� ����
	StatComponent = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("Stat Component"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �� �޽� ����
	GunMesh->AttachToComponent(HandsMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("GripPoint"));

	// ���� �ʱ�ȭ
	StatUpdate();
	//CurrentHp = MaxHp;

	// ������
	Reload();

	// �κ��丮 ���� ����
	if (InvenWidgetClass)
	{
		InvenWidget = Cast<UPlayerInventoryWidget>(CreateWidget(GetWorld(), InvenWidgetClass));
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	if (bInventoryToggle)
		return;

	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	if (bInventoryToggle)
		return;

	AddControllerPitchInput(Value);
}

void APlayerCharacter::FireProjectile()
{
	// TODO: ���߿� ���������� �߻��ϵ��� ����?
	if (bInventoryToggle)
		return;

	UWorld* World = GetWorld();
	if (World && MuzzleLocation)
	{
		FRotator SpawnRotation = GetControlRotation();
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		World->SpawnActor<AProjectile>(ProjectileClass,
			SpawnLocation, SpawnRotation, ActorSpawnParams);

		// TODO: ����, �ִϸ��̼� ����
	}
}

void APlayerCharacter::FireButtonPressed()
{
	if (bReloading)
		return;

	bFireButtonPressed = true;
}

void APlayerCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void APlayerCharacter::FireNextReady()
{
	bFireReady = true;
}

void APlayerCharacter::FireFinal()
{
	if (bFireButtonPressed && bFireReady)
	{
		// ��ź üũ
		if (CurrentMana <= 0)
			return;

		bFireReady = false;
		FireProjectile();
		GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&APlayerCharacter::FireNextReady,
			FireInterval);

		// ��ź ������Ʈ
		CurrentMana--;

		// ����׿�
		//UE_LOG(LogTemp, Warning, TEXT("%d / %d , %d"), CurrentMana, MagazineMana, TotalMana);
	}
}

void APlayerCharacter::Reload()
{
	if (bReloading || CurrentMana == MagazineMana)
		return;

	bReloading = true;

	GetWorldTimerManager().SetTimer(
		ReloadTimer,
		this,
		&APlayerCharacter::ReloadEnd,
		ReloadTime);
}

void APlayerCharacter::ReloadEnd()
{
	bReloading = false;

	// ���� ����
	int32 BulletDiff = MagazineMana - CurrentMana;
	CurrentMana = MagazineMana;
	TotalMana -= BulletDiff;

	// ����׿�
	//UE_LOG(LogTemp, Warning, TEXT("Realod Complete!"));
}

void APlayerCharacter::UpdatePlayerHUD()
{
	AMyGameMode* CurGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CurGameMode)
	{
		UPlayerHUD* CurHUD = Cast<UPlayerHUD>(CurGameMode->GetCurrentWidget());
		if (CurHUD)
		{
			const FString AmmoStr = FString::Printf(TEXT("Ammo %02d/%02d\nTotal %03d"),
				CurrentMana, MagazineMana, TotalMana);

			CurHUD->GetAmmoText()->SetText(FText::FromString(AmmoStr));
		}
	}
}

void APlayerCharacter::TakeItem()
{
	TArray<AActor*> OverlappingActorArray;

	GetOverlappingActors(OverlappingActorArray, APotionItem::StaticClass());

	for (auto CurActor : OverlappingActorArray)
	{
		APotionItem* Potion = Cast<APotionItem>(CurActor);
		if (Potion)
		{
			// 1. ������ ���� ����
			FItemStat PotionStat = Potion->GetItemStat();

			// 2. �κ��丮�� �̹� �ִ��� �˻�
			TPair<FItemStat, int32>* FindResult = Inventory.Find(PotionStat.Type);

			// 3-1. ������ ���� ����
			if (FindResult)
			{
				FindResult->Value++;
			}

			// 3-2. ������ �߰�
			else
			{
				TPair<FItemStat, int32> ItemInfoAndQuantity{ Potion->GetItemStat(), 1 };
				Inventory.Add(MakeTuple(PotionStat.Type, ItemInfoAndQuantity));			
			}

			// 4. �ش� �����ۿ� �ִ� ��ȣ�ۿ� �Լ� ȣ��(���⼭ �ı�)
			Potion->Interact();

			// �ϳ��� ��� ����
			break;
		}
	}

	// ���� ���� ����
	if (InvenWidget)
	{
		//InvenWidget->InvenTextureArray.Empty();
		InvenWidget->InvenQuantityArray.Empty();
		InvenWidget->InvenItemStatArray.Empty();

		for (auto Item : Inventory)
		{
			// TODO: �׳� ������ ���� ��� �ѱ�� �ɷ� ����
			//InvenWidget->InvenTextureArray.Add(Item.Value.Key.Icon);
			InvenWidget->InvenQuantityArray.Add(Item.Value.Value);
			InvenWidget->InvenItemStatArray.Add(Item.Value.Key);
		}

		// ������Ʈ ���� ����
		InvenWidget->bChanged = true;
	}
}

void APlayerCharacter::InventoryWidgetToggle()
{
	if (InvenWidget)
	{
		if (bInventoryToggle)
		{
			InvenWidget->RemoveFromParent();
			bInventoryToggle = false;

			// ���콺 �Ⱥ��̰�
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
		else
		{
			InvenWidget->AddToViewport();
			bInventoryToggle = true;

			// ���콺 ���̰�
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}
}

void APlayerCharacter::StatUpdate()
{
	// ���� ����, ���� �� ����Ʈ �Ҹ� �� ȣ��
	if (StatComponent)
	{
		MaxHp = StatComponent->GetCurrentStat(EPlayerStatType::MAX_HP);
		MagazineMana = StatComponent->GetCurrentStat(EPlayerStatType::MANA_MAGAZINE);
		TotalMana = StatComponent->GetCurrentStat(EPlayerStatType::MANA_TOTAL);

		Attack = StatComponent->GetCurrentStat(EPlayerStatType::ATTACK);
		FireInterval = StatComponent->GetCurrentStat(EPlayerStatType::FIRE_INTERVAL);
		MoveSpeed = StatComponent->GetCurrentStat(EPlayerStatType::MOVE_SPEED);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����ü ���� üũ
	FireFinal();
	
	// ź�� ���� UI ������Ʈ
	UpdatePlayerHUD();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);
	
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::TakeItem);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &APlayerCharacter::InventoryWidgetToggle);
}

