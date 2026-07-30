#include "RankedMatch.hpp"

RankedMatch::RankedMatch(string p1, string p2, int p1_initial_bullets, int p2_initial_bullets, int p1_initial_health, int p2_initial_health)
    : Match(p1, p2, "ranked", p1_initial_bullets, p2_initial_bullets, p1_initial_health, p2_initial_health)
{
}

void RankedMatch::apply_health_changes()
{
    string player1 = get_player1_username();
    string player2 = get_player2_username();

    string player1_action = get_action(player1);
    string player2_action = get_action(player2);

    if (player1_action == "shoot" && player2_action == "reload")
    {
        decrease_health(player2);
    }

    if (player2_action == "shoot" && player1_action == "reload")
    {
        decrease_health(player1);
    }
}

void RankedMatch::apply_bullet_changes()
{
    string player1 = get_player1_username();
    string player2 = get_player2_username();

    string player1_action = get_action(player1);
    string player2_action = get_action(player2);

    if (player1_action == "shoot")
    {
        decrease_bullet(player1);
    }
    else if (player1_action == "reload")
    {
        increase_bullet(player1);
    }

    if (player2_action == "shoot")
    {
        decrease_bullet(player2);
    }
    else if (player2_action == "reload")
    {
        increase_bullet(player2);
    }
}

string RankedMatch::finish_turn()
{
    string player1 = get_player1_username();
    string player2 = get_player2_username();

    apply_health_changes();
    apply_bullet_changes();

    string winner = "";

    if (get_health(player1) <= 0)
    {
        winner = player2;
    }
    else if (get_health(player2) <= 0)
    {
        winner = player1;
    }

    if (winner != "")
    {
        deactivate();
        return winner;
    }

    save_turn_to_history();
    reset_actions();
    go_to_next_turn();

    return "";
}