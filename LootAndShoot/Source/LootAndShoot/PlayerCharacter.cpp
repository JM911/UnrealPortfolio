// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Projectile.h"

#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "Components/TextBlock.h"

#include "PlayerInventoryWidget.h"
#include "PlayerStatWidget.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/ProgressBar.h"

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
	AllStatUpdate();
	//CurrentHp = MaxHp;

	// ������
	Reload();

	// �κ��丮 ���� ����
	if (InvenWidgetClass)
	{
		InvenWidget = Cast<UPlayerInventoryWidget>(CreateWidget(GetWorld(), InvenWidgetClass));
	}

	// �κ��丮 ���� �ʱ�ȭ
	if (InvenWidget)
	{
		InvenWidget->MyInit();
	}

	// ���� ���� ����
	if (StatWidgetClass)
	{
		StatWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));
	}

	// �׽�Ʈ�� ���� �� ����Ʈ ����
	StatComponent->PlayerLevelUp(30);
	UE_LOG(LogTemp, Warning, TEXT("Current Stat Point: %d"), StatComponent->GetLevelUpPoint());

	// ���� ���� �ʱ�ȭ
	if (StatWidget)
	{
		// �ʱ�ȭ

		// �ִ� ���� ����
		StatWidget->MyInit(StatComponent->GetMaxLevel());

		// ���� �ʱ�ȭ => enum ��ȸ ��� ������?
		StatWidgetUpdate(EPlayerStatType::MAX_HP);
		StatWidgetUpdate(EPlayerStatType::MANA_MAGAZINE);
		StatWidgetUpdate(EPlayerStatType::MANA_TOTAL);
		StatWidgetUpdate(EPlayerStatType::ATTACK);
		StatWidgetUpdate(EPlayerStatType::FIRE_INTERVAL);
		StatWidgetUpdate(EPlayerStatType::MOVE_SPEED);

		// ���� �� ����Ʈ �ʱ�ȭ
		StatWidgetLevelUpPointUpdate();
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
	if (bInventoryToggle || bStatWidgetToggle)
		return;

	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	if (bInventoryToggle || bStatWidgetToggle)
		return;

	AddControllerPitchInput(Value);
}

void APlayerCharacter::FireProjectile()
{
	// TODO: ���߿� ���������� �߻��ϵ��� ����?
	UWorld* World = GetWorld();
	if (World && MuzzleLocation)
	{
		FRotator SpawnRotation = GetControlRotation();
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass,
			SpawnLocation, SpawnRotation, ActorSpawnParams);

		// ���ݷ� ����
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetDamageValue(Attack);
		}

		// TODO: ����, �ִϸ��̼� ����
	}
}

void APlayerCharacter::FireButtonPressed()
{
	if (bInventoryToggle || bStatWidgetToggle)
		return;

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
	CurrentTotalMana -= BulletDiff;

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
			// ����׿� ���� �ؽ�Ʈ ������Ʈ
			const FString AmmoStr = FString::Printf(TEXT("Ammo %02d/%02d\nTotal %03d"),
				CurrentMana, MagazineMana, CurrentTotalMana);
			CurHUD->GetAmmoText()->SetText(FText::FromString(AmmoStr));

			// HP, MP, EXP �ؽ�Ʈ
			const FString HpStr = FString::Printf(TEXT("%d / %d"),
				(int)CurrentHp, (int)MaxHp);
			CurHUD->GetHpBarText()->SetText(FText::FromString(HpStr));

			const FString TotalManaStr = FString::Printf(TEXT("%d / %d"),
				CurrentTotalMana, MaxTotalMana);
			CurHUD->GetTotalManaBarText()->SetText(FText::FromString(TotalManaStr));

			const FString ManaMagazineStr = FString::Printf(TEXT("%d / %d"),
				CurrentMana, MagazineMana);
			CurHUD->GetManaMagazineBarText()->SetText(FText::FromString(ManaMagazineStr));

			const FString ExpStr = FString::Printf(TEXT("%d / %d"),
				(int)CurrentExp, (int)NextLevelExp);
			CurHUD->GetExpBarText()->SetText(FText::FromString(ExpStr));

			// HP, MP, EXP �� ������Ʈ
			float HpPercent = CurrentHp / MaxHp;
			float TotalMpPercent = (float)CurrentTotalMana / (float)MaxTotalMana;
			float MpMagazinePercent = (float)CurrentMana / (float)MagazineMana;
			float ExpPercent = CurrentExp / NextLevelExp;

			CurHUD->GetHpBar()->SetPercent(HpPercent);
			CurHUD->GetTotalMpBar()->SetPercent(TotalMpPercent);
			CurHUD->GetMpMagazineBar()->SetPercent(MpMagazinePercent);
			CurHUD->GetExpBar()->SetPercent(ExpPercent);
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
		InvenWidget->InvenQuantityArray.Empty();
		InvenWidget->InvenItemStatArray.Empty();

		// ������ ���� üũ
		int ItemCount = 0;

		// �� ĭ�� ���� ���� ������ ����
		FItemStat DummyItemStat;
		DummyItemStat.Type = EPotionType::NONE;
		DummyItemStat.Name = FString("");
		DummyItemStat.Icon = nullptr;
		DummyItemStat.Description = FString("");

		// �����ϴ� ������ �� ��ŭ ä���ֱ�
		for (auto Item : Inventory)
		{
			InvenWidget->InvenQuantityArray.Add(Item.Value.Value);
			InvenWidget->InvenItemStatArray.Add(Item.Value.Key);
			ItemCount++;
		}

		// ������ ��� ��ĭ���� ���� (���̵�����, 0)
		//for (int i = ItemCount; i < 18; i++)
		//{
		//	InvenWidget->InvenQuantityArray.Add(0);
		//	InvenWidget->InvenItemStatArray.Add(DummyItemStat);
		//}

		// ������Ʈ ���� ����
		InvenWidget->bChanged = true;
	}
}

