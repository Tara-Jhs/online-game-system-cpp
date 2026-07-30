#pragma once

#include <string>

using namespace std;

class RuntimeError
{
public:
    virtual string get_message() const = 0;
    virtual ~RuntimeError();
};

class BadRequest : public RuntimeError
{
public:
    string get_message() const override;
};

class NotFound : public RuntimeError
{
public:
    string get_message() const override;
};

class PermissionDenied : public RuntimeError
{
public:
    string get_message() const override;
};

class Empty : public RuntimeError
{
public:
    string get_message() const override;
};

class MismatchLevel : public RuntimeError
{
public:
    string get_message() const override;
};