#pragma once

#include <iostream>
#include <string>

using namespace std;

class Admin
{
private:
    string username;
    string password;

public:
    Admin(string u, string p);
    string get_username() const;
    bool check_password(const string &password) const;
};
