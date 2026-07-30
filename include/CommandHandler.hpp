#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "Errors.hpp"

using namespace std;

class GameSystem;

class CommandHandler
{
private:
    GameSystem &game_system;

    const string PUT = "PUT";
    const string GET = "GET";
    const string POST = "POST";
    const string DELETE = "DELETE";

    bool is_valid_method(const string &method) const;
    bool is_valid_command(const string &method, const string &command) const;
    map<string, string> parse_arguments(stringstream &ss) const;
    using Handler = void (GameSystem::*)(const map<string, string> &args);
    void execute_handler(const map<string, Handler> &handlers, const string &command, const map<string, string> &args);
    void execute_command(const string &method, const string &command, const map<string, string> &args);

public:
    CommandHandler(GameSystem &game_system);
    void run();
};