#include "game_modes.h"
#include "logger.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

// Режим игры против компьютера
int playAgainstComputer() {
    cout << "\n=== РЕЖИМ 'ИГРОК ПРОТИВ КОМПЬЮТЕРА' ===" << endl;

    // 1. Получаем настройки игры
    GameSettings settings = getGameSettings();

    // 2. Генерируем секретное число
    string secretNumber;
    try {
        secretNumber = generateSecretNumber(settings.numberLength, settings.ZeroFirst);
    }
    catch (const exception& e) {
        cerr << "Ошибка при генерации числа: " << e.what() << endl;
        return -1;
    }

    // 3. Основной цикл игры
    return playGameSession(settings, secretNumber);
}

// Режим игры против игрока
int playPlayerVsPlayer() {
    cout << "\n=== РЕЖИМ 'ИГРОК ПРОТИВ ИГРОКА' ===" << endl;

    // 1. Получаем настройки игры
    GameSettings settings = getGameSettings();

    // 2. Игрок 1 загадывает число
    string secretNumber;
    bool validSecret = false;

    while (!validSecret) {
        string prompt = "Игрок 1, загадайте число из " +
            to_string(settings.numberLength) + " цифр: ";

        // Используем скрытый ввод
        secretNumber = secretInput(prompt);

        //Проверка на выход
        if (secretNumber == "exit" || secretNumber == "выход") {
            cout << "Игра прервана." << endl;
            return -1;
        }

        // Валидация загаданного числа
        string errorMessage;
        if (ValidNumber(secretNumber, settings.numberLength,
            settings.ZeroFirst, &errorMessage)) {
            validSecret = true;
            cout << "Число принято! Игрок 2, теперь игра за вами :)\n" << endl;
        }
        else {
            cout << "Ошибка: " << errorMessage << endl;
            cout << "Пожалуйста, введите корректное число.\n" << endl;
        }
    }

    // 3. Основной цикл игры
    return playGameSession(settings, secretNumber);
}

// Основной игровой цикл (общий для двух режимов)
int playGameSession(const GameSettings& settings, const string& secretNumber) {
    cout << "\n=== ИГРА НАЧИНАЕТСЯ ===" << endl;
    cout << "Угадайте " << settings.numberLength << " - значное число." << endl;
    cout << "Для выхода введите 'exit' в любой момент.\n" << endl;

    vector<GameTurn> gameLog;
    int attemptCount = 0;
    bool gameWon = false;
    bool gameOver = false;

    while (!gameOver && !gameWon) {
        attemptCount++;

        if (settings.maxAttempts > 0 && attemptCount > settings.maxAttempts) {
            cout << "\n===========================================" << endl;
            cout << "ПОРАЖЕНИЕ! Вы использовали все " << settings.maxAttempts << " попыток(ки)" << endl;
            cout << "Загаданное число было: " << secretNumber << endl;
            cout << "===========================================\n" << endl;
            gameOver = true;
            break;
        }

        // Запрос ввода
        string userInput;
        cout << "Попытка #" << attemptCount;
        if (settings.maxAttempts > 0) {
            cout << " (осталось " << (settings.maxAttempts - attemptCount + 1) << ")";
        }
        cout << ": ";

        cin >> userInput;

        // Проверка на выход
        if (userInput == "exit" || userInput == "выход") {
            cout << "\nИгра прервана." << endl;
            cout << "Загаданное число было: " << secretNumber << endl;
            gameOver = true;
            break;
        }

        // Валидация ввода
        string errorMessage;
        if (!ValidNumber(userInput, settings.numberLength,
            settings.ZeroFirst, &errorMessage)) {
            cout << "Ошибка: " << errorMessage << endl;
            cout << "Попробуйте снова." << endl;
            attemptCount--;
            continue;
        }

        // Проверка на повторный ввод того же числа
        bool alreadyGuessed = false;
        for (const auto& turn : gameLog) {
            if (turn.guess == userInput) {
                cout << "Вы уже вводили это число на попытке #" << turn.attemptNumber << "." << endl;
                cout << "Попробуйте другое число." << endl;
                alreadyGuessed = true;
                break;
            }
        }

        if (alreadyGuessed) {
            attemptCount--;
            continue;
        }

        // Подсчет быков и коров
        auto result = countBullsAndCows(secretNumber, userInput);
        int bulls = result.first;
        int cows = result.second;

        // Сохранение хода в лог
        gameLog.emplace_back(attemptCount, userInput, bulls, cows);

        cout << "Подсказка: " << bulls << " бык(ов), " << cows << " коров(а)" << endl;

        // Проверка победы
        if (bulls == settings.numberLength) {
            gameWon = true;
            cout << "\n===========================================" << endl;
            cout << "ПОБЕДА! Игрок угадал число: " << secretNumber << endl;
            cout << "Количество попыток: " << attemptCount << endl;
            cout << "===========================================\n" << endl;
            break;
        }
    }

    // 5. Завершение игры
    if (gameWon || (!gameWon && settings.maxAttempts > 0 && attemptCount > settings.maxAttempts)) {
        // Вывод статистики
        printGameStatistics(settings, secretNumber, attemptCount, gameWon);

        // Предложение сохранить лог
        saveGameLog(gameLog, secretNumber, settings);

        return gameWon ? attemptCount : -1;
    }

    return -1; // Игра была прервана
}

