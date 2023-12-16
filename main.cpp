#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

// Define color codes for better visibility
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

// Forward declaration of Admin class
class Admin;

// Employee class
class Employee
{
private:
    string username;
    string password;
    bool isLoggedIn;
    chrono::time_point<chrono::system_clock> clockInTime;
    chrono::time_point<chrono::system_clock> clockOutTime;

public:
    // Constructor for Employee class
    Employee(const string &u, const string &p) : username(u), password(p), isLoggedIn(false) {}

    // Method to record clock in time
    void clockIn()
    {
        if (!isLoggedIn)
        {
            cout << "Please log in first." << endl;
            return;
        }

        clockInTime = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(clockInTime);
        struct tm *timeInfo = localtime(&currentTime);
        cout << "Clocking in at " << YELLOW << put_time(timeInfo, "%F %I:%M:%S %p") << RESET << endl;
        saveClockTime("clock_times.txt", true);
    }

    // Method to record clock out time and calculate elapsed time
    void clockOut()
    {
        if (!isLoggedIn)
        {
            cout << "Please log in first." << endl;
            return;
        }

        cout << "Clocking out..." << endl;
        clockOutTime = chrono::system_clock::now();
        saveClockTime("clock_times.txt", false);
        chrono::duration<double> elapsedSeconds = clockOutTime - clockInTime;
        cout << "Clock out successful. Elapsed time: " << YELLOW << elapsedSeconds.count() << " seconds" << RESET << endl;
    }

    // Method to view employee's schedule
    void viewSchedule()
    {
        if (!isLoggedIn)
        {
            cout << "Please log in first." << endl;
            return;
        }

        ifstream file("schedule.txt");
        string line;
        while (getline(file, line))
        {
            if (line.find(username) != string::npos)
            {
                cout << "Your schedule: " << YELLOW << line << RESET << endl;
                break;
            }
        }
        file.close();
    }

    // Method to log in employee
    bool login(const string &enteredUsername, const string &enteredPassword)
    {
        if (enteredUsername == username && enteredPassword == password)
        {
            cout << "Login successful." << endl;
            isLoggedIn = true;
            return true;
        }
        else
        {
            cout << "Incorrect username or password." << endl;
            return false;
        }
    }

    // Method to check login status
    bool isLoggedInStatus() const
    {
        return isLoggedIn;
    }

    // Method to get employee's username
    const string &getUsername() const
    {
        return username;
    }

    // Method to save clock time to file
    void saveClockTime(const string &fileName, bool isClockIn)
    {
        ofstream file(fileName, ios::app);
        if (isClockIn)
        {
            time_t inTime = chrono::system_clock::to_time_t(clockInTime);
            file << username << " Clock In: " << YELLOW << put_time(localtime(&inTime), "%F %I:%M:%S %p") << RESET << endl;
        }
        else
        {
            time_t outTime = chrono::system_clock::to_time_t(clockOutTime);
            file << username << " Clock Out: " << YELLOW << put_time(localtime(&outTime), "%F %I:%M:%S %p") << RESET << endl;
        }
        file.close();
    }

    // Friend function declaration for logout
    friend void logout(Employee &emp);
};

// Admin class
class Admin
{
private:
    string username;
    string password;

public:
    // Constructor for Admin class
    Admin(const string &u, const string &p) : username(u), password(p) {}

    // Method to log in admin
    bool login(const string &enteredUsername, const string &enteredPassword)
    {
        return (enteredUsername == username && enteredPassword == password);
    }

    // Method to add a schedule
    void addSchedule(const string &schedule, const string &fileName)
    {
        ofstream file(fileName, ios::app);
        file << schedule << endl;
        file.close();
        cout << "Schedule added successfully." << endl;
    }

    // Friend function declaration for admin logout
    friend void logout(Admin &admin);

    // Static method to find the index of an employee in the vector
    static size_t findEmployeeIndex(const vector<string> &employeeUsernames, const string &targetUsername)
    {
        auto it = find(employeeUsernames.begin(), employeeUsernames.end(), targetUsername);
        if (it != employeeUsernames.end())
        {
            return distance(employeeUsernames.begin(), it);
        }
        else
        {
            return -1;
        }
    }

    // Friend function declarations for additional functionality
    friend void changeEmployeeCredentials(Admin &admin, vector<string> &employeeUsernames, vector<string> &employeePasswords);
    friend void createEmployee(Admin &admin, vector<string> &employeeUsernames, vector<string> &employeePasswords);
};

// Friend function definition for Employee logout
void logout(Employee &emp)
{
    cout << "Logging out..." << endl;
    emp.isLoggedIn = false;
}

// Friend function definition for Admin logout
void logout(Admin &admin)
{
    cout << "Logging out..." << endl;
}

// Additional functionality for Admin class - change employee credentials
void changeEmployeeCredentials(Admin &admin, vector<string> &employeeUsernames, vector<string> &employeePasswords)
{
    string targetUsername;
    cout << "Enter the username of the employee to change credentials: ";
    cin >> targetUsername;

    size_t index = Admin::findEmployeeIndex(employeeUsernames, targetUsername);
    if (index != -1)
    {
        cout << "Enter new username for employee: ";
        cin >> employeeUsernames[index];
        cout << "Enter new password for employee: ";
        cin >> employeePasswords[index];
        cout << "Credentials changed successfully." << endl;
    }
    else
    {
        cout << "Employee not found." << endl;
    }
}

