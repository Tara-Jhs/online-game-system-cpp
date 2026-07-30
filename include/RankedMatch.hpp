#pragma once

#include "Match.hpp"

class RankedMatch : public Match
{
private:
    void apply_bullet_changes();
    void apply_health_changes();

public:
    RankedMatch(string p1, string p2, int p1_initial_bullets, int p2_initial_bullets, int p1_initial_health, int p2_initial_health);
    string finish_turn() override;
};