// Режим игры два игрока против друг друга
int playTwoPlayersVersus() {
    cout << "\n=== РЕЖИМ 'ДВА ИГРОКА ПРОТИВ ДРУГ ДРУГА' ===" << endl;
    cout << "Оба игрока загадывают числа, затем по очереди пытаются угадать число противника." << endl;
    cout << "Побеждает тот, кто первым угадает число противника.\n" << endl;

    // 1. Получаем настройки игры
    GameSettings settings = getGameSettings();

    // 2. Игрок 1 загадывает число
    string secretPlayer1 = getPlayerSecret(1, settings);
    if (secretPlayer1.empty()) return 0; // Игрок вышел

    cout << "Число принято! Игрок 2, теперь ваша очередь загадывать." << endl;
 
    // 3. Игрок 2 загадывает число
    string secretPlayer2 = getPlayerSecret(2, settings);
    if (secretPlayer2.empty()) return 0; // Игрок вышел

    cout << "Оба числа загаданы! Игра начинается.\n" << endl;
    cout << "Нажмите Enter, чтобы начать...";
    cin.ignore();
    cin.get();

    // 4. Инициализация игровых переменных
    vector<GameTurn> logPlayer1;
    vector<GameTurn> logPlayer2;

    int roundNumber = 0;
    bool player1Won = false;
    bool player2Won = false;
    bool gameOver = false;

    // 5. Основной игровой цикл
    while (!gameOver) {
        roundNumber++;

        // 5.1 Ход игрока 1
        if (!player1Won) {
            player1Won = processPlayerTurn(1, secretPlayer2, logPlayer1, settings, roundNumber);
            if (player1Won) {
                gameOver = true;
                continue;
            }
        }

        // 5.2 Ход игрока 2
        if (!player2Won) {
            player2Won = processPlayerTurn(2, secretPlayer1, logPlayer2, settings, roundNumber);
            if (player2Won) {
                gameOver = true;
                continue;
            }
        }

        // Проверка на ограничение попыток
        if (settings.maxAttempts > 0) {
            if (logPlayer1.size() >= settings.maxAttempts && logPlayer2.size() >= settings.maxAttempts) {
                cout << "\nДостигнуто максимальное количество попыток!" << endl;
                gameOver = true;
            }
        }

        // Проверка завершения игры
        if (player1Won || player2Won) {
            gameOver = true;
        }
    }

    // 6. Завершение игры и вывод результатов
    showGameResults(player1Won, player2Won, logPlayer1, logPlayer2, secretPlayer1, secretPlayer2, settings);

    // 7. Предложение сохранить лог
    offerSaveGameLog(logPlayer1, logPlayer2, secretPlayer1, secretPlayer2, settings);

    // 8. Возвращаем результат
    if (player1Won && !player2Won) return 1;
    if (player2Won && !player1Won) return 2;
    return 0;
}

// Получаем загаданные числа от пользователей
string getPlayerSecret(int playerNumber, const GameSettings& settings) {
    cout << "\n--- ИГРОК " << playerNumber << " ЗАГАДЫВАЕТ ЧИСЛО ---" << endl;

    string secret;
    bool validSecret = false;

    while (!validSecret) {
        string prompt = "Игрок " + to_string(playerNumber) +
            ", введите загаданное число (" +
            to_string(settings.numberLength) + " цифр): ";

        secret = secretInput(prompt);

        // Проверка на выход
        if (secret == "exit" || secret == "выход") {
            cout << "Игра прервана." << endl;
            return "";
        }

        // Валидация
        string errorMessage;
        if (ValidNumber(secret, settings.numberLength,
            settings.ZeroFirst, &errorMessage)) {
            validSecret = true;
        }
        else {
            cout << "Ошибка: " << errorMessage << endl;
            cout << "Пожалуйста, введите корректное число." << endl;
        }
    }

    return secret;
}

