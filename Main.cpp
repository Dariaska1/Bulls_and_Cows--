#include "ui.h"
#include "Utils.h"
using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Заголовок программы
    clearScreen();
    cout << "=====================================================" << endl;
    cout << "|              ДОБРО ПОЖАЛОВАТЬ В ИГРУ              |" << endl;
    cout << "|                 'БЫКИ И КОРОВЫ'!                  |" << endl;
    cout << "=====================================================" << endl;
    cout << "Приятной игры :)" << endl;
    cout << "Нажмите Enter, чтобы продолжить...";
    cin.get();

    // Запуск главного меню
    mainMenu();

    return 0;
}