#pragma once
#include <string>
#include <utility>
#include <vector>
#include <chrono>

// Структуры данных
struct GameTurn {
    int attemptNumber;
    std::string guess;
    int bulls;
    int cows;
    std::chrono::system_clock::time_point timestamp;

    GameTurn(int attempt, const std::string& g, int b, int c)
        : attemptNumber(attempt), guess(g), bulls(b), cows(c), timestamp(std::chrono::system_clock::now()) {}
};

struct GameSettings {
    int numberLength;   
    bool ZeroFirst;
    int maxAttempts;
};

// Настройки игры
GameSettings getGameSettings();

// Основные игровые функции
std::pair<int, int> countBullsAndCows(const std::string& secret, const std::string& guess);
bool ValidNumber(const std::string& number, int expectedLength, bool ZeroFirst, std::string* errorMessage);
std::string generateSecretNumber(int length, bool ZeroFirst);

// Функции ввода-вывода
std::string secretInput(const std::string& prompt);
void clearScreen();
void printGameStatistics(const GameSettings& settings, const std::string& secretNumber, int attemptCount, bool gameWon);