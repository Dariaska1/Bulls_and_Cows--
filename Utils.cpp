#include "utils.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <limits>
#include <chrono>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Функция для получения настроек игры от пользователя
GameSettings getGameSettings() {
    GameSettings settings{};

    cout << "\n=== НАСТРОЙКИ ИГРЫ ===" << endl;

    // 1. Запрос длины числа
    while (true) {
        cout << "Длина загадываемого числа (1-10, по умолчанию 4): ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            settings.numberLength = 4;
            break;
        }

        //Проверка соответствия диапазону
        try {
            int value = stoi(input);
            if (value >= 1 && value <= 10) {
                settings.numberLength = value;
                break;
            }
            else {
                cout << "Ошибка: длина должна быть от 1 до 10." << endl;
            }
        }
        catch (const exception&) {
            cout << "Ошибка: введите число от 1 до 10." << endl;
        }
    }

    // 2. Запрос о нуле в начале
    while (true) {
        cout << "Разрешить число, начинающееся с нуля? (y/n, по умолчанию n): ";
        string input;
        getline(cin, input);

        if (input.empty() || input == "n" || input == "N") {
            settings.ZeroFirst = false;
            break;
        }
        else if (input == "y" || input == "Y") {
            settings.ZeroFirst = true;
            break;
        }
        else {
            cout << "Ошибка: введите 'y' или 'n'." << endl;
        }
    }

    // 3. Запрос максимального количества попыток
    while (true) {
        cout << "Максимальное количество попыток (1-1000: 0 = без ограничений, по умолчанию 0): ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            settings.maxAttempts = 0;
            break;
        }

        //Проверка соответсвия диапазону
        try {
            int value = stoi(input);

            if (value == 0) {
                settings.maxAttempts = 0;
                break;
            }
            else if (value >= 1 && value <= 1000) {
                settings.maxAttempts = value;
                break;
            }
            else if (value < 0) {
                cout << "Ошибка: количество попыток не может быть отрицательным." << endl;
            }
            else {
                cout << "Ошибка: количество попыток должно быть от 1 до 1000." << endl;
                cout << "Совет: для игры без ограничений введите 0." << endl;
            }
        }
        catch (const exception&) {
            cout << "Ошибка: введите число от 0 до 1000." << endl;
        }
    }

    // Проверка комбинации: длина 10 и запрет нуля в начале
    if (!settings.ZeroFirst && settings.numberLength == 10) {
        cout << "\nВнимание: при длине 10 и запрете нуля в начале невозможно сгенерировать число." << endl;
        cout << "Автоматически разрешаем ноль в начале." << endl;
        settings.ZeroFirst = true;
    }

    // Проверка логичности настроек
    if (settings.maxAttempts > 0 && settings.maxAttempts < settings.numberLength) {
        cout << "\nВнимание: максимальное количество попыток (" << settings.maxAttempts
             << ") меньше длины числа (" << settings.numberLength << ")." << endl;
        cout << "Это может сделать игру очень сложной." << endl;

        //предлагаем изменить количество попыток
        while (true) {
            cout << "Хотите изменить количество попыток? (y/n): ";
            string choice;
            getline(cin, choice);

            if (choice == "y" || choice == "Y") {
                // Пользователь согласен изменить количество попыток
                while (true) {
                    cout << "Введите новое количество попыток (0 = без ограничений, 1-999): ";
                    string newInput;
                    getline(cin, newInput);

                    try {
                        int newValue = stoi(newInput);

                        if (newValue == 0) {
                            settings.maxAttempts = 0;
                            cout << "Установлено: без ограничения попыток." << endl;
                            break;
                        }
                        else if (newValue >= 1 && newValue <= 999) {
                            // Если новое значение всё ещё меньше длины числа, спрашиваем подтверждение
                            if (newValue < settings.numberLength) {
                                cout << "Новое количество попыток (" << newValue
                                    << ") всё ещё меньше длины числа ("
                                    << settings.numberLength << ")." << endl;

                                cout << "Всё равно использовать это значение? (y/n): ";
                                string confirm;
                                getline(cin, confirm);

                                if (confirm == "y" || confirm == "Y") {
                                    settings.maxAttempts = newValue;
                                    cout << "Количество попыток изменено на " << newValue << "." << endl;
                                    break;
                                }
                                // Если не подтвердили, продолжаем запрашивать новое значение
                            }
                            else {
                                settings.maxAttempts = newValue;
                                cout << "Количество попыток изменено на " << newValue << "." << endl;
                                break;
                            }
                        }
                        else if (newValue < 0) {
                            cout << "Ошибка: количество попыток не может быть отрицательным." << endl;
                        }
                        else {
                            cout << "Ошибка: количество попыток должно быть от 1 до 999." << endl;
                        }
                    }
                    catch (const exception&) {
                        cout << "Ошибка: введите число от 0 до 999." << endl;
                    }
                }
                break; // Выходим из цикла подтверждения изменения

            }
            else if (choice == "n" || choice == "N") {
                // Пользователь не хочет менять количество попыток
                cout << "Оставляем текущее количество попыток: " << settings.maxAttempts << endl;
                break;
            }
            else {
                cout << "Ошибка: введите 'y' или 'n'." << endl;
            }
        }
    }

    // Вывод итоговых настроек
    cout << "\n=== НАСТРОЙКИ СОХРАНЕНЫ ===" << endl;
    cout << "Длина числа: " << settings.numberLength << endl;
    cout << "Ноль в начале: " << (settings.ZeroFirst ? "разрешён" : "запрещён") << endl;

    if (settings.maxAttempts > 0) {
        cout << "Максимальное количество попыток: " << settings.maxAttempts << endl;
    }
    else {
        cout << "Ограничения по попыткам: нет" << endl;
    }

    cout << "============================\n" << endl;

    return settings;
}

