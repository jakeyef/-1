#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Перечисление Group
enum Group { WORK, FAMILY, FRIENDS, OTHERS };

// Структура Date
struct Date {
    int day, month, year;
};

// Структура Phone
struct Phone {
    int countryCode;
    int cityCode;
    long long number;
};

// Структура Contact
struct Contact {
    string lastName;    // 
    string firstName;   // 
    string middleName;  // 
    Phone phone;        // 
    Date birthday;      // 
    string email;       // 
    Group category;     // 
};

// --- Вспомогательные функции и Валидация ---

// Безопасный ввод целых чисел
int getSafeInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка! Введите число: ";
    }
    return value;
}

// Валидация Email
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0) return false;
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1) return false;
    return true;
}

// Валидация телефона
bool isValidPhone(int countryCode) {
    return (countryCode >= 1 && countryCode <= 999);
}

// Форматирование имени (обрезка)
string formatName(string name) {
    if (name.length() > 15) {
        return name.substr(0, 15) + "...";
    }
    return name;
}

string groupToString(Group g) {
    switch (g) {
    case WORK: return "WORK";
    case FAMILY: return "FAMILY";
    case FRIENDS: return "FRIENDS";
    default: return "OTHERS";
    }
}

// --- Основной функционал ---

// Добавление контакта
void addContact(vector<Contact>& db) {
    Contact c;
    cout << "Фамилия: "; cin >> c.lastName;
    cout << "Имя: "; cin >> c.firstName;
    cout << "Отчество: "; cin >> c.middleName;

    do {
        cout << "Код страны (1-999): "; c.phone.countryCode = getSafeInt();
    } while (!isValidPhone(c.phone.countryCode)); // 

    cout << "Код города: "; c.phone.cityCode = getSafeInt();
    cout << "Номер: "; cin >> c.phone.number;

    cout << "Дата рождения (день месяц год): ";
    c.birthday.day = getSafeInt();
    c.birthday.month = getSafeInt();
    c.birthday.year = getSafeInt();

    do {
        cout << "Email: "; cin >> c.email;
    } while (!isValidEmail(c.email)); // 

    cout << "Категория (0-WORK, 1-FAMILY, 2-FRIENDS, 3-OTHERS): ";
    int cat = getSafeInt();
    c.category = static_cast<Group>(cat % 4);

    db.push_back(c);
}

// Просмотр записей (Таблица)
void showContacts(const vector<Contact>& db) {
    if (db.empty()) {
        cout << "Список пуст." << endl;
        return;
    }
    cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;
    cout << "| " << setw(18) << left << "Фамилия"
        << "| " << setw(18) << "Имя"
        << "| " << setw(15) << "Телефон"
        << "| " << setw(12) << "Дата рожд."
        << "| " << setw(10) << "Группа" << " |" << endl;
    cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;

    for (const auto& c : db) {
        string fullPhone = "+" + to_string(c.phone.countryCode) + "(" + to_string(c.phone.cityCode) + ")" + to_string(c.phone.number);
        string fullDate = to_string(c.birthday.day) + "." + to_string(c.birthday.month) + "." + to_string(c.birthday.year);

        cout << "| " << setw(18) << left << formatName(c.lastName) // 
            << "| " << setw(18) << formatName(c.firstName)
            << "| " << setw(15) << (fullPhone.length() > 15 ? fullPhone.substr(0, 12) + "..." : fullPhone)
            << "| " << setw(12) << fullDate
            << "| " << setw(10) << groupToString(c.category) << " |" << endl;
    }
    cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;
}

// Сохранение и загрузка
void saveToFile(const vector<Contact>& db) {
    ofstream out("database.txt");
    for (const auto& c : db) {
        out << c.lastName << " " << c.firstName << " " << c.middleName << " "
            << c.phone.countryCode << " " << c.phone.cityCode << " " << c.phone.number << " "
            << c.birthday.day << " " << c.birthday.month << " " << c.birthday.year << " "
            << c.email << " " << static_cast<int>(c.category) << endl;
    }
}

void loadFromFile(vector<Contact>& db) {
    ifstream in("database.txt");
    if (!in.is_open()) return;
    Contact c;
    int cat;
    while (in >> c.lastName >> c.firstName >> c.middleName >> c.phone.countryCode
        >> c.phone.cityCode >> c.phone.number >> c.birthday.day
        >> c.birthday.month >> c.birthday.year >> c.email >> cat) {
        c.category = static_cast<Group>(cat);
        db.push_back(c);
    }
}

// Поиск по нескольким словам
void searchContacts(const vector<Contact>& db) {
    cout << "Введите ключевые слова через пробел: ";
    string input;
    cin.ignore();
    getline(cin, input);

    bool found = false;
    for (const auto& c : db) {
        if (c.lastName.find(input) != string::npos || c.firstName.find(input) != string::npos) {
            cout << "Найден: " << c.lastName << " " << c.firstName << endl;
            found = true;
        }
    }
    if (!found) cout << "Контакты не найдены." << endl; // 
}

// Сортировка
void sortContacts(vector<Contact>& db) {
    cout << "1. По алфавиту\n2. По дате рождения\nВыбор: ";
    int choice = getSafeInt();
    if (choice == 1) {
        sort(db.begin(), db.end(), [](const Contact& a, const Contact& b) {
            return (a.lastName + a.firstName) < (b.lastName + b.firstName); // 
            });
    }
    else {
        sort(db.begin(), db.end(), [](const Contact& a, const Contact& b) {
            if (a.birthday.year != b.birthday.year) return a.birthday.year < b.birthday.year;
            if (a.birthday.month != b.birthday.month) return a.birthday.month < b.birthday.month;
            return a.birthday.day < b.birthday.day; // 
            });
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<Contact> database; // 
    loadFromFile(database);   // 

    int choice;
    do {
        cout << "\n--- Smart-Contacts ---\n"
            << "1. Добавить\n2. Просмотр\n3. Поиск\n4. Сортировка\n5. Выход\nВыбор: ";
        choice = getSafeInt();

        switch (choice) {
        case 1: addContact(database); break;
        case 2: showContacts(database); break;
        case 3: searchContacts(database); break;
        case 4: sortContacts(database); break;
        }
    } while (choice != 5); // 

    saveToFile(database); // 
    return 0;
}