void APlayerCharacter::InventoryWidgetToggle()
{
	// ���� ���� â�� �������� ��ŵ
	if (bStatWidgetToggle)
		return;

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

void APlayerCharacter::StatUpdate(EPlayerStatType Type)
{
	if (StatComponent)
	{
		switch (Type)
		{
		case EPlayerStatType::MAX_HP:
			MaxHp = StatComponent->GetCurrentStat(EPlayerStatType::MAX_HP);
			CurrentHp = MaxHp;
			break;
		case EPlayerStatType::MANA_MAGAZINE:
			MagazineMana = StatComponent->GetCurrentStat(EPlayerStatType::MANA_MAGAZINE);
			break;
		case EPlayerStatType::MANA_TOTAL:
			MaxTotalMana = StatComponent->GetCurrentStat(EPlayerStatType::MANA_TOTAL);
			CurrentTotalMana = MaxTotalMana;
			break;
		case EPlayerStatType::ATTACK:
			Attack = StatComponent->GetCurrentStat(EPlayerStatType::ATTACK);
			break;
		case EPlayerStatType::FIRE_INTERVAL:
			FireInterval = StatComponent->GetCurrentStat(EPlayerStatType::FIRE_INTERVAL);
			break;
		case EPlayerStatType::MOVE_SPEED:
			MoveSpeedRate = StatComponent->GetCurrentStat(EPlayerStatType::MOVE_SPEED);
			GetCharacterMovement()->MaxWalkSpeed = BaseSpeed * MoveSpeedRate;
			break;
		default:
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("%d / %d, %d, %d, %d, %d, %d"),
			StatComponent->GetLevelUpPoint(),
			StatComponent->GetStatLevel(EPlayerStatType::MAX_HP),
			StatComponent->GetStatLevel(EPlayerStatType::MANA_MAGAZINE),
			StatComponent->GetStatLevel(EPlayerStatType::MANA_TOTAL),
			StatComponent->GetStatLevel(EPlayerStatType::ATTACK),
			StatComponent->GetStatLevel(EPlayerStatType::FIRE_INTERVAL),
			StatComponent->GetStatLevel(EPlayerStatType::MOVE_SPEED));
	}
}

void APlayerCharacter::AllStatUpdate()
{
	if (StatComponent)
	{
		StatUpdate(EPlayerStatType::MAX_HP);
		StatUpdate(EPlayerStatType::MANA_MAGAZINE);
		StatUpdate(EPlayerStatType::MANA_TOTAL);
		StatUpdate(EPlayerStatType::ATTACK);
		StatUpdate(EPlayerStatType::FIRE_INTERVAL);
		StatUpdate(EPlayerStatType::MOVE_SPEED);
	}
}

void APlayerCharacter::LevelUpMaxHp()
{
	LevelUpStat(EPlayerStatType::MAX_HP);
}

void APlayerCharacter::LevelUpManaMagazine()
{
	LevelUpStat(EPlayerStatType::MANA_MAGAZINE);
}

void APlayerCharacter::LevelUpManaToTal()
{
	LevelUpStat(EPlayerStatType::MANA_TOTAL);
}

void APlayerCharacter::LevelUpAttack()
{
	LevelUpStat(EPlayerStatType::ATTACK);
}

void APlayerCharacter::LevelUpFireInterval()
{
	LevelUpStat(EPlayerStatType::FIRE_INTERVAL);
}

void APlayerCharacter::LevelUpMoveSpeed()
{
	LevelUpStat(EPlayerStatType::MOVE_SPEED);
}

void APlayerCharacter::StatWidgetUpdate(EPlayerStatType Type)
{
	if (StatWidget)
	{
		StatWidget->SetStatChanged(Type, StatComponent->GetStatLevel(Type));
		StatWidgetLevelUpPointUpdate();
	}
}

void APlayerCharacter::StatWidgetLevelUpPointUpdate()
{
	if (StatWidget)
	{
		const FString LevelUpPointStr = FString::Printf(TEXT("Level Up Point: %02d"),
			StatComponent->GetLevelUpPoint());

		StatWidget->GetLevelUpPointText()->SetText(FText::FromString(LevelUpPointStr));
	}
}

void APlayerCharacter::StatWidgetToggle()
{
	// ���� �κ��丮 â�� �������� ��ŵ
	if (bInventoryToggle)
		return;

	if (StatWidget)
	{
		if (bStatWidgetToggle)
		{
			StatWidget->RemoveFromParent();
			bStatWidgetToggle = false;

			// ���콺 �Ⱥ��̰�
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
		else
		{
			StatWidget->AddToViewport();
			bStatWidgetToggle = true;

			// ���콺 ���̰�
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
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

	PlayerInputComponent->BindAction("Stat", IE_Pressed, this, &APlayerCharacter::StatWidgetToggle);

	PlayerInputComponent->BindAction("Test_1", IE_Pressed, this, &APlayerCharacter::LevelUpMaxHp);
	PlayerInputComponent->BindAction("Test_2", IE_Pressed, this, &APlayerCharacter::LevelUpManaMagazine);
	PlayerInputComponent->BindAction("Test_3", IE_Pressed, this, &APlayerCharacter::LevelUpManaToTal);
	PlayerInputComponent->BindAction("Test_4", IE_Pressed, this, &APlayerCharacter::LevelUpAttack);
	PlayerInputComponent->BindAction("Test_5", IE_Pressed, this, &APlayerCharacter::LevelUpFireInterval);
	PlayerInputComponent->BindAction("Test_6", IE_Pressed, this, &APlayerCharacter::LevelUpMoveSpeed);
}

void APlayerCharacter::AddHp(float Amount)
{
	CurrentHp += Amount;
	if (CurrentHp > MaxHp)
	{
		CurrentHp = MaxHp;
	}
}

void APlayerCharacter::AddMana(float Amount)
{
	CurrentTotalMana += Amount;
	if (CurrentTotalMana > MaxTotalMana)
	{
		CurrentTotalMana = MaxTotalMana;
	}
}

void APlayerCharacter::AddExp(float Amount)
{
	CurrentExp += Amount;
	if (CurrentExp > NextLevelExp)
	{
		int8 LevelUpPoint = CurrentExp / NextLevelExp;
		float LeftExp = CurrentExp - (LevelUpPoint * NextLevelExp);
		
		StatComponent->PlayerLevelUp(LevelUpPoint);
		CurrentExp = LeftExp;

		// ���� �ؽ�Ʈ ������Ʈ
		StatWidgetLevelUpPointUpdate();
	}
}

void APlayerCharacter::LevelUpStat(EPlayerStatType Type)
{
	StatComponent->LevelUpStat(Type);
	StatUpdate(Type);

	// ���� ���� ������Ʈ
	StatWidgetUpdate(Type);
}

void APlayerCharacter::DrinkPotion(EPotionType Type)
{
	UE_LOG(LogTemp, Warning, TEXT("Drink Poition Type: %s"), *GetEStateAsString(Type));
}

FString APlayerCharacter::GetEStateAsString(EPotionType Type)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPotionType"), true);

	if (enumPtr == nullptr)
	{
		return FString("Invalid");
	}

	return enumPtr->GetNameStringByIndex((int32)Type);
}