// Additional functionality for Admin class - create a new employee
void createEmployee(Admin &admin, vector<string> &employeeUsernames, vector<string> &employeePasswords)
{
    string newUsername;
    cout << "Enter the username for the new employee: ";
    cin >> newUsername;

    if (Admin::findEmployeeIndex(employeeUsernames, newUsername) == -1)
    {
        cout << "Enter password for the new employee: ";
        string newPassword;
        cin >> newPassword;

        employeeUsernames.push_back(newUsername);
        employeePasswords.push_back(newPassword);
        cout << "New employee created successfully." << endl;
    }
    else
    {
        cout << "Username already exists. Please choose a different username." << endl;
    }
}

// Function to read usernames and passwords from a file
void readUsernamesAndPasswords(const string &fileName, vector<string> &usernames, vector<string> &passwords)
{
    ifstream file(fileName);
    string username, password;
    while (file >> username >> password)
    {
        usernames.push_back(username);
        passwords.push_back(password);
    }
    file.close();
}

int main()
{
    // Read employee usernames and passwords from a file
    vector<string> employeeUsernames, employeePasswords;
    readUsernamesAndPasswords("employee.txt", employeeUsernames, employeePasswords);

    // Read admin username and password from a file
    vector<string> adminUsernames, adminPasswords;
    readUsernamesAndPasswords("admin.txt", adminUsernames, adminPasswords);

    string enteredUsername, enteredPassword;

    do
    {
        // Get username and password from the user
        cout << "Enter username (or 'exit' to quit): ";
        cin >> enteredUsername;

        // Check if the user wants to exit
        if (enteredUsername == "exit")
        {
            break;
        }

        cout << "Enter password: ";
        cin >> enteredPassword;

        // Check if the user is an employee
        bool isEmployee = false;
        size_t empIndex = 0; // To keep track of the employee index
        for (size_t i = 0; i < employeeUsernames.size(); ++i)
        {
            if (enteredUsername == employeeUsernames[i] && enteredPassword == employeePasswords[i])
            {
                isEmployee = true;
                empIndex = i;
                break;
            }
        }

        // Check if the user is an admin
        bool isAdmin = false;
        for (size_t i = 0; i < adminUsernames.size(); ++i)
        {
            if (enteredUsername == adminUsernames[i] && enteredPassword == adminPasswords[i])
            {
                isAdmin = true;
                break;
            }
        }

        if (isEmployee)
        {
            // Create an employee with a username
            Employee employee(enteredUsername, employeePasswords[empIndex]);

            // Try to log in
            while (!employee.login(enteredUsername, enteredPassword))
            {
                // If login is unsuccessful, ask again
                cout << "Please enter valid username and password." << endl;
                cout << "Enter username (or 'exit' to quit): ";
                cin >> enteredUsername;

                // Check if the user wants to exit
                if (enteredUsername == "exit")
                {
                    break;
                }

                cout << "Enter password: ";
                cin >> enteredPassword;
            }

            // Check if the user wants to exit
            if (enteredUsername == "exit")
            {
                break;
            }

            // If login is successful, provide clock-in/clock-out options and schedule view
            int choice;

            do
            {
                cout << BLUE << "\n----- Employee Menu -----" << RESET << endl;
                cout << "1. Clock In" << endl;
                cout << "2. Clock Out" << endl;
                cout << "3. View Schedule" << endl;
                cout << "4. Logout" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice)
                {
                case 1:
                    employee.clockIn();
                    break;
                case 2:
                    employee.clockOut();
                    break;
                case 3:
                    employee.viewSchedule();
                    break;
                case 4:
                    logout(employee);
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
                }
            } while (choice != 4);
        }
        else if (isAdmin)
        {
            // Create an admin with a username
            Admin admin(enteredUsername, enteredPassword);

            // Try to log in
            while (!admin.login(enteredUsername, enteredPassword))
            {
                // If login is unsuccessful, ask again
                cout << "Please enter valid username and password." << endl;
                cout << "Enter username (or 'exit' to quit): ";
                cin >> enteredUsername;

                // Check if the user wants to exit
                if (enteredUsername == "exit")
                {
                    break;
                }

                cout << "Enter password: ";
                cin >> enteredPassword;
            }

            // Check if the user wants to exit
            if (enteredUsername == "exit")
            {
                break;
            }

            // If login is successful, provide options to add a schedule
            int choice;

            do
            {
                cout << BLUE << "\n----- Admin Menu -----" << RESET << endl;
                cout << "1. Add Schedule" << endl;
                cout << "2. Change Employee Credentials" << endl;
                cout << "3. Create Employee" << endl;
                cout << "4. Logout" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice)
                {
                case 1:
                {
                    string schedule;
                    cout << "Enter the schedule: ";
                    cin.ignore();
                    getline(cin, schedule);
                    admin.addSchedule(schedule, "schedule.txt");
                }
                break;
                case 2:
                    changeEmployeeCredentials(admin, employeeUsernames, employeePasswords);
                    break;
                case 3:
                    createEmployee(admin, employeeUsernames, employeePasswords);
                    break;
                case 4:
                    logout(admin);
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
                }
            } while (choice != 4);
        }
        else
        {
            cout << "Invalid username or password." << endl;
        }
    } while (true);

    cout << "Exiting..." << endl;

    return 0;
}