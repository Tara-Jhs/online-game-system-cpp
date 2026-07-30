#pragma once

#include "Match.hpp"

class CasualMatch : public Match
{
public:
    CasualMatch(string p1, string p2);
    string finish_turn() override;
};