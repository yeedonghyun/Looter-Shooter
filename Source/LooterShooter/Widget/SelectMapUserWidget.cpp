#include "SelectMapUserWidget.h"
#include "Kismet/GameplayStatics.h"

USelectMapUserWidget::USelectMapUserWidget(const FObjectInitializer& Object) : Super(Object)
{
    MapIndex = 0;
    color = FLinearColor::White;
}

void USelectMapUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Play)
    {
        Play->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonClicked);
        Play->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonHovered);
        Play->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnPlayButtonUnhovered);
    }

    if (AsianTown)
    {
        AsianTown->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnAsianTownButtonClicked);
        AsianTown->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnAsianTownButtonHovered);
        AsianTown->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnAsianTownButtonUnhovered);
    }

    if (MilitaryAirport)
    {
        MilitaryAirport->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnMilitaryAirportButtonClicked);
        MilitaryAirport->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnMilitaryAirportButtonHovered);
        MilitaryAirport->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnMilitaryAirportButtonUnhovered);
    }

    if (Back)
    {
        Back->OnClicked.AddDynamic(this, &USelectMapUserWidget::OnBackButtonClicked);
        Back->OnHovered.AddDynamic(this, &USelectMapUserWidget::OnExitButtonHovered);
        Back->OnUnhovered.AddDynamic(this, &USelectMapUserWidget::OnExitButtonUnhovered);
    }
}

void USelectMapUserWidget::OnPlayButtonClicked()
{
    if (MapIndex == 1) {
        UGameplayStatics::OpenLevel(this, FName("L_Showcase_map"));
    }
    else if (MapIndex == 2) {
        UGameplayStatics::OpenLevel(this, FName("Map_Airbase_Demo"));
    }
}

void USelectMapUserWidget::OnAsianTownButtonClicked()
{
    if (AsianTown) {
        color = AsianTown->GetBackgroundColor();
        color.A = 1;
        AsianTown->SetBackgroundColor(color);
    }
    if (MilitaryAirport) {
        color = MilitaryAirport->GetBackgroundColor();
        color.A = 0;
        MilitaryAirport->SetBackgroundColor(color);
    }
    MapIndex = 1;
}

void USelectMapUserWidget::OnMilitaryAirportButtonClicked()
{
    if (AsianTown) {
        color = AsianTown->GetBackgroundColor();
        color.A = 0;
        AsianTown->SetBackgroundColor(color);
    }
    if (MilitaryAirport) {
        color = AsianTown->GetBackgroundColor();
        color.A = 1;
        MilitaryAirport->SetBackgroundColor(color);
    }
    MapIndex = 2;
}

void USelectMapUserWidget::OnBackButtonClicked()
{
    this->RemoveFromParent();

    if (TSubclassOf<UUserWidget> StorageWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/Widget/BP_MainMenuUserWidget.BP_MainMenuUserWidget_C'")))
    {
        UUserWidget* StorageWidget = CreateWidget<UUserWidget>(GetWorld(), StorageWidgetClass);
        if (StorageWidget)
        {
            StorageWidget->AddToViewport();
        }
    }
}

void USelectMapUserWidget::OnPlayButtonHovered()
{
    if (Play) {
        color = Play->GetBackgroundColor();
        color.A = 1;
        Play->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnPlayButtonUnhovered()
{
    if (Play) {
        color = Play->GetBackgroundColor();
        color.A = 0;
        Play->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnAsianTownButtonHovered()
{
    if (AsianTown) {
        color = AsianTown->GetBackgroundColor();
        color.A = 1;
        AsianTown->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnAsianTownButtonUnhovered()
{
    if (AsianTown && MapIndex != 1) {
        color = AsianTown->GetBackgroundColor();
        color.A = 0;
        AsianTown->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnMilitaryAirportButtonHovered()
{
    if (MilitaryAirport) {
        color = MilitaryAirport->GetBackgroundColor();
        color.A = 1;
        MilitaryAirport->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnMilitaryAirportButtonUnhovered()
{
    if (MilitaryAirport && MapIndex != 2) {
        color = MilitaryAirport->GetBackgroundColor();
        color.A = 0;
        MilitaryAirport->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnExitButtonHovered()
{
    if (Back) {
        color = Back->GetBackgroundColor();
        color.A = 1;
        Back->SetBackgroundColor(color);
    }
}

void USelectMapUserWidget::OnExitButtonUnhovered()
{
    if (Back) {
        color = Back->GetBackgroundColor();
        color.A = 0;
        Back->SetBackgroundColor(color);
    }
}
