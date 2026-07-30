#include "Player.hpp"

const int BRONZE_MAX_RP = 1400;
const int SILVER_MAX_RP = 1750;
const int GOLDEN_MAX_RP = 2250;
const int RANKED_INITIAL_HEALTH = 3;
const int RANKED_INITIAL_BULLETS = 3;

Player::Player(string u, string p, int x, int r)
{
    username = u;
    password = p;
    xp = x;
    rp = r;
    casual_ready = false;
    in_match = false;
    total_wins = 0;
    total_losses = 0;
    health_penalty_amount = 0;
    health_penalty_matches = 0;
    bullet_penalty_amount = 0;
    bullet_penalty_matches = 0;
}

string Player::get_username() const
{
    return username;
}

bool Player::check_password(const string &password) const
{
    return this->password == password;
}

int Player::get_xp() const
{
    return xp;
}

int Player::get_rp() const
{
    return rp;
}

string Player::get_level() const
{
    if (rp < BRONZE_MAX_RP)
    {
        return "Bronze";
    }

    if (rp < SILVER_MAX_RP)
    {
        return "Silver";
    }

    if (rp < GOLDEN_MAX_RP)
    {
        return "Golden";
    }

    return "Platinum";
}

bool Player::is_casual_ready() const
{
    return casual_ready;
}

void Player::set_casual_ready(bool status)
{
    casual_ready = status;
}

bool Player::is_in_match() const
{
    return in_match;
}

void Player::set_in_match(bool status)
{
    in_match = status;
}

void Player::add_xp(int amount)
{
    xp += amount;
}

void Player::reduce_xp(int amount)
{
    xp -= amount;
}

int Player::get_total_wins() const
{
    return total_wins;
}

int Player::get_total_losses() const
{
    return total_losses;
}

void Player::add_win()
{
    total_wins++;
}

void Player::add_loss()
{
    total_losses++;
}

void Player::block_user(const string &username)
{
    for (const string &blocked_username : blocked_users)
    {
        if (blocked_username == username)
        {
            return;
        }
    }

    blocked_users.push_back(username);
}

void Player::unblock_user(const string &username)
{
    for (auto it = blocked_users.begin(); it != blocked_users.end(); it++)
    {
        if (*it == username)
        {
            blocked_users.erase(it);
            return;
        }
    }
}

bool Player::has_blocked(const string &username) const
{
    for (const string &blocked_username : blocked_users)
    {
        if (blocked_username == username)
        {
            return true;
        }
    }

    return false;
}

void Player::add_rp(int amount)
{
    rp += amount;
}

void Player::reduce_rp(int amount)
{
    rp -= amount;
}

void Player::set_health_penalty(int amount, int number_of_matches)
{
    health_penalty_amount = amount;
    health_penalty_matches = number_of_matches;
}

void Player::set_bullet_penalty(int amount, int number_of_matches)
{
    bullet_penalty_amount = amount;
    bullet_penalty_matches = number_of_matches;
}

int Player::get_initial_ranked_health() const
{
    if (health_penalty_matches > 0)
    {
        return RANKED_INITIAL_HEALTH - health_penalty_amount;
    }

    return RANKED_INITIAL_HEALTH;
}

int Player::get_initial_ranked_bullets() const
{
    if (bullet_penalty_matches > 0)
    {
        return RANKED_INITIAL_BULLETS - bullet_penalty_amount;
    }

    return RANKED_INITIAL_BULLETS;
}

void Player::decrease_penalties_after_ranked_match()
{
    if (health_penalty_matches > 0)
    {
        health_penalty_matches--;

        if (health_penalty_matches == 0)
        {
            health_penalty_amount = 0;
        }
    }

    if (bullet_penalty_matches > 0)
    {
        bullet_penalty_matches--;

        if (bullet_penalty_matches == 0)
        {
            bullet_penalty_amount = 0;
        }
    }
}