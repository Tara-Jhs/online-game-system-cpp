#include "Match.hpp"

Match::Match(string p1, string p2, string type, int p1_bullets, int p2_bullets, int p1_health, int p2_health)
{
    player1 = p1;
    player2 = p2;
    match_type = type;

    player1_bullets = p1_bullets;
    player2_bullets = p2_bullets;

    player1_health = p1_health;
    player2_health = p2_health;

    player1_action = "pending";
    player2_action = "pending";

    turn = 1;
    active = true;
}

bool Match::has_player(const string &username) const
{
    return username == player1 || username == player2;
}

bool Match::is_active() const
{
    return active;
}

string Match::get_match_type() const
{
    return match_type;
}

string Match::get_player1_username() const
{
    return player1;
}

string Match::get_player2_username() const
{
    return player2;
}

string Match::get_opponent(const string &username) const
{
    if (username == player1)
    {
        return player2;
    }

    if (username == player2)
    {
        return player1;
    }

    return "";
}

bool Match::has_played_this_turn(const string &username) const
{
    if (username == player1)
    {
        return player1_action != "pending";
    }

    if (username == player2)
    {
        return player2_action != "pending";
    }

    return false;
}

int Match::get_bullets(const string &username) const
{
    if (username == player1)
    {
        return player1_bullets;
    }

    if (username == player2)
    {
        return player2_bullets;
    }

    return 0;
}

int Match::get_health(const string &username) const
{
    if (username == player1)
    {
        return player1_health;
    }

    if (username == player2)
    {
        return player2_health;
    }

    return 0;
}

void Match::set_action(const string &username, const string &action)
{
    if (username == player1)
    {
        player1_action = action;
    }
    else if (username == player2)
    {
        player2_action = action;
    }
}

bool Match::both_players_played() const
{
    return player1_action != "pending" && player2_action != "pending";
}

string Match::get_action(const string &username) const
{
    if (username == player1)
    {
        return player1_action;
    }

    if (username == player2)
    {
        return player2_action;
    }

    return "";
}

int Match::get_turn() const
{
    return turn;
}

int Match::get_history_size() const
{
    return player1_history.size();
}

string Match::get_history_action(const string &username, int index) const
{
    if (username == player1)
    {
        return player1_history[index];
    }

    if (username == player2)
    {
        return player2_history[index];
    }

    return "";
}

void Match::increase_bullet(const string &username)
{
    if (username == player1)
    {
        player1_bullets++;
    }
    else if (username == player2)
    {
        player2_bullets++;
    }
}

void Match::decrease_bullet(const string &username)
{
    if (username == player1)
    {
        player1_bullets--;
    }
    else if (username == player2)
    {
        player2_bullets--;
    }
}

void Match::decrease_health(const string &username)
{
    if (username == player1)
    {
        player1_health--;
    }
    else if (username == player2)
    {
        player2_health--;
    }
}

void Match::save_turn_to_history()
{
    player1_history.push_back(player1_action);
    player2_history.push_back(player2_action);
}

void Match::reset_actions()
{
    player1_action = "pending";
    player2_action = "pending";
}

void Match::go_to_next_turn()
{
    turn++;
}

void Match::deactivate()
{
    active = false;
}