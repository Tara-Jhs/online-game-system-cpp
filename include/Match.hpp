#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Match
{
private:
    string player1;
    string player2;
    string match_type;

    int player1_bullets;
    int player2_bullets;

    int player1_health;
    int player2_health;

    string player1_action;
    string player2_action;

    vector<string> player1_history;
    vector<string> player2_history;

    int turn;
    bool active;

public:
    Match(string p1, string p2, string type, int p1_bullets, int p2_bullets, int p1_health, int p2_health);

    virtual ~Match() = default;

    bool has_player(const string &username) const;
    bool is_active() const;
    string get_match_type() const;

    string get_player1_username() const;
    string get_player2_username() const;
    string get_opponent(const string &username) const;

    bool has_played_this_turn(const string &username) const;
    int get_bullets(const string &username) const;
    int get_health(const string &username) const;

    void set_action(const string &username, const string &action);
    bool both_players_played() const;
    string get_action(const string &username) const;

    int get_turn() const;
    int get_history_size() const;
    string get_history_action(const string &username, int index) const;

    void increase_bullet(const string &username);
    void decrease_bullet(const string &username);
    void decrease_health(const string &username);

    void save_turn_to_history();
    void reset_actions();
    void go_to_next_turn();
    void deactivate();

    virtual string finish_turn() = 0;
};