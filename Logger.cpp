#include "logger.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// Сохранение лога игры в файл с запросом к пользователю
void saveGameLog(const vector<GameTurn>& gameLog, const string& secretNumber,
    const GameSettings& settings) {

    // Спрашиваем пользователя
    char saveChoice;
    cout << "\nХотите сохранить лог игры в файл? (y/n): ";
    cin >> saveChoice;

    if (!(saveChoice == 'y' || saveChoice == 'Y')) {
        return; // Пользователь не хочет сохранять
    }

    // Запрашиваем имя файла
    string filename;
    cout << "Введите имя файла (по умолчанию 'game_log.txt'): ";
    cin.ignore();
    getline(cin, filename);

    if (filename.empty()) {
        filename = "game_log.txt";
    }

    // Открываем файл для записи
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для записи лога: " << filename << endl;
        return;
    }

    // Записываем заголовок
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm localTime;
    localtime_s(&localTime, &now);

    file << "\n=== ЛОГ ИГРЫ 'БЫКИ И КОРОВЫ' ===" << endl;
    file << "Дата: " << put_time((const tm*)&localTime, "%Y-%m-%d %H:%M:%S") << endl;;
    file << "Загаданное число: " << secretNumber << endl;
    file << "Длина числа: " << settings.numberLength << endl;
    file << "Ноль в начале: " << (settings.ZeroFirst ? "разрешён" : "запрещён") << endl;

    if (settings.maxAttempts > 0) {
        file << "Максимальное количество попыток: " << settings.maxAttempts << endl;
    }
    else {
        file << "Ограничений по попыткам: нет" << endl;
    }

    file << "\nХоды игры:" << endl;

    // Записываем ходы
    for (const auto& turn : gameLog) {
        time_t turnTime = chrono::system_clock::to_time_t(turn.timestamp);
        tm turnLocalTime;
        localtime_s(&turnLocalTime, &turnTime);

        file << "  Попытка " << turn.attemptNumber << ": "
            << "Число: " << turn.guess << " - "
            << turn.bulls << " быков, " << turn.cows << " коров"
            << " (время: " << put_time((const tm*)&turnLocalTime, "%H:%M:%S") << ")" << endl;
    }

    file << "=== КОНЕЦ ЛОГА ===" << endl;
    file.close();

    cout << "Лог игры сохранён в файле: " << filename << endl;
}

// Сохранение лога в файл для режима двух игроков против друг друга
void offerSaveGameLog(const vector<GameTurn>& logPlayer1, const vector<GameTurn>& logPlayer2,
    const string& secretPlayer1, const string& secretPlayer2, const GameSettings& settings) {

    // Создаём объединённый лог
    vector<GameTurn> combinedLog;

    // Добавляем попытки обоих игроков
    for (const auto& turn : logPlayer1) {
        combinedLog.push_back(turn);
    }
    for (const auto& turn : logPlayer2) {
        combinedLog.push_back(turn);
    }

    // Сохраняем лог
    saveGameLog(combinedLog, "Игрок 1: " + secretPlayer1 + ", Игрок 2: " + secretPlayer2, settings);
}