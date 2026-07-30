#include "Admin.hpp"

Admin::Admin(string u, string p)
{
    username = u;
    password = p;
}

string Admin::get_username() const
{
    return username;
}

bool Admin::check_password(const string &password) const
{
    return this->password == password;
}