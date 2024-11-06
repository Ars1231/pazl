#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

using namespace std;
//исходная структура
struct BankDeposit {
    string name; // название
    double amount; // сумма вклада
    string currency;// тпи валюты
    double interestRate;// ставка в % годовых
};

/*

это для понимания тебе пример из инета как работает outfile

Чтобы использовать этот класс, в текст программы необходимо включить заголовочный файл fstream.h. 1

Пример использования: 4

Включить необходимые заголовочные файлы: #include <iostream> и #include <fstream>. 4
Создать объект ofstream: ofstream outfile(“example.txt”). 4
Проверить, открыт ли файл: if (outfile.is_open()). 4
Написать в файл: outfile << “Hello, this is a test file.\n”; outfile << “This file is created using a C++ program.\n”;. 4
Закрыть файл: outfile.close(). 4
Пример кода: 4

#include <iostream>
#include <fstream>

int main()
{
    // Создаём объект ofstream для открытия файла для записи
    std::ofstream outfile(“example.txt”)
    // Проверяем, открыт ли файл
    if (outfile.is_open())
    {
        // Пишем текст в файл
        outfile << “Hello, this is a test file.\n”;
        outfile << “This file is created using a C++ program.\n”;
        // Закрываем файл
        outfile.close()
        // Уведомляем пользователя
        std::cout << “File created and text written successfully.\n”;
    } else {
        // Если файл нельзя открыть, выводим сообщение об ошибке
        std::cerr << “Error: Could not create the file.\n”;
    }
    return 0;
}*/



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
        if (deposit.name.empty()) break; // проверка на пустую строку
        inFile >> deposit.amount;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // игнорирование оставшейся части строки
        getline(inFile, deposit.currency);
        inFile >> deposit.interestRate;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); 
        deposits.push_back(deposit);
    }
    inFile.close();
    return deposits;
}




/*

для понимания inFile.peek() != EOF
istream::peek() returns the constant EOF (which is not guaranteed to be equal to -1) when it detects end-of-file or error. To check robustly for end-of-file, do this:
int c = file.peek();
if (c == EOF) {
  if (file.eof())
    // end of file
  else
    // error
} else {
  // do something with 'c'
}
You should know that the underlying OS primitive, read(2), only signals EOF when you try to read past the end of the file. 
Therefore, file.eof() will not be true when you have merely read up to the last character in the file. In other words, file.eof() being false does not mean the next read operation will succeed.
*/


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

int main(int argc, char *argv[]) {
    int n;
    cout << "Введите количество вкладов: ";
    cin >> n;
    vector<BankDeposit> deposits(n);
    for (int i = 0; i < n; ++i) {
        cout << "Вклад " << i + 1 << ":" << "\n";
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
    //сохранение в файлы
    savefile(deposits, "deposits.txt");
    savebinarfile(deposits, "deposits.bin");
    //размеры файлов
    size_t textFileSize = getFileSize("deposits.txt");
    size_t binaryFileSize = getFileSize("deposits.bin");
    cout << "Размер текстового файла: " << textFileSize << " байт\n";
    cout << "Размер двоичного файла: " << binaryFileSize << " байт\n";
    //загрузка текста из файла
    vector<BankDeposit> textDeposits = loadFromTextFile("deposits.txt");
    vector<BankDeposit> binaryDeposits = loadFromBinaryFile("deposits.bin");
    
    cout << "Данные из текстового файла:\n";
    for (const auto &deposit : textDeposits) {
        cout << "Название: " << deposit.name;
        cout << "Сумма: " << deposit.amount;
        cout << ", Валюта: " << deposit.currency;
        cout << "Проценты:" << deposit.interestRate<< "\n";
    }

    cout << "Данные из двоичного файла:\n";
    for (const auto &deposit : binaryDeposits) {
        cout << "Название: " << deposit.name;
        cout << "Сумма: " << deposit.amount;
        cout << ", Валюта: " << deposit.currency;
        cout << "Проценты: " << deposit.interestRate << "\n";
    }

    return 0;
}
