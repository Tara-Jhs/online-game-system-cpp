#include "GameSystem.hpp"
#include "Player.hpp"
#include "Admin.hpp"
#include "Errors.hpp"
#include "Match.hpp"
#include "CasualMatch.hpp"
#include "RankedMatch.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

const int INITIAL_XP = 500;
const int INITIAL_RP = 1200;
const int HEALTH_BONUS_MULTIPLIER = 25;

const string ROLE_PLAYER = "player";
const string ROLE_ADMIN = "admin";

const string MATCH_CASUAL = "casual";
const string MATCH_RANKED = "ranked";

string remove_carriage_return(string s)
{
    if (!s.empty() && s.back() == '\r')
    {
        s.pop_back();
    }
    return s;
}

GameSystem::GameSystem()
{
    current_username = "";
    current_role = "";
    next_invitation_id = 1;
    next_report_id = 1;
}

void GameSystem::load_player(const string &file_path)
{
    ifstream file(file_path);
    string line, username, password, xp, rp;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty() || line == "\r")
            continue;

        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, xp, ',');
        getline(ss, rp, ',');

        username = remove_carriage_return(username);
        password = remove_carriage_return(password);
        xp = remove_carriage_return(xp);
        rp = remove_carriage_return(rp);

        players.push_back(Player(username, password, stoi(xp), stoi(rp)));
    }
    file.close();
}

void GameSystem::load_admin(const string &file_path)
{
    ifstream file(file_path);
    string line, username, password;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty() || line == "\r")
            continue;

        stringstream ss(line);
        getline(ss, username, ',');
        getline(ss, password, ',');

        username = remove_carriage_return(username);
        password = remove_carriage_return(password);

        admins.push_back(Admin(username, password));
    }
    file.close();
}

int string_to_int(const string &value)
{
    try
    {
        size_t pos;
        int result = stoi(value, &pos);

        if (pos != value.size())
        {
            throw BadRequest();
        }

        return result;
    }
    catch (...)
    {
        throw BadRequest();
    }
}

void GameSystem::require_argument(const map<string, string> &args, const string &key)
{
    if (args.count(key) == 0)
    {
        throw BadRequest();
    }
}

void GameSystem::require_player() const
{
    if (current_role != ROLE_PLAYER)
    {
        throw PermissionDenied();
    }
}

void GameSystem::require_admin() const
{
    if (current_role != ROLE_ADMIN)
    {
        throw PermissionDenied();
    }
}

bool GameSystem::username_exists(const string &username) const
{
    for (const Player &player : players)
    {
        if (player.get_username() == username)
        {
            return true;
        }
    }

    for (const Admin &admin : admins)
    {
        if (admin.get_username() == username)
        {
            return true;
        }
    }

    return false;
}

void GameSystem::handle_register(const map<string, string> &args)
{
    require_argument(args, "username");
    require_argument(args, "password");

    if (current_role != "")
    {
        throw PermissionDenied();
    }

    string username = args.at("username");
    string password = args.at("password");

    if (username_exists(username))
    {
        throw BadRequest();
    }

    players.push_back(Player(username, password, INITIAL_XP, INITIAL_RP));

    current_username = username;
    current_role = ROLE_PLAYER;

    cout << "OK" << endl;
}

void GameSystem::handle_logout(const map<string, string> &args)
{
    (void)args;

    if (current_role == "")
    {
        throw PermissionDenied();
    }

    current_username = "";
    current_role = "";

    cout << "OK" << endl;
}

void GameSystem::handle_login(const map<string, string> &args)
{
    require_argument(args, "username");
    require_argument(args, "password");

    if (current_role != "")
    {
        throw PermissionDenied();
    }

    string username = args.at("username");
    string password = args.at("password");

    for (const Player &player : players)
    {
        if (player.get_username() == username)
        {
            if (!player.check_password(password))
            {
                throw PermissionDenied();
            }

            current_username = username;
            current_role = ROLE_PLAYER;

            cout << "OK" << endl;
            return;
        }
    }

    for (const Admin &admin : admins)
    {
        if (admin.get_username() == username)
        {
            if (!admin.check_password(password))
            {
                throw PermissionDenied();
            }

            current_username = username;
            current_role = ROLE_ADMIN;

            cout << "OK" << endl;
            return;
        }
    }

    throw NotFound();
}

