#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <regex> 

using namespace std;

class bank {
private:
    string nam[100];
    int tot[100];
    int accNo[100];
    string passwords[100];
    bool active[100];
    string dates[100];
    string cities[100];
    int n;

public:
    bank() {
        for (int i = 0; i < 100; i++) {
            tot[i] = 0;
            active[i] = false;
        }
        n = 0;
    }

    bool isValidDate(const string& dob) {
    
        regex pattern(R"((0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(\d{4}))");

        
        if (regex_match(dob, pattern)) {
            int day = stoi(dob.substr(0, 2));
            int month = stoi(dob.substr(3, 2));
            int year = stoi(dob.substr(6, 4));

            
            if (month == 2) {
                
                bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                if ((isLeap && day > 29) || (!isLeap && day > 28)) {
                    return false;
                }
            } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                if (day > 30) {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    void newacc() {
        int i; 

        try {
            cout << "How many accounts you want to create: ";
            cin >> n;

            if (cin.fail()) {
                throw runtime_error("Invalid input. Please enter a valid integer.");
            }

            srand(time(0));
            for (i = 1; i <= n; i++) {
                cout << "\nEnter your name: ";
                string name;
                cin >> name;

                bool isValidName = true;
                for (char c : name) {
                    if (!isalpha(c)) {
                        isValidName = false;
                        break;
                    }
                }

                while (!isValidName) {
                    cout << "Invalid input. Name should contain only alphabetic characters. Please enter your name again: ";
                    cin >> name;
                    isValidName = true;
                    for (char c : name) {
                        if (!isalpha(c)) {
                            isValidName = false;
                            break;
                        }
                    }
                }

                nam[i] = name;

                tot[i] = 0;
                active[i] = true;
                accNo[i] = rand() % 100000 + 1000;

                do {
                    cout << "Enter the D.O.B (DD/MM/YYYY): ";
                    cin >> dates[i];
                    if (!isValidDate(dates[i])) {
                        cout << "Invalid date format. Please enter a valid date in DD/MM/YYYY format.\a" << endl;
                    }
                } while (!isValidDate(dates[i]));

                cout << "Enter your city: ";
                cin >> cities[i];
                string password;
                bool isValid = true;
                do {
                    cout << "Set a 4-digit password for your account: ";
                    cin >> password;
                    isValid = true;
                    if (password.length() != 4) {
                        isValid = false;
                    } else {
                        for (size_t j = 0; j < password.length(); j++) {
                            if (!isdigit(password[j])) {
                                isValid = false;
                                break;
                            }
                        }
                    }
                    if (!isValid) {
                        cout << "Password must be exactly 4 digits long. Try again.\a" << endl;
                    }
                } while (!isValid);
                passwords[i] = password;
                cout << "Your account password is set.\nYour account number is: " << accNo[i] << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            newacc();
        }
    }
    

    void display() {
        int choice;
        while (true) {
            cout << "\n" << endl;
            cout << "----------------------------" << endl;
            cout << "            MENU            " << endl;
            cout << "----------------------------" << endl;
            cout << "\nPress 1 to deposit";
            cout << "\nPress 2 to withdraw";
            cout << "\nPress 3 to check balance of all accounts";
            cout << "\nPress 4 to check balance of a specific account";
            cout << "\nPress 5 to delete an account";
            cout << "\nPress 6 to show all accounts";
            cout << "\nPress 7 to update account information";
            cout << "\nPress 8 to exit";
            cout << "\nPress any (1-8) :";
            cin >> choice;
            switch (choice) {
                case 1:
                    deposit();
                    break;
                case 2:
                    withdraw();
                    break;
                case 3:
                    balance();
                    break;
                case 4:
                    specificBalance();
                    break;
                case 5:
                    deleteAccount();
                    break;
                case 6:
                    showAllAccounts();
                    break;
                case 7:
                    updateInfo();
                    break;
                case 8:
                    cout << "Thanks for using my program" << endl;
                    return;
                default:
                    cout << "Invalid option, try again." << endl;
            }
        }
    }

    void deposit() {
        string password;
        cout << "\nEnter your account password: ";
        cin >> password;
        int acc = findAccountByPassword(password);
        if (acc != -1) {
            int dep;
            cout << "Enter the amount to deposit: ";
            cin >> dep;
            tot[acc] += dep;
            cout << nam[acc] << ", your new balance is " << tot[acc] << endl;
        } else {
            cout << "Invalid account password.\a" << endl;
        }
    }

    void withdraw() {
        string password;
        cout << "\nEnter your account password: ";
        cin >> password;
        int acc = findAccountByPassword(password);
        if (acc != -1) {
            int with;
            cout << "Enter the amount to withdraw: ";
            cin >> with;
            if (with <= tot[acc]) {
                tot[acc] -= with;
                cout << nam[acc] << ", your new balance is " << tot[acc] << endl;
            } else {
                cout << "Insufficient funds.\a" << endl;
            }
        } else {
            cout << "Invalid account password.\a" << endl;
        }
    }

    void balance() {
        for (int i = 1; i <= n; i++) {
            if (active[i]) {
                cout << "\n" << nam[i] << ", your balance is " << tot[i];
            }
        }
        cout << endl;
    }

    void specificBalance() {
        string password;
        cout << "\nEnter your account password: ";
        cin >> password;
        int acc = findAccountByPassword(password);
        if (acc != -1) {
            cout << nam[acc] << ", your balance is " << tot[acc] << endl;
        } else {
            cout << "Invalid account password.\a" << endl;
        }
    }

    void deleteAccount() {
        string password;
        cout << "\nEnter your account password: ";
        cin >> password;
        int acc = findAccountByPassword(password);
        if (acc != -1) {
            active[acc] = false;
            tot[acc] = 0;
            cout << "Account for " << nam[acc] << " has been deleted." << endl;
        } else {
            cout << "Invalid account password.\a" << endl;
        }
    }

    void showAllAccounts() {
        cout << "\nAll Active Accounts:" << endl;
        for (int i = 1; i <= n; i++) {
            if (active[i]) {
                cout << "Account Number: " << accNo[i] << ", Name: " << nam[i] <<
				 ", Balance: " << tot[i] << ", D.O.B: " << dates[i] << ", City: " << cities[i] << endl;
            }
        }
        if (n == 0) {
            cout << "No active accounts available." << endl;
        }
        else
        {
        	cout<<"no account active"<<endl;
		}
    }

    void updateInfo() {
        string password;
        cout << "\nEnter your account password: ";
        cin >> password;
        int acc = findAccountByPassword(password);
        if (acc != -1) {
            cout << "Account found. You can update your information now." << endl;
            
            cout << "Enter your new name (or press enter to keep current): ";
            string name;
            cin.ignore(); 
            getline(cin, name);
            if (!name.empty()) {
                bool isValidName = true;
                for (char c : name) {
                    if (!isalpha(c)) {
                        isValidName = false;
                        break;
                    }
                }

                while (!isValidName) {
                    cout << "Invalid input. Name should contain only alphabetic characters."<<endl;
					cout<<" Please enter your name again: ";
                    getline(cin, name);
                    isValidName = true;
                    for (char c : name) {
                        if (!isalpha(c)) {
                            isValidName = false;
                            break;
                        }
                    }
                }
                nam[acc] = name;
            }

            cout << "Enter your new city (or press enter to keep current): ";
            string city;
            getline(cin, city);
            if (!city.empty()) {
                cities[acc] = city;
            }

            string dob;
            do {
                cout << "Enter the new D.O.B (DD/MM/YYYY) (or press enter to keep current): ";
                getline(cin, dob);
                if (dob.empty() || isValidDate(dob)) {
                    break;
                } else {
                    cout << "Invalid date format. Please enter a valid date in DD/MM/YYYY format.\a" << endl;
                }
            } while (true);
            if (!dob.empty()) {
                dates[acc] = dob;
            }

            cout << "Information updated successfully." << endl;
        } else {
            cout << "Invalid account password.\a" << endl;
        }
    }

    int findAccountByPassword(string password) {
        for (int i = 1; i <= n; i++) {
            if (active[i] && passwords[i] == password) {
                return i;
            }
        }
        return -1;
    }
};

int main() {
    system("color E4");
    cout << "                                           WELCOME TO APNA ACCOUNT             " << endl;
    cout << "                                    Hissab kitab ka chakkar, ab hua assan!     " << endl;

    bank s1;
    s1.newacc();
    s1.display();
    return 0;
}

