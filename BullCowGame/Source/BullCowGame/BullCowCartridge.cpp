// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    PrintLine(PlayerInput);
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //Checking player guess
    {
        ProcessGuess(PlayerInput);
    }
    
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

        //Welcoming the player
    PrintLine(TEXT("Welcome to the Bull Cow Game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and press Enter."));
    //PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); //Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));

}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) // Checks if the correct word is guessed
    {
        PrintLine(TEXT("Correct!"));
        EndGame();
        return;
    }
    if (Guess.Len() != HiddenWord.Len()) // Check for the right number of characters
    {
        PrintLine(TEXT("The hidden word is %i characters long."), HiddenWord.Len());
        return;
    }
    if (!IsIsogram(Guess)) // Isogram check
    {
        PrintLine(TEXT("This word is not an isogram."));
        return;
    }

    --Lives;  // Decrement a life
    
    if (Lives <= 0) // If you have no lives left quit the game
    {
        ClearScreen();
        PrintLine(TEXT("You have lost!"));
        PrintLine(TEXT("The hidden word was: %s."), *HiddenWord);
        EndGame();
        return;
    }
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);

    PrintLine(TEXT("You have %i lives remanining guess again."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)     
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    
    for (FString Word : WordList)
    {
        if (Word.Len() >=4 && Word.Len() <=8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])        
        {
            Count.Bulls++;
            continue;

        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}