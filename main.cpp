#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool logIn();

int main(){
int choice;
cout << "Select a choice." << endl;
cout << "Register" << endl;
cout << "Login" << endl;
cout << "Your choice: " << endl;
cin >> choice;

if(choice == 1)
{
string username,
password;

cout << "Select a username: " << endl;
cin >> username;

cout << "Select a password: " << endl;
cin >> passname;
if {
ofstream file;
file.open(username + ".txt") // this creates a txt file for each employee, if you want to use a database ok, but I am not good at implementing a database in the code
file << username << endl << password; // stores the information in the given file
file.close

main();
}
else if(choice == 1)
{
bool status = logIn();
if(!status) {
cout << "Incorrect username or password. Please try again." << endl;
system("PAUSE");
return 0;
}
else {
cout << "Login successful." << endl;
system("PAUSE");
return 1;
}
}
}

bool login(){
string username,
password,
user, 
pass;

cout << "Enter username: " << endl;
cin >> username;

cout << "Enter password: " << endl;
cin >> password;

ifstream read(username + ".txt")
getline(read, user);
getline(read, pass);

if (user == username && pass == password)
{
return true;
}
else {
return false;
}
}
