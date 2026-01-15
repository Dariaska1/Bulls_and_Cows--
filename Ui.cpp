#include "ui.h"
#include "game_modes.h"
#include "utils.h"
#include <iostream>
#include <limits>

using namespace std;

// Главное меню программы
void mainMenu() {
    int choice;
    bool exitProgram = false;

    while (!exitProgram) {
        clearScreen();

        cout << "=====================================================" << endl;
        cout << "|               ИГРА 'БЫКИ И КОРОВЫ'                |" << endl;
        cout << "=====================================================" << endl;
        cout << "|                                                   |" << endl;
        cout << "|               ВЫБЕРИТЕ РЕЖИМ ИГРЫ:                |" << endl;
        cout << "|                                                   |" << endl;
        cout << "|             1. Игра против компьютера             |" << endl;
        cout << "|             2. Игрок против игрока                |" << endl;
        cout << "|             3. Два игрока против друг друга       |" << endl;
        cout << "|             4. Правила игры                       |" << endl;
        cout << "|             5. Выход                              |" << endl;
        cout << "|                                                   |" << endl;
        cout << "=====================================================" << endl;

        cout << "\nВаш выбор (1-5): ";

        // Проверка корректности ввода
        if (!(cin >> choice)) {
            cout << "Ошибка: пожалуйста, введите число от 1 до 5." << endl;
            cin.clear();
            cin.ignore();
            cout << "Нажмите Enter, чтобы продолжить...";
            cin.get();
            continue;
        }

        // Очистка буфера ввода
        cin.ignore(), '\n';

        // Обработка выбора пользователя
        switch (choice) {
        case 1:
            clearScreen();
            playAgainstComputer();
            break;

        case 2:
            clearScreen();
            playPlayerVsPlayer();
            break;
        case 3:
            clearScreen();
            playTwoPlayersVersus();
            break;
        case 4:
            clearScreen();
            displayRules();
            break;
        case 5:
            cout << "\nСпасибо за игру! До свидания!" << endl;
            exitProgram = true;
            break;

        default:
            cout << "Неверный выбор. Пожалуйста, выберите от 1 до 5." << endl;
            cout << "Нажмите Enter, чтобы продолжить...";
            cin.get();
            break;
        }
    }
}

// Отображение правил игры
void displayRules() {
    clearScreen();

    cout << "=====================================================" << endl;
    cout << "|              ПРАВИЛА ИГРЫ 'БЫКИ И КОРОВЫ'         |" << endl;
    cout << "=====================================================" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  1. Компьютер или игрок загадывает число          |" << endl;
    cout << "|     из уникальных цифр (например, 1234)           |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  2. Другой игрок пытается угадать это число       |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  3. После каждой попытки даётся подсказка:        |" << endl;
    cout << "|     - БЫКИ: цифры, которые угаданы правильно      |" << endl;
    cout << "|       и находятся на своих местах                 |" << endl;
    cout << "|     - КОРОВЫ: цифры, которые есть в числе,        |" << endl;
    cout << "|       но находятся на других местах               |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  4. Цель: угадать число за минимальное            |" << endl;
    cout << "|     количество попыток                            |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  5. Все цифры в числе уникальны                   |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  6. В любой момент можно выйти,                   |" << endl;
    cout << "|     введя 'exit'                                  |" << endl;
    cout << "|                                                   |" << endl;
    cout << "|  Пример:                                          |" << endl;
    cout << "|  Загадано: 1234                                   |" << endl;
    cout << "|  Попытка:  1324                                   |" << endl;
    cout << "|  Подсказка: 2 быка, 2 коровы                      |" << endl;
    cout << "|  (быки: 1 и 4, коровы: 2 и 3)                     |" << endl;
    cout << "|                                                   |" << endl;
    cout << "=====================================================" << endl;
    cout << "\nНажмите Enter, чтобы вернуться в меню...";
    cin.get();
}