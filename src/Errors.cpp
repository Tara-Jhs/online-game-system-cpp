#include "Errors.hpp"

RuntimeError::~RuntimeError() = default;

string BadRequest::get_message() const
{
    return "Bad Request";
}

string NotFound::get_message() const
{
    return "Not Found";
}

string PermissionDenied::get_message() const
{
    return "Permission Denied";
}

string Empty::get_message() const
{
    return "Empty";
}

string MismatchLevel::get_message() const
{
    return "Mismatch Level";
}