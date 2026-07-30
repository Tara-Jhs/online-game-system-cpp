# Online Game Management System

A command-line online game management system developed individually in C++ as a university Advanced Programming project.

## Overview

This project simulates an online gaming platform in which players can register, log in, send invitations, participate in casual or ranked matches, and view different reports.

Administrators can also log in and access monitoring and reporting features.

## Features

- Player registration and login
- Administrator login
- Loading player and administrator information from CSV files
- Casual and ranked matches
- Sending and accepting game invitations
- Experience Point (XP) management
- Ranked Point (RP) management
- Player level classification
- Searching and sorting ranked opponents
- Blocking and unblocking players
- Match and player reports
- Input validation and error handling
- Command processing using `GET`, `POST`, `PUT`, and `DELETE`

## Match Types

### Casual Match

Players can participate in casual matches and receive experience points based on the result of each match.

### Ranked Match

Players can participate in ranked matches against players at the same level. Ranked matches affect the ranking points of the players.

The ranking levels include:

- Bronze
- Silver
- Gold
- Platinum

## Input Files

The program receives the paths of two CSV files as command-line arguments:

1. Players file
2. Administrators file

Example players file:

```csv
username,password,XP,RP
player1,password1,1500,1200
player2,password2,900,1600
```

Example administrators file:

```csv
username,password
admin1,password1
admin2,password2
```

## Build

The project can be compiled using the provided Makefile:

```bash
make
```

## Run

```bash
./UTGame <path-to-players-file> <path-to-admins-file>
```

Example:

```bash
./UTGame players.csv admins.csv
```

## Project Structure

- `main.cpp` — program entry point
- `Player.cpp` / `Player.hpp` — player information and operations
- `Admin.cpp` / `Admin.hpp` — administrator information and permissions
- `Match.cpp` / `Match.hpp` — base match functionality
- `CasualMatch.cpp` / `CasualMatch.hpp` — casual match implementation
- `RankedMatch.cpp` / `RankedMatch.hpp` — ranked match implementation
- `Invitation.cpp` / `Invitation.hpp` — invitation management
- `Report.cpp` / `Report.hpp` — reporting functionality
- `GameSystem.cpp` / `GameSystem.hpp` — main system management
- `CommandHandler.cpp` / `CommandHandler.hpp` — command parsing and execution
- `Errors.cpp` / `Errors.hpp` — error handling
- `Makefile` — build configuration

## Programming Concepts

- C++ programming
- Object-oriented programming
- Classes and inheritance
- Encapsulation and polymorphism
- Modular software design
- File handling
- CSV processing
- Command-line interfaces
- Input parsing
- Error handling
- Multi-file project organization

## Technologies

- C++
- Makefile
- Visual Studio Code
- Linux command line

## Author

**Tara Jahanshahi**

Computer Engineering Student  
University of Tehran
