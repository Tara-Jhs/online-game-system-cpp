#include "CasualMatch.hpp"

CasualMatch::CasualMatch(string p1, string p2)
    : Match(p1, p2, "casual", 1, 1, 0, 0)
{
}

string CasualMatch::finish_turn()
{
    string player1 = get_player1_username();
    string player2 = get_player2_username();

    string player1_action = get_action(player1);
    string player2_action = get_action(player2);

    string winner = "";

    if (player1_action == "shoot" && player2_action == "reload")
    {
        winner = player1;
    }
    else if (player2_action == "shoot" && player1_action == "reload")
    {
        winner = player2;
    }

    if (winner != "")
    {
        deactivate();
        return winner;
    }

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

    save_turn_to_history();
    reset_actions();
    go_to_next_turn();

    return "";
}