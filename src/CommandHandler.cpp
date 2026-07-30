#include "CommandHandler.hpp"
#include "GameSystem.hpp"

CommandHandler::CommandHandler(GameSystem &game_system) : game_system(game_system)
{
}

bool CommandHandler::is_valid_method(const string &method) const
{
    return method == GET || method == POST || method == DELETE || method == PUT;
}

bool CommandHandler::is_valid_command(const string &method, const string &command) const
{
    if (method == POST)
    {
        return command == "register" || command == "login" || command == "logout" || command == "casual_match_ready" || command == "invitation" || command == "start_match" || command == "reject_invitation" || command == "action" || command == "report" || command == "block" || command == "penalty" || command == "dismiss_report";
    }

    if (method == GET)
    {
        return command == "casual_match_opponents" || command == "match_status" || command == "reports" || command == "profile" || command == "received_invitations" || command == "ranked_match_opponents";
    }

    return false;
}

map<string, string> CommandHandler::parse_arguments(stringstream &ss) const
{
    map<string, string> args;

    string question_mark;
    if (!(ss >> question_mark) || question_mark != "?")
    {
        throw BadRequest();
    }

    string key;
    while (ss >> key)
    {
        ss >> ws;

        if (ss.peek() != '"')
        {
            throw BadRequest();
        }

        ss.get();

        string value;
        char ch;

        bool closed = false;

        while (ss.get(ch))
        {
            if (ch == '"')
            {
                closed = true;
                break;
            }

            value += ch;
        }

        if (!closed)
        {
            throw BadRequest();
        }

        args[key] = value;
    }

    return args;
}

void CommandHandler::execute_handler(const map<string, Handler> &handlers, const string &command, const map<string, string> &args)
{
    auto it = handlers.find(command);

    if (it == handlers.end())
    {
        throw BadRequest();
    }

    Handler handler = it->second;
    (game_system.*handler)(args);
}

void CommandHandler::execute_command(const string &method,const string &command, const map<string, string> &args)
{
    const map<string, Handler> post_handlers = {
        {"register", &GameSystem::handle_register},
        {"login", &GameSystem::handle_login},
        {"logout", &GameSystem::handle_logout},
        {"casual_match_ready", &GameSystem::handle_casual_match_ready},
        {"invitation", &GameSystem::handle_invitation},
        {"start_match", &GameSystem::handle_start_match},
        {"reject_invitation", &GameSystem::handle_reject_invitation},
        {"action", &GameSystem::handle_action},
        {"report", &GameSystem::handle_report},
        {"block", &GameSystem::handle_block},
        {"penalty", &GameSystem::handle_penalty},
        {"dismiss_report", &GameSystem::handle_dismiss_report}
    };

    const map<string, Handler> get_handlers = {
        {"casual_match_opponents", &GameSystem::handle_casual_match_opponents},
        {"ranked_match_opponents", &GameSystem::handle_ranked_match_opponents},
        {"match_status", &GameSystem::handle_match_status},
        {"reports", &GameSystem::handle_get_reports},
        {"profile", &GameSystem::handle_profile},
        {"received_invitations", &GameSystem::handle_received_invitations}
    };

    if (method == "POST")
    {
        execute_handler(post_handlers, command, args);
    }
    else if (method == "GET")
    {
        execute_handler(get_handlers, command, args);
    }
    else
    {
        throw BadRequest();
    }
}

void CommandHandler::run()
{
    string line;

    while (getline(cin, line))
    {
        if (line.empty())
        {
            continue;
        }

        try
        {
            stringstream ss(line);
            string method, command;

            ss >> method;

            if (!is_valid_method(method))
            {
                throw BadRequest();
            }

            if (!(ss >> command))
            {
                throw BadRequest();
            }

            if (!is_valid_command(method, command))
            {
                throw NotFound();
            }

            map<string, string> args = parse_arguments(ss);

            execute_command(method, command, args);
        }
        catch (const RuntimeError &error)
        {
            cout << error.get_message() << endl;
        }
    }
}