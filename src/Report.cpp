#include "Report.hpp"

Report::Report(int id, string sender, string reported, string reason)
{
    this->id = id;
    sender_username = sender;
    reported_username = reported;
    this->reason = reason;
}

int Report::get_id() const
{
    return id;
}

string Report::get_sender_username() const
{
    return sender_username;
}

string Report::get_reported_username() const
{
    return reported_username;
}

string Report::get_reason() const
{
    return reason;
}