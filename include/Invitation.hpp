#pragma once

#include <iostream>
#include <string>

using namespace std;

class Invitation
{
private:
    int id;
    string from_username;
    string to_username;
    string match_type;
    bool active;

public:
    Invitation(int id, string from, string to, string type);

    int get_id() const;
    string get_from_username() const;
    string get_to_username() const;
    string get_match_type() const;
    bool is_active() const;
    void deactivate();
};