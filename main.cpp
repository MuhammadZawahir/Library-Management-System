#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;
// Declaring constants
const int BOOK_LIMIT = 100;
const int SECONDS_IN_DAY = 86400;
const int FINE_PER_DAY = 2;
const int DAYS_ALLOWED = 1;

// defining structures
struct Book
{
    int BookID;
    string bookName;
    string bookAuthor;
    bool available;
};

struct Transaction
{
    int BookID;
    string username;
    string transactionType;
    time_t borrowingTime;
    time_t returningTime;
};

// Function Prototypes
void menu();
void addBook(Book library[], int &bookCount);
void displayBooks(Book library[], int &bookCount);
void borrowBook(Book library[], int &bookCount, Transaction Ttransaction[], int &transactionCount);
void returnBook(Book library[], int &bookCount, Transaction Ttransaction[], int &transactionCount);
void saveToFile(Book library[], int &bookCount);
void saveTransactionsToFile(Transaction Ttransaction[], int transactionCount);
void loadFromFile(Book library[], int &bookCount);
void loadTransactionsFromFile(Transaction Ttransaction[], int &transactionCount);
void removeTransaction(Transaction Ttransaction[], int &transactionCount, int index);
void calculateFine(Transaction &transaction);
void printTransactionsFromFile(Transaction Ttransaction[], int transactionCount);

int main()
{
    Book library[BOOK_LIMIT];
    int bookCount = 0;        // starting index of the library array
    int transactionCount = 0; // starting index of transaction array
    int choice;

    Transaction Ttransaction[BOOK_LIMIT];

    loadFromFile(library, bookCount);
    loadTransactionsFromFile(Ttransaction, transactionCount); // calling functions so that the data saved in files can be extracted
    do
    {
        menu();
        cout << "Enter you choice: "; // asking the user to select what action they want to perform
        cin >> choice;

        switch (choice) // switch case statements that will call the function that the user wants
        {
        case 1:
            addBook(library, bookCount);

            break;
        case 2:
            displayBooks(library, bookCount);
            cout << "\nPress enter to continue";
            cin.ignore();
            cin.get();

            break;
        case 3:
            borrowBook(library, bookCount, Ttransaction, transactionCount);
            cout << "\nPress enter to continue";
            cin.get();

            break;
        case 4:
            returnBook(library, bookCount, Ttransaction, transactionCount);
            cout << "\nPress enter to continue";
            cin.ignore();
            cin.get();

            break;
        case 5:
            printTransactionsFromFile(Ttransaction, transactionCount);
            cout << "\nPress enter to continue";
            cin.ignore();
            cin.get();

            break;
        case 0:
            cout << "\nGoodBye!" << endl;

            return 0;
        default:
            cout << "\nInvalid input, please enter again.\nPress enter to continue"; // If the user enter invalid input like anything other than from 0 to 5 is invalid or if the user entered the wrong dataType variable
            cin.ignore();
            cin.get();
        }

        saveToFile(library, bookCount);
        saveTransactionsToFile(Ttransaction, transactionCount);

    } while (choice != 0);

    return 0;
}

void menu() // displaying all the available options
{
    cout << "\n\tLibrary Management System\t\n"
         << endl;
    cout << "What would you like to do?\n";
    cout << "1. Add a new book" << endl;
    cout << "2. Display all book in the library" << endl;
    cout << "3. Borrow a Book" << endl;
    cout << "4. Return a Book" << endl;
    cout << "5. Display Transaction History" << endl;
    cout << "0. Exit" << endl
         << endl;
    return;
}

void addBook(Book library[], int &bookCount)
{
    if (bookCount < BOOK_LIMIT)
    {
        bool ID = false; // flag is initially set to false
        Book newBook;
        // taking input about book and details
        cout << "Enter Book Name:";
        cin.ignore();
        getline(cin, newBook.bookName);
        cout << "Enter Author's Name:";
        getline(cin, newBook.bookAuthor);
        do
        {
            cout << "Enter book ID:";
            cin >> newBook.BookID;
            ID = true;
            for (int i = 0; i < bookCount; i++)
            {
                if (newBook.BookID == library[i].BookID)
                {
                    cout << "This book ID already exists" << endl; // if another book with the same ID already exists, it should not store another book in the same ID
                    ID = false;
                    break;
                }
            }
        } while (!ID);

        newBook.available = true;       // since the book has just been added, so it is available
        library[bookCount++] = newBook; // for the next iteration
        cout << "\nBook successfully added to library." << endl;
    }
    else
    {
        cout << "\nLimit reached. The book can not be added to the library." << endl;
    }
    cout << "\nPress enter to continue";
    cin.ignore();
    cin.get();
}

void displayBooks(Book library[], int &bookCount) // display all the available books