// Функция подсчёта быков и коров
pair<int, int> countBullsAndCows(const string& secret, const string& guess) {
   
    int bulls = 0, cows = 0;
    int secretCount[10] = { 0 };
    int guessCount[10] = { 0 };

    //Ищем быков и повторяющиеся цифры
    for (size_t i = 0; i < secret.length(); ++i) {
        if (secret[i] == guess[i]) {
            bulls++;
        }
        else {
            secretCount[secret[i] - '0']++;
            guessCount[guess[i] - '0']++;
        }
    }

    //Вычисляем коров
    for (int i = 0; i < 10; ++i) {
        cows += min(secretCount[i], guessCount[i]);
    }

    return { bulls, cows };
}

// Функция валидации числа
bool ValidNumber(const string& number, int expectedLength = -1, bool ZeroFirst = false, string* errorMessage = nullptr) {

    // Вспомогательная функция для установки сообщения об ошибке
    auto setError = [&](const string& msg) {
        if (errorMessage != nullptr) {
            *errorMessage = msg;
        }
        return false;
    };

    // 1. Проверка на пустую строку
    if (number.empty()) {
        return setError("Число не может быть пустым");
    }

    // 2. Проверка максимальной длины (не больше 10 из-за уникальности цифр)
    if (number.length() > 10) {
        return setError("Число не может быть длиннее 10 цифр (все цифры должны быть уникальными)");
    }

    // 3. Проверка, что все символы - цифры
    for (char c : number) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            ostringstream oss;
            oss << "Недопустимый символ '" << c << "'. Число должно содержать только цифры 0-9";
            return setError(oss.str());
        }
    }

    // 4. Проверка на уникальность цифр
    vector<bool> digitSeen(10, false);
    for (char c : number) {
        int digit = c - '0';

        if (digitSeen[digit]) {
            ostringstream oss;
            oss << "Цифра " << digit << " повторяется. Все цифры должны быть уникальными";
            return setError(oss.str());
        }

        digitSeen[digit] = true;
    }

    // 5. Проверка на ведущий ноль (если не разрешено)
    if (!ZeroFirst && number[0] == '0') {
        return setError("Число не может начинаться с нуля");
    }

    // 6. Проверка длины (если expectedLength задан)
    if (expectedLength != -1) {
        if (static_cast<int>(number.length()) != expectedLength) {
            ostringstream oss;
            oss << "Некорректная длина. Ожидается " << expectedLength
                << " цифр, получено " << number.length();
            return setError(oss.str());
        }
    }

    // Если все проверки пройдены
    if (errorMessage != nullptr) {
        *errorMessage = "Число корректно";
    }
    return true;
}

// Функция генерации случайного числа
string generateSecretNumber(int length, bool ZeroFirst = false) {

    const int MAX_ATTEMPTS = 1000;
    int attempts = 0;

    // Основной цикл генерации с проверкой
    while (attempts < MAX_ATTEMPTS) {
        vector<char> digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

        unsigned seed = static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count() + attempts);
        mt19937 generator(seed);

        // Перемешиваем цифры с помощью алгоритма Fisher-Yates
        shuffle(digits.begin(), digits.end(), generator);

        string result;
        for (int i = 0; i < length; ++i) {
            result += digits[i];
        }

        // Проверяем сгенерированное число на валидность
        if (ValidNumber(result, length, ZeroFirst, nullptr)) {
            return result;
        }

        attempts++;
    }

    throw runtime_error("Не удалось сгенерировать корректное число. Проверьте логику валидации.");
}

// Функция для скрытого ввода (символы не отображаются на экране)
string secretInput(const string& prompt) {
    string input;

    cout << prompt;

    // Настраиваем консоль для скрытия ввода
#ifdef _WIN32
    // Для Windows: получаем дескриптор стандартного ввода
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;  // Переменная для хранения текущих настроек консоли

    // Сохраняем текущий режим консоли
    GetConsoleMode(hStdin, &mode);

    // Отключаем отображение вводимых символов (бит ECHO)
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    // Считываем ввод (символы не отображаются)
    cin >> input;

    // Восстанавливаем исходный режим консоли
    SetConsoleMode(hStdin, mode);
#else
// Для Linux/Mac: используем system("stty -echo") и system("stty echo")
    system("stty -echo"); // Отключаем эхо

    cin >> secretNumber; // Вводим, символы не отображаются

    system("stty echo"); // Восстанавливаем эхо
#endif
    cout << endl;

    return input;
}

// Функция вывода статистики
void printGameStatistics(const GameSettings& settings, const string& secretNumber, int attemptCount, bool gameWon) {
    cout << "=== СТАТИСТИКА ИГРЫ ===" << endl;
    cout << "Загаданное число: " << secretNumber << endl;
    cout << "Ваших попыток: " << attemptCount << endl;

    if (settings.maxAttempts > 0) {
        cout << "Лимит попыток: " << settings.maxAttempts << endl;
        if (gameWon) {
            int usedPercentage = (attemptCount * 100) / settings.maxAttempts;
            cout << "Использовано: " << attemptCount << " из " << settings.maxAttempts
                << " (" << usedPercentage << "%)" << endl;
        }
    }
    cout << "========================" << endl;
}

// Очищает экран консоли (кроссплатформенная функция)
void clearScreen() {

 //для Windows
#ifdef _WIN32
    system("cls");      // Вызов системной команды cls (clear screen)

 //для Unix-подобных систем
#else
    system("clear");    // Вызов системной команды clear
#endif
}