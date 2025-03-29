#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Book {
private:
    string title;
    string author;
    int id;
    bool available;

public:
    Book(string t, string a, int i) : title(t), author(a), id(i), available(true) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getId() const { return id; }
    bool isAvailable() const { return available; }

    void checkOut() { available = false; }
    void returnBook() { available = true; }

    void display() const {
        cout << "ID: " << id << "\nTitle: " << title 
             << "\nAuthor: " << author << "\nStatus: " 
             << (available ? "Available" : "Checked Out") << endl;
    }
};

class User {
private:
    string name;
    int id;
    vector<int> borrowedBooks;

public:
    User(string n, int i) : name(n), id(i) {}

    string getName() const { return name; }
    int getId() const { return id; }
    const vector<int>& getBorrowedBooks() const { return borrowedBooks; }

    void borrowBook(int bookId) {
        borrowedBooks.push_back(bookId);
    }

    void returnBook(int bookId) {
        borrowedBooks.erase(remove(borrowedBooks.begin(), borrowedBooks.end(), bookId), 
                           borrowedBooks.end());
    }

    void display() const {
        cout << "User ID: " << id << "\nName: " << name 
             << "\nBooks Borrowed: " << borrowedBooks.size() << endl;
    }
};

class Library {
private:
    vector<Book> books;
    vector<User> users;
    int nextBookId = 1;
    int nextUserId = 1;

public:
    void addBook(string title, string author) {
        books.emplace_back(title, author, nextBookId++);
    }

    void addUser(string name) {
        users.emplace_back(name, nextUserId++);
    }

    Book* findBook(int id) {
        for (auto& book : books) {
            if (book.getId() == id) return &book;
        }
        return nullptr;
    }

    User* findUser(int id) {
        for (auto& user : users) {
            if (user.getId() == id) return &user;
        }
        return nullptr;
    }

    void displayAllBooks() const {
        for (const auto& book : books) {
            book.display();
            cout << "-----------------" << endl;
        }
    }

    void displayAvailableBooks() const {
        for (const auto& book : books) {
            if (book.isAvailable()) {
                book.display();
                cout << "-----------------" << endl;
            }
        }
    }

    void checkOutBook(int userId, int bookId) {
        User* user = findUser(userId);
        Book* book = findBook(bookId);

        if (user && book && book->isAvailable()) {
            book->checkOut();
            user->borrowBook(bookId);
            cout << "Book checked out successfully!" << endl;
        } else {
            cout << "Operation failed. Check user ID, book ID and availability." << endl;
        }
    }

    void returnBook(int userId, int bookId) {
        User* user = findUser(userId);
        Book* book = findBook(bookId);

        if (user && book && !book->isAvailable()) {
            book->returnBook();
            user->returnBook(bookId);
            cout << "Book returned successfully!" << endl;
        } else {
            cout << "Operation failed. Check user ID and book ID." << endl;
        }
    }

    void saveData() {
        ofstream bookFile("books.txt"), userFile("users.txt");
        
        for (const auto& book : books) {
            bookFile << book.getId() << "," << book.getTitle() << "," 
                    << book.getAuthor() << "," << book.isAvailable() << "\n";
        }
        
        for (const auto& user : users) {
            userFile << user.getId() << "," << user.getName();
            for (int bookId : user.getBorrowedBooks()) {
                userFile << "," << bookId;
            }
            userFile << "\n";
        }
        
        cout << "Data saved successfully!" << endl;
    }

    void loadData() {
        ifstream bookFile("books.txt"), userFile("users.txt");
        string line;
        
        // Load books
        while (getline(bookFile, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1+1);
            size_t pos3 = line.find(',', pos2+1);
            
            int id = stoi(line.substr(0, pos1));
            string title = line.substr(pos1+1, pos2-pos1-1);
            string author = line.substr(pos2+1, pos3-pos2-1);
            bool available = line.substr(pos3+1) == "1";
            
            Book book(title, author, id);
            if (!available) book.checkOut();
            books.push_back(book);
            nextBookId = max(nextBookId, id+1);
        }
        
        // Load users
        while (getline(userFile, line)) {
            size_t pos1 = line.find(',');
            int id = stoi(line.substr(0, pos1));
            string name = line.substr(pos1+1);
            
            User user(name, id);
            size_t pos = name.find(',');
            if (pos != string::npos) {
                name = name.substr(0, pos);
                size_t start = pos+1;
                while ((pos = line.find(',', start)) != string::npos) {
                    int bookId = stoi(line.substr(start, pos-start));
                    user.borrowBook(bookId);
                    start = pos+1;
                }
                if (start < line.length()) {
                    int bookId = stoi(line.substr(start));
                    user.borrowBook(bookId);
                }
            }
            
            users.push_back(user);
            nextUserId = max(nextUserId, id+1);
        }
        
        cout << "Data loaded successfully!" << endl;
    }
};

void displayMenu() {
    cout << "\nLibrary Management System\n";
    cout << "1. Add Book\n";
    cout << "2. Add User\n";
    cout << "3. Display All Books\n";
    cout << "4. Display Available Books\n";
    cout << "5. Check Out Book\n";
    cout << "6. Return Book\n";
    cout << "7. Save Data\n";
    cout << "8. Load Data\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch(choice) {
            case 1: {
                string title, author;
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                library.addBook(title, author);
                break;
            }
            case 2: {
                string name;
                cout << "Enter user name: ";
                getline(cin, name);
                library.addUser(name);
                break;
            }
            case 3:
                library.displayAllBooks();
                break;
            case 4:
                library.displayAvailableBooks();
                break;
            case 5: {
                int userId, bookId;
                cout << "Enter user ID: ";
                cin >> userId;
                cout << "Enter book ID: ";
                cin >> bookId;
                library.checkOutBook(userId, bookId);
                break;
            }
            case 6: {
                int userId, bookId;
                cout << "Enter user ID: ";
                cin >> userId;
                cout << "Enter book ID: ";
                cin >> bookId;
                library.returnBook(userId, bookId);
                break;
            }
            case 7:
                library.saveData();
                break;
            case 8:
                library.loadData();
                break;
            case 9:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 9);
    
    return 0;
}