{
    if (bookCount > 0) // if any books are in the library, then the loop will run
    {
        cout << "----------------------------------------------------------------------------------------------" << endl;
        cout << setw(10) << "ID" << setw(25) << "Book Name" << setw(25) << "Author Name" << setw(30) << "Availability Status" << endl; // for a presentable display
        string status;

        for (int i = 0; i < bookCount; i++) // loop will continue until the last book in the library
        {
            if (library[i].available == true) // If the bool variable os status is true, then it is available as we have done earlier and vice versa
            {
                status = "Available";
            }
            else
            {
                status = "Borrowed";
            }
            cout << "----------------------------------------------------------------------------------------------" << endl;
            cout << setw(10) << library[i].BookID << setw(25) << library[i].bookName << setw(25) << library[i].bookAuthor << setw(25) << status << endl;
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;
    }
    else
    {
        cout << "\nNo books in the library to display." << endl; // if no books, nothing to display
    }

    return;
}

// borrow a book from the library
void borrowBook(Book library[], int &bookCount, Transaction Ttransaction[], int &transactionCount)
{
    int bookID = 0;
    if (bookCount > 0)
    {
        cout << "----------------------------------------------------------------------------------------------" << endl;
        cout << setw(10) << "ID" << setw(25) << "Book Name" << setw(25) << "Author Name" << setw(30) << "Availability Status" << endl;
        string status;

        // showing the user all the books so that they know what they want to borrow
        for (int i = 0; i < bookCount; i++)
        {
            if (library[i].available == true)
            {
                status = "Available";
            }
            else
            {
                status = "Borrowed";
            }
            cout << "----------------------------------------------------------------------------------------------" << endl;
            cout << setw(10) << library[i].BookID << setw(25) << library[i].bookName << setw(25) << library[i].bookAuthor << setw(25) << status << endl;
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;

        cout << "Enter ID of the book you want to borrow: ";
        cin >> bookID;

        for (int i = 0; i < bookCount; i++)
        {
            if (library[i].BookID == bookID && library[i].available)
            {
                char confirm;
                cout << "\nDo you want to borrow the book \"" << library[i].bookName << "\"? (y/n) "; // confirming from the user if they want the same book by telling them the name of the book
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y')
                {
                    Transaction transaction;
                    transaction.BookID = bookID;
                    cout << "Enter your name: ";
                    cin.ignore();
                    getline(cin, transaction.username);
                    transaction.transactionType = "Borrow";
                    transaction.borrowingTime = time(0);
                    library[i].available = false;
                    Ttransaction[transactionCount++] = transaction;
                    cout << "\nBook borrowed successfully. Please return it in time!" << endl;
                }
            }
            else if (library[i].BookID < bookID || library[i].BookID > bookID)
            {
                // Empty so that the for loop works properly
            }
            else
            {
                cout << "\nBook not available or already borrowed." << endl;
            }
        }
    }
    else
    {
        cout << setw(20) << "\nNo book available" << endl;
    }
}

// returning a borrowed book
void returnBook(Book library[], int &bookCount, Transaction Ttransaction[], int &transactionCount)
{
    int bookID;
    if (transactionCount > 0)
    {
        cout << "----------------------------------------------------------------------------------------------" << endl; // for readability
        cout << setw(10) << "ID" << setw(25) << "Book Name" << setw(25) << "Author Name" << setw(30) << "Availability Status" << endl;
        string status;

        for (int i = 0; i < bookCount; i++)
        {
            // if the book is not borrowed it will show "Available" in status column and if it is borrowed, it shows "Borrowed".
            if (library[i].available == true)
            {
                status = "Available";
            }
            else
            {
                status = "Borrowed";
            }
            cout << "----------------------------------------------------------------------------------------------" << endl;
            cout << setw(10) << library[i].BookID << setw(25) << library[i].bookName << setw(25) << library[i].bookAuthor << setw(25) << status << endl;
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;
        cout << "\nEnter ID of the book you want to return: ";
        cin >> bookID;

        // loop for calculating transaction
        for (int i = 0; i < transactionCount; i++)
        {
            if (Ttransaction[i].BookID == bookID)
            {
                Transaction transaction = Ttransaction[i];
                for (int j = 0; j < bookCount; j++)
                {
                    if (library[j].BookID == bookID)
                    {
                        library[j].available = true;
                        break;
                    }
                }
                transaction.returningTime = time(0);
                calculateFine(transaction);
                removeTransaction(Ttransaction, transactionCount, i);
                cout << "\nBook returned successfully" << endl;
                return;
            }
        }

        cout << "\nBook not found in transactions" << endl;
    }
    else
    {
        cout << setw(20) << "No book available for returning" << endl;
    }
}

// Print transactions from file
void printTransactionsFromFile(Transaction Ttransaction[], int transactionCount)
{
    ifstream inFile("transactions.txt");

    if (!inFile)
    {
        cout << "Starting with an empty transaction history." << endl;
        return;
    }

    if (transactionCount > 0)
    {
        cout << setw(65) << "TRANSACTION HISTORY" << endl;
        cout << setw(20) << "Book ID " << setw(15) << "Customer Name" << setw(20) << "Transaction Type" << setw(23) << "Borrowing Date" << endl;
        cout << setw(20) << "---------------------------------------------------------------------------------------" << endl;
        for (int i = 0; i < transactionCount; i++)
        {
            inFile >> Ttransaction[i].BookID >> Ttransaction[i].username >> Ttransaction[i].transactionType >> Ttransaction[i].borrowingTime >> Ttransaction[i].returningTime;

            tm tm_borrow = *localtime(&Ttransaction[i].borrowingTime);
            char borrowingTimeString[20];
            strftime(borrowingTimeString, sizeof(borrowingTimeString), "%Y-%m-%d %H:%M:%S", &tm_borrow);

            cout << setw(17) << Ttransaction[i].BookID << setw(15) << Ttransaction[i].username << setw(18) << Ttransaction[i].transactionType << setw(31) << borrowingTimeString << endl;
        }
        cout << setw(20) << "---------------------------------------------------------------------------------------" << endl;

        inFile.close();
    }
    else
    {
        cout << "\nNo Transactions were found." << endl;
    }
}

// Save books to file
void saveToFile(Book library[], int &bookCount) // Saving the data in a text file, so we can use the data even when we have exited the program and we are running it again
{
    ofstream outFile;
    outFile.open("books.txt"); // declaring a text file to write, this will make a text file if not already made and if it is already made, it will write in that file

    if (outFile.is_open()) // if there is no error in opening the file
    {
        outFile << bookCount;
        for (int i = 0; i < bookCount; ++i)
        {
            // storing data in the text file
            outFile << library[i].bookName << endl
                    << library[i].bookAuthor << endl
                    << library[i].BookID << endl
                    << library[i].available << endl;
        }
        outFile.close();
    }
    else // if there is an error opening file
    {
        cout << "Error: Cannot open file" << endl;
    }
    return;
}

// Save transactions to file
void saveTransactionsToFile(Transaction Ttransaction[], int transactionCount)
{
    ofstream outFile("transactions.txt");

    if (!outFile)
    {
        cout << "Error opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < transactionCount; i++)
    {
        outFile << Ttransaction[i].BookID << " "
                << Ttransaction[i].username << " "
                << Ttransaction[i].transactionType << " "
                << (Ttransaction[i].borrowingTime) << " "
                << Ttransaction[i].returningTime << endl;
    }

    outFile.close();
}

// Load books from file
void loadFromFile(Book library[], int &bookCount) // To read the data that we saved in the file in the previous function
{
    ifstream inFile;
    inFile.open("books.txt"); // declaring a text file to read, this wil read from the text file that we created in the above function
    if (inFile.is_open())     // if ther is no error in opening the file
    {
        inFile >> ws >> bookCount >> ws;
        for (int i = 0; i < bookCount; i++)
        {
            // reading data from file
            getline(inFile, library[i].bookName, '\n');
            getline(inFile, library[i].bookAuthor, '\n');
            inFile >> library[i].BookID >> ws >> library[i].available;
            inFile.ignore();
        }
        inFile.close();
    }
    else // if error while opening file
    {
        cout << "Error: Could not load data from file." << endl;
    }
    return;
}

// Load transactions from file
void loadTransactionsFromFile(Transaction Ttransaction[], int &transactionCount)
{
    ifstream inFile("transactions.txt");

    if (!inFile)
    {
        cout << "Starting with an empty transaction history." << endl;
        return;
    }

    for (; transactionCount < BOOK_LIMIT && inFile >> Ttransaction[transactionCount].BookID >> Ttransaction[transactionCount].username >> Ttransaction[transactionCount].transactionType >> Ttransaction[transactionCount].borrowingTime >> Ttransaction[transactionCount].returningTime; ++transactionCount)
    {
    }

    inFile.close();
}

// removing transactions
void removeTransaction(Transaction Ttransaction[], int &transactionCount, int index)
{
    for (int i = index; i < transactionCount - 1; i++)
    {
        Ttransaction[i] = Ttransaction[i + 1];
    }
    transactionCount--;
}

// fine for late returning
void calculateFine(Transaction &transaction)
{

    int totalDays = (transaction.returningTime - transaction.borrowingTime) / SECONDS_IN_DAY;

    if (totalDays > 1)
    {
        int fine = (totalDays - DAYS_ALLOWED) * FINE_PER_DAY;
        cout << "\nPay Fine for late return: $" << fine << endl;
    }
    else
    {
        cout << "\nNo fine!You have returned the book in time." << endl;
    }
}