Player *GameSystem::find_player_by_username(const string &username)
{
    for (Player &player : players)
    {
        if (player.get_username() == username)
        {
            return &player;
        }
    }
    return nullptr;
}

void GameSystem::handle_casual_match_ready(const map<string, string> &args)
{
    require_argument(args, "status");

    string status = args.at("status");

    if (status != "true" && status != "false")
    {
        throw BadRequest();
    }

    require_player();

    Player *player = find_player_by_username(current_username);

    if (player == nullptr)
    {
        throw NotFound();
    }

    player->set_casual_ready(status == "true");

    cout << "OK" << endl;
}

void GameSystem::handle_casual_match_opponents(const map<string, string> &args)
{
    string sort_order = "desc";

    if (args.count("sort_order") != 0)
    {
        sort_order = args.at("sort_order");
    }

    if (sort_order != "asc" && sort_order != "desc")
    {
        throw BadRequest();
    }

    require_player();

    vector<Player> opponents;

    for (const Player &player : players)
    {
        if (player.is_casual_ready() && player.get_username() != current_username)
        {
            opponents.push_back(player);
        }
    }

    if (opponents.empty())
    {
        throw Empty();
    }

    sort(opponents.begin(), opponents.end(), [sort_order](const Player &a, const Player &b)
         {
        if (a.get_xp() == b.get_xp())
        {
            return a.get_username() < b.get_username();
        }

        if (sort_order == "asc")
        {
            return a.get_xp() < b.get_xp();
        }

        return a.get_xp() > b.get_xp(); });

    int index = 1;

    for (const auto &player : opponents)
    {
        cout << index << ". "
             << player.get_username()
             << " with "
             << player.get_xp()
             << " XP" << endl;

        index++;
    }
}

void GameSystem::handle_ranked_match_opponents(const map<string, string> &args)
{
    string sort_order = "desc";

    if (args.count("sort_order") != 0)
    {
        sort_order = args.at("sort_order");
    }

    if (sort_order != "asc" && sort_order != "desc")
    {
        throw BadRequest();
    }

    require_player();

    Player *current_player = find_player_by_username(current_username);

    if (current_player == nullptr)
    {
        throw NotFound();
    }

    vector<Player> opponents;

    for (const Player &player : players)
    {
        if (player.get_username() != current_username &&
            player.get_level() == current_player->get_level())
        {
            opponents.push_back(player);
        }
    }

    if (opponents.empty())
    {
        throw Empty();
    }

    sort(opponents.begin(), opponents.end(), [sort_order](const Player &a, const Player &b)
         {
             if (a.get_rp() == b.get_rp())
             {
                 return a.get_username() < b.get_username();
             }

             if (sort_order == "asc")
             {
                 return a.get_rp() < b.get_rp();
             }

             return a.get_rp() > b.get_rp(); });

    int index = 1;

    for (const Player &player : opponents)
    {
        cout << index << ". "
             << player.get_username()
             << " with "
             << player.get_rp()
             << " RP" << endl;

        index++;
    }
}

bool GameSystem::admin_exists(const string &username) const
{
    for (const Admin &admin : admins)
    {
        if (admin.get_username() == username)
        {
            return true;
        }
    }

    return false;
}

void GameSystem::handle_invitation(const map<string, string> &args)
{
    require_argument(args, "username");
    require_argument(args, "match_type");

    string username = args.at("username");
    string match_type = args.at("match_type");

    if (match_type != MATCH_CASUAL && match_type != MATCH_RANKED)
    {
        throw BadRequest();
    }

    require_player();

    if (!username_exists(username))
    {
        throw NotFound();
    }

    if (admin_exists(username))
    {
        throw PermissionDenied();
    }

    Player *receiver = find_player_by_username(username);

    if (receiver == nullptr)
    {
        throw NotFound();
    }

    if (receiver->has_blocked(current_username))
    {
        throw NotFound();
    }

    invitations.push_back(Invitation(next_invitation_id, current_username, username, match_type));
    next_invitation_id++;

    cout << "OK" << endl;
}

