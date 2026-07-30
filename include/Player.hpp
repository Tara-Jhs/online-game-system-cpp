#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Player
{
private:
    string username;
    string password;
    int xp;
    int rp;
    bool casual_ready;
    bool in_match;
    int total_wins;
    int total_losses;
    vector<string> blocked_users;
    int health_penalty_amount;
    int health_penalty_matches;
    int bullet_penalty_amount;
    int bullet_penalty_matches;

public:
    Player(string u, string p, int x, int r);

    string get_username() const;
    bool check_password(const string &password) const;

    int get_xp() const;
    int get_rp() const;
    string get_level() const;

    bool is_casual_ready() const;
    void set_casual_ready(bool status);

    bool is_in_match() const;
    void set_in_match(bool status);

    void add_xp(int amount);
    void reduce_xp(int amount);

    void add_rp(int amount);
    void reduce_rp(int amount);

    int get_total_wins() const;
    int get_total_losses() const;
    void add_win();
    void add_loss();

    void block_user(const string &username);
    void unblock_user(const string &username);
    bool has_blocked(const string &username) const;

    void set_health_penalty(int amount, int number_of_matches);
    void set_bullet_penalty(int amount, int number_of_matches);

    int get_initial_ranked_health() const;
    int get_initial_ranked_bullets() const;

    void decrease_penalties_after_ranked_match();
};
