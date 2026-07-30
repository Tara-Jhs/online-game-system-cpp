#pragma once

#include "Player.hpp"
#include "Admin.hpp"
#include "Invitation.hpp"
#include "Match.hpp"
#include "Report.hpp"

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <memory>

class GameSystem
{
private:
    vector<Player> players;
    vector<Admin> admins;
    vector<Invitation> invitations;
    vector<unique_ptr<Match>> matches;
    string current_username;
    vector<Report> reports;
    string current_role;
    int next_invitation_id;
    int next_report_id;
    void require_argument(const map<string, string> &args, const string &key);
    void require_player() const;
    void require_admin() const;

    bool username_exists(const string &username) const;
    Player *find_player_by_username(const string &username);
    bool admin_exists(const string &username) const;
    Invitation *find_invitation_by_id(int id);
    Match *find_active_match_by_username(const string &username);

    void update_xp_after_match(Player *winner, Player *loser);
    void update_rp_after_match(Player *winner, Player *loser, int remaining_health);

public:
    GameSystem();
    void load_player(const string &file_path);
    void load_admin(const string &file_path);

    void handle_register(const map<string, string> &args);
    void handle_logout(const map<string, string> &args);
    void handle_login(const map<string, string> &args);
    void handle_casual_match_ready(const map<string, string> &args);
    void handle_casual_match_opponents(const map<string, string> &args);
    void handle_ranked_match_opponents(const map<string, string> &args);
    void handle_invitation(const map<string, string> &args);
    void handle_block(const map<string, string> &args);
    void handle_start_match(const map<string, string> &args);
    void handle_reject_invitation(const map<string, string> &args);
    void handle_action(const map<string, string> &args);
    void handle_match_status(const map<string, string> &args);
    void handle_report(const map<string, string> &args);
    void handle_get_reports(const map<string, string> &args);
    void handle_profile(const map<string, string> &args);
    void handle_received_invitations(const map<string, string> &args);
    void handle_penalty(const map<string, string> &args);
    void handle_dismiss_report(const map<string, string> &args);
};
