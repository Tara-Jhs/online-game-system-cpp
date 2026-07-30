#pragma once

#include <iostream>
#include <string>

using namespace std;

class Report
{
private:
    int id;
    string sender_username;
    string reported_username;
    string reason;

public:
    Report(int id, string sender, string reported, string reason);
    int get_id() const;
    string get_sender_username() const;
    string get_reported_username() const;
    string get_reason() const;
};