#include "Invitation.hpp"

Invitation::Invitation(int id, string from, string to, string type)
{
    this->id = id;
    from_username = from;
    to_username = to;
    match_type = type;
    active = true;
}

int Invitation::get_id() const
{
    return id;
}

string Invitation::get_from_username() const
{
    return from_username;
}

string Invitation::get_to_username() const
{
    return to_username;
}

string Invitation::get_match_type() const
{
    return match_type;
}

bool Invitation::is_active() const
{
    return active;
}

void Invitation::deactivate()
{
    active = false;
}