void GameSystem::handle_block(const map<string, string> &args)
{
    require_argument(args, "username");
    require_argument(args, "status");

    string username = args.at("username");
    string status = args.at("status");

    if (status != "blocked" && status != "unblocked")
    {
        throw BadRequest();
    }

    require_player();

    if (!username_exists(username))
    {
        throw NotFound();
    }

    if (admin_exists(username))
    {
        throw BadRequest();
    }

    Player *current_player = find_player_by_username(current_username);

    if (current_player == nullptr)
    {
        throw NotFound();
    }

    if (status == "blocked")
    {
        current_player->block_user(username);
    }
    else
    {
        current_player->unblock_user(username);
    }

    cout << "OK" << endl;
}

Invitation *GameSystem::find_invitation_by_id(int id)
{
    for (Invitation &invitation : invitations)
    {
        if (invitation.get_id() == id && invitation.is_active())
        {
            return &invitation;
        }
    }

    return nullptr;
}

void GameSystem::handle_start_match(const map<string, string> &args)
{
    require_argument(args, "invitation_id");

    int invitation_id = string_to_int(args.at("invitation_id"));

    require_player();

    Invitation *invitation = find_invitation_by_id(invitation_id);

    if (invitation == nullptr)
    {
        throw NotFound();
    }

    if (invitation->get_to_username() != current_username)
    {
        throw PermissionDenied();
    }

    Player *receiver = find_player_by_username(invitation->get_to_username());
    Player *sender = find_player_by_username(invitation->get_from_username());

    if (receiver == nullptr || sender == nullptr)
    {
        throw NotFound();
    }

    if (receiver->is_in_match() || sender->is_in_match())
    {
        throw PermissionDenied();
    }

    if (invitation->get_match_type() == MATCH_RANKED &&
        sender->get_level() != receiver->get_level())
    {
        invitation->deactivate();
        throw MismatchLevel();
    }

    receiver->set_in_match(true);
    sender->set_in_match(true);

    invitation->deactivate();

    if (invitation->get_match_type() == MATCH_CASUAL)
    {
        matches.push_back(make_unique<CasualMatch>(
            sender->get_username(),
            receiver->get_username()));
    }
    else
    {
        int sender_initial_bullets = sender->get_initial_ranked_bullets();
        int receiver_initial_bullets = receiver->get_initial_ranked_bullets();

        int sender_initial_health = sender->get_initial_ranked_health();
        int receiver_initial_health = receiver->get_initial_ranked_health();

        matches.push_back(make_unique<RankedMatch>(
            sender->get_username(),
            receiver->get_username(),
            sender_initial_bullets,
            receiver_initial_bullets,
            sender_initial_health,
            receiver_initial_health));

        sender->decrease_penalties_after_ranked_match();
        receiver->decrease_penalties_after_ranked_match();
    }

    cout << "OK" << endl;
}

void GameSystem::handle_reject_invitation(const map<string, string> &args)
{
    require_argument(args, "invitation_id");

    int invitation_id = string_to_int(args.at("invitation_id"));

    require_player();

    Invitation *invitation = find_invitation_by_id(invitation_id);

    if (invitation == nullptr)
    {
        throw NotFound();
    }

    if (invitation->get_to_username() != current_username)
    {
        throw PermissionDenied();
    }

    invitation->deactivate();

    cout << "OK" << endl;
}

Match *GameSystem::find_active_match_by_username(const string &username)
{
    for (auto &match : matches)
    {
        if (match->is_active() && match->has_player(username))
        {
            return match.get();
        }
    }

    return nullptr;
}

