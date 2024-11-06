#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct BankDeposit {
    string name; // название
    double amount; // сумма вклада
    string currency;// тпи валюты
    double interestRate;// ставка в % годовых
};

// функция для сохранения массива вкладов в текстовый файл
void savefile(const vector<BankDeposit> &deposits, const string &filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
        return;
    }
    for (const auto &deposit : deposits) {
        outFile << deposit.name << '\n'
                << deposit.amount << '\n'
                << deposit.currency << '\n'
                << deposit.interestRate << '\n';
    }
    outFile.close();
}

// функция для сохранения массива вкладов в двоичный файл
void savebinarfile(const vector<BankDeposit> &deposits, const string &filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи: " << filename << endl;
        return;
    }
    for (const auto &deposit : deposits) {
        size_t nameSize = deposit.name.size();
        outFile.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        outFile.write(deposit.name.c_str(), nameSize);
        outFile.write(reinterpret_cast<const char*>(&deposit.amount), sizeof(deposit.amount));
        size_t currencySize = deposit.currency.size();
        outFile.write(reinterpret_cast<const char*>(&currencySize), sizeof(currencySize));
        outFile.write(deposit.currency.c_str(), currencySize);
        outFile.write(reinterpret_cast<const char*>(&deposit.interestRate), sizeof(deposit.interestRate));
    }
    outFile.close();
}

// функция для загрузки данных из текстового файла
vector<BankDeposit> loadFromTextFile(const string &filename) {
    vector<BankDeposit> deposits;
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Ошибка открытия файла для чтения: " << filename << endl;
        return deposits;
    }
    while (!inFile.eof()) {
        BankDeposit deposit;
        getline(inFile, deposit.name);
        if (deposit.name.empty()) break; // Проверка на пустую строку
        inFile >> deposit.amount;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшейся части строки
        getline(inFile, deposit.currency);
        inFile >> deposit.interestRate;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшейся части строки
        deposits.push_back(deposit);
    }
    inFile.close();
    return deposits;
}

// функция для загрузки данных из двоичного файла
vector<BankDeposit> loadFromBinaryFile(const string &filename) {
    vector<BankDeposit> deposits;
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Ошибка открытия файла для чтения: " << filename << endl;
        return deposits;
    }
    while (inFile.peek() != EOF) {
        BankDeposit deposit;
        size_t nameSize;
        inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        deposit.name.resize(nameSize);
        inFile.read(&deposit.name[0], nameSize);
        inFile.read(reinterpret_cast<char*>(&deposit.amount), sizeof(deposit.amount));
        size_t currencySize;
        inFile.read(reinterpret_cast<char*>(&currencySize), sizeof(currencySize));
        deposit.currency.resize(currencySize);
        inFile.read(&deposit.currency[0], currencySize);
        
        inFile.read(reinterpret_cast<char*>(&deposit.interestRate), sizeof(deposit.interestRate));
        deposits.push_back(deposit);
    }
    inFile.close();
    return deposits;
}

// функция для измерения размера файла
size_t getFileSize(const string &filename) {
    ifstream inFile(filename, ios::binary | ios::ate);
    return inFile.tellg();
}

// функция для вывода числа с двумя знаками после запятой
void printWithTwoDecimalPlaces(double value) {
    cout << (int)value;  // вывод целой части
    cout << '.';         // вывод запятой
    int decimalPart = (int)((value - (int)value) * 100);  // две последние цифры
    if (decimalPart < 0) decimalPart = -decimalPart; // если отриц число
    if (decimalPart < 10) cout << '0';   // добавление 0 при необходимости
    cout << decimalPart; // вывод дробной части
}

int main(int argc, char *argv[]) {
    int n;
    cout << "Введите количество вкладов: ";
    cin >> n;

    vector<BankDeposit> deposits(n);

    for (int i = 0; i < n; ++i) {
        cout << "Вклад " << i + 1 << ":\n";
        cout << "Название: ";
        cin.ignore(); 
        getline(cin, deposits[i].name);
        cout << "Сумма: ";
        cin >> deposits[i].amount;
        cout << "Тип валюты: ";
        cin.ignore(); 
        getline(cin, deposits[i].currency);
        cout << "Ставка в % годовых: ";
        cin >> deposits[i].interestRate;
    }

    savefile(deposits, "deposits.txt");
    savebinarfile(deposits, "deposits.bin");

    size_t textFileSize = getFileSize("deposits.txt");
    size_t binaryFileSize = getFileSize("deposits.bin");

    cout << "Размер текстового файла: " << textFileSize << " байт\n";
    cout << "Размер двоичного файла: " << binaryFileSize << " байт\n";

    vector<BankDeposit> textDeposits = loadFromTextFile("deposits.txt");
    vector<BankDeposit> binaryDeposits = loadFromBinaryFile("deposits.bin");

    cout << "\n Данные из текстового файла:\n";
    for (const auto &deposit : textDeposits) {
        cout << "Название: " << deposit.name << ", Сумма: ";
        printWithTwoDecimalPlaces(deposit.amount);
        cout << ", Валюта: " << deposit.currency << ", Ставка: ";
        printWithTwoDecimalPlaces(deposit.interestRate);
        cout << "%\n";
    }

    cout << "\n Данные из двоичного файла:\n";
    for (const auto &deposit : binaryDeposits) {
        cout << "Название: " << deposit.name << ", Сумма: ";
        printWithTwoDecimalPlaces(deposit.amount);
        cout << ", Валюта: " << deposit.currency << ", Ставка: ";
        printWithTwoDecimalPlaces(deposit.interestRate);
        cout << "%\n";
    }

    return 0;
}
