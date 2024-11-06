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
