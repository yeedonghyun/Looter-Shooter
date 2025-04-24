#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EscapePoint.generated.h"

UCLASS()
class LOOTERSHOOTER_API AEscapePoint : public AActor
{
	GENERATED_BODY()

public:
	AEscapePoint();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Escape")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Escape")
	float EscapeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* CollisionBox;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetActive(bool bActive) { bIsActive = bActive; }

	UFUNCTION(BlueprintCallable)
	bool GetActive() const { return bIsActive; }

	UFUNCTION(BlueprintCallable)
	void SetEscapeTime(float Time) { EscapeTime = Time; }

	UFUNCTION(BlueprintCallable)
	float GetEscapeTime() const { return EscapeTime; }

	UFUNCTION(BlueprintCallable)
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};