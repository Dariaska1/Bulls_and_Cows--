#pragma once
#include "utils.h"

// Основные игровые режимы
int playAgainstComputer();
int playPlayerVsPlayer();
int playTwoPlayersVersus();

// Вспомогательная функция для режима против компьютера и против игрока
int playGameSession(const GameSettings& settings, const std::string& secretNumber);

// Вспомогательные функции для режима двух игроков
std::string getPlayerSecret(int playerNumber, const GameSettings& settings);
bool processPlayerTurn(int playerNumber, const std::string& opponentSecret, std::vector<GameTurn>& playerLog, 
    const GameSettings& settings, int roundNumber);
void showGameResults(bool player1Won, bool player2Won,
    const std::vector<GameTurn>& logPlayer1, const std::vector<GameTurn>& logPlayer2,
    const std::string& secretPlayer1, const std::string& secretPlayer2, const GameSettings& settings);