// Обработка хода игрока
bool processPlayerTurn(int playerNumber, const string& opponentSecret,
    vector<GameTurn>& playerLog, const GameSettings& settings, int roundNumber) {

    clearScreen();
    cout << "=== РАУНД " << roundNumber << " ===" << endl;
    cout << "\n--- ХОД ИГРОКА " << playerNumber << " ---" << endl;

    cout << "Попробуйте угадать число Игрока " << (playerNumber == 1 ? 2 : 1) << "." << endl;

    // Показываем оставшиеся попытки
    if (settings.maxAttempts > 0) {
        cout << "У вас осталось " << (settings.maxAttempts - playerLog.size()) << " попыток" << endl;
    }
    else {
        cout << "Ограничений по попыткам нет" << endl;
    }

    // Показываем историю попыток
    if (!playerLog.empty()) {
        cout << "\nВаши предыдущие попытки:" << endl;
        for (const auto& turn : playerLog) {
            cout << "  Попытка " << turn.attemptNumber << ": "
                << turn.guess << " - "
                << turn.bulls << " бык(ов), " << turn.cows << " коров(а)" << endl;
        }
    }

    // Запрос ввода
    string guess;
    cout << "\nВаше предположение: ";
    cin >> guess;

    // Проверка на выход
    if (guess == "exit" || guess == "выход") {
        cout << "Игра прервана." << endl;
        exit(0);
    }

    // Валидация ввода
    string errorMessage;
    if (!ValidNumber(guess, settings.numberLength,
        settings.ZeroFirst, &errorMessage)) {
        cout << "Ошибка: " << errorMessage << endl;
        cout << "Нажмите Enter, чтобы продолжить...";
        cin.ignore();
        cin.get();
        return false;
    }

    // Подсчёт быков и коров
    std::pair <int, int> result = countBullsAndCows(opponentSecret, guess);
    int bulls = result.first;
    int cows = result.second;

    // Сохранение хода
    playerLog.emplace_back(playerLog.size() + 1, guess, bulls, cows);

    // Вывод результата
    cout << "\nПодсказка: " << bulls << " бык(ов), " << cows << " коров(а)" << endl;

    // Проверка победы
    if (bulls == settings.numberLength) {
        cout << "\nПОЗДРАВЛЯЕМ! Игрок " << playerNumber << " угадал число!" << endl;
        cout << "Нажмите Enter, чтобы продолжить...";
        cin.ignore();
        cin.get();
        return true;
    }

    cout << "\nНажмите Enter, чтобы продолжить...";
    cin.ignore();
    cin.get();
    return false;
}

// Вывод результатов игры
void showGameResults(bool player1Won, bool player2Won, const vector<GameTurn>& logPlayer1, const vector<GameTurn>& logPlayer2,
    const string& secretPlayer1, const string& secretPlayer2, const GameSettings& settings) {
    clearScreen();
    cout << "=============================================" << endl;
    cout << "              ИГРА ЗАВЕРШЕНА" << endl;
    cout << "=============================================\n" << endl;

    if (player1Won && player2Won) {
        cout << "НИЧЬЯ! Оба игрока угадали числа друг друга!" << endl;
        cout << "Игрок 1 угадал за " << logPlayer1.size() << " попыток" << endl;
        cout << "Игрок 2 угадал за " << logPlayer2.size() << " попыток" << endl;
    }
    else if (player1Won) {
        cout << "ПОБЕДИТЕЛЬ: ИГРОК 1!" << endl;
        cout << "Он угадал число Игрока 2 за " << logPlayer1.size() << " попыток" << endl;
        if (!player2Won) {
            cout << "Игрок 2 не успел угадать число (сделал " << logPlayer2.size() << " попыток)" << endl;
        }
    }
    else if (player2Won) {
        cout << "ПОБЕДИТЕЛЬ: ИГРОК 2!" << endl;
        cout << "Он угадал число Игрока 1 за " << logPlayer2.size() << " попыток" << endl;
        if (!player1Won) {
            cout << "Игрок 1 не успел угадать число (сделал " << logPlayer1.size() << " попыток)" << endl;
        }
    }
    else {
        cout << "НИЧЬЯ! Ни один из игроков не угадал число противника." << endl;
        cout << "Игрок 1 сделал " << logPlayer1.size() << " попыток" << endl;
        cout << "Игрок 2 сделал " << logPlayer2.size() << " попыток" << endl;
    }

    cout << "\nЗагаданные числа:" << endl;
    cout << "Игрок 1: " << secretPlayer1 << endl;
    cout << "Игрок 2: " << secretPlayer2 << endl;
}