void GameSystem::handle_action(const map<string, string> &args)
{
    require_argument(args, "action");

    string action = args.at("action");

    if (action != "shoot" && action != "defend" && action != "reload")
    {
        throw BadRequest();
    }

    require_player();

    Match *match = find_active_match_by_username(current_username);

    if (match == nullptr)
    {
        throw NotFound();
    }

    if (match->has_played_this_turn(current_username))
    {
        throw PermissionDenied();
    }

    if (action == "shoot" && match->get_bullets(current_username) == 0)
    {
        throw BadRequest();
    }

    match->set_action(current_username, action);

    if (match->both_players_played())
    {
        string winner = match->finish_turn();

        if (winner != "")
        {
            string loser = match->get_opponent(winner);

            Player *winner_player = find_player_by_username(winner);
            Player *loser_player = find_player_by_username(loser);

            if (winner_player != nullptr && loser_player != nullptr)
            {
                if (match->get_match_type() == MATCH_CASUAL)
                {
                    update_xp_after_match(winner_player, loser_player);
                }
                else if (match->get_match_type() == MATCH_RANKED)
                {
                    int remaining_health = match->get_health(winner);
                    update_rp_after_match(winner_player, loser_player, remaining_health);
                }

                winner_player->set_in_match(false);
                loser_player->set_in_match(false);
            }
        }
    }

    cout << "OK" << endl;
}

void GameSystem::handle_match_status(const map<string, string> &args)
{
    (void)args;

    require_player();

    Match *match = find_active_match_by_username(current_username);

    if (match == nullptr)
    {
        throw NotFound();
    }

    string opponent = match->get_opponent(current_username);

    string your_action = match->get_action(current_username);
    string opponent_action = match->get_action(opponent);

    cout << "Turn " << match->get_turn() << endl;
    cout << "You: " << your_action << endl;

    if (opponent_action == "pending")
    {
        cout << "Your opponent: pending" << endl;
    }
    else
    {
        cout << "Your opponent: played" << endl;
    }

    cout << "History:" << endl;

    cout << left << setw(20) << "Opponent's moves:" << "Your moves:" << endl;

    for (int i = 0; i < match->get_history_size(); i++)
    {
        cout << left << setw(20)
             << match->get_history_action(opponent, i)
             << match->get_history_action(current_username, i)
             << endl;
    }

    cout << "Your remaining bullets: "
         << match->get_bullets(current_username)
         << endl;
    if (match->get_match_type() == MATCH_RANKED)
    {
        cout << "Your remaining health: "
             << match->get_health(current_username)
             << endl;
    }
}

void GameSystem::update_xp_after_match(Player *winner, Player *loser)
{
    int winner_xp = winner->get_xp();
    int loser_xp = loser->get_xp();

    double delta = 50 - 0.1 * (winner_xp - loser_xp);
    int delta_xp = static_cast<int>(delta);
    if (delta_xp < 5)
    {
        delta_xp = 5;
    }

    winner->add_xp(delta_xp);
    loser->reduce_xp(delta_xp);
    winner->add_win();
    loser->add_loss();
}

void GameSystem::update_rp_after_match(Player *winner, Player *loser, int remaining_health)
{
    int delta_rp = 0;
    string level = winner->get_level();

    if (level == "Bronze")
    {
        delta_rp = 75;
    }
    else if (level == "Silver")
    {
        delta_rp = 100;
    }
    else if (level == "Golden")
    {
        delta_rp = 125;
    }
    else if (level == "Platinum")
    {
        delta_rp = 150;
    }

    int health_bonus = remaining_health * HEALTH_BONUS_MULTIPLIER;

    winner->add_rp(delta_rp + health_bonus);
    loser->reduce_rp(delta_rp);

    winner->add_win();
    loser->add_loss();
}

void GameSystem::handle_report(const map<string, string> &args)
{
    require_argument(args, "username");
    require_argument(args, "reason");

    string username = args.at("username");
    string reason = args.at("reason");

    if (reason == "")
    {
        throw BadRequest();
    }

    require_player();

    if (!username_exists(username))
    {
        throw NotFound();
    }

    if (admin_exists(username))
    {
        throw PermissionDenied();
    }

    reports.push_back(Report(next_report_id, current_username, username, reason));
    next_report_id++;

    cout << "OK" << endl;
}

void GameSystem::handle_get_reports(const map<string, string> &args)
{
    (void)args;

    require_admin();

    if (reports.empty())
    {
        throw Empty();
    }

    for (const Report &report : reports)
    {
        cout << report.get_id()
             << ": \""
             << report.get_sender_username()
             << "\" reported \""
             << report.get_reported_username()
             << "\" for: \""
             << report.get_reason()
             << "\""
             << endl;
    }
}

void GameSystem::handle_profile(const map<string, string> &args)
{
    if (current_role == "")
    {
        throw PermissionDenied();
    }

    string username;

    if (args.count("username") == 0)
    {
        require_player();

        username = current_username;
    }
    else
    {
        username = args.at("username");
    }

    if (!username_exists(username))
    {
        throw NotFound();
    }

    if (admin_exists(username))
    {
        throw PermissionDenied();
    }

    Player *player = find_player_by_username(username);

    if (player == nullptr)
    {
        throw NotFound();
    }

    cout << "username: \"" << player->get_username() << "\"" << endl;
    cout << "Level: " << player->get_level() << endl;
    cout << "RP: " << player->get_rp() << endl;
    cout << "XP: " << player->get_xp() << endl;
    cout << "Total wins: " << player->get_total_wins() << endl;
    cout << "Total losses: " << player->get_total_losses() << endl;
}

void GameSystem::handle_received_invitations(const map<string, string> &args)
{
    (void)args;

    require_player();

    bool found = false;

    for (const Invitation &invitation : invitations)
    {
        if (invitation.is_active() &&
            invitation.get_to_username() == current_username)
        {
            cout << invitation.get_id()
                 << ": Invitation from \""
                 << invitation.get_from_username()
                 << "\" for a \""
                 << invitation.get_match_type()
                 << "\" match"
                 << endl;

            found = true;
        }
    }

    if (!found)
    {
        throw Empty();
    }
}

void GameSystem::handle_penalty(const map<string, string> &args)
{
    require_argument(args, "report_id");
    require_argument(args, "type");
    require_argument(args, "amount");
    require_argument(args, "number_of_matches");

    int report_id = string_to_int(args.at("report_id"));
    int amount = string_to_int(args.at("amount"));
    int number_of_matches = string_to_int(args.at("number_of_matches"));

    string type = args.at("type");

    if (type != "health_penalty" && type != "bullet_penalty")
    {
        throw BadRequest();
    }

    if (type == "health_penalty" && (amount < 1 || amount > 2))
    {
        throw BadRequest();
    }

    if (type == "bullet_penalty" && (amount < 1 || amount > 3))
    {
        throw BadRequest();
    }

    if (number_of_matches < 1)
    {
        throw BadRequest();
    }

    require_admin();

    for (auto it = reports.begin(); it != reports.end(); it++)
    {
        if (it->get_id() == report_id)
        {
            Player *reported_player = find_player_by_username(it->get_reported_username());

            if (reported_player == nullptr)
            {
                throw NotFound();
            }

            if (type == "health_penalty")
            {
                reported_player->set_health_penalty(amount, number_of_matches);
            }
            else
            {
                reported_player->set_bullet_penalty(amount, number_of_matches);
            }

            reports.erase(it);

            cout << "OK" << endl;
            return;
        }
    }

    throw NotFound();
}

void GameSystem::handle_dismiss_report(const map<string, string> &args)
{
    require_argument(args, "report_id");

    int report_id = string_to_int(args.at("report_id"));

    require_admin();

    for (auto it = reports.begin(); it != reports.end(); it++)
    {
        if (it->get_id() == report_id)
        {
            reports.erase(it);
            cout << "OK" << endl;
            return;
        }
    }

    throw NotFound();
}