#pragma once
#include "utils.h"

//Функции сохранения логов
void saveGameLog(const std::vector<GameTurn>& gameLog, const std::string& secretNumber,
    const GameSettings& settings);

void offerSaveGameLog(const std::vector<GameTurn>& logPlayer1, const std::vector<GameTurn>& logPlayer2,
    const std::string& secretPlayer1, const std::string& secretPlayer2, const GameSettings& settings);