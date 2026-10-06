# 🏆 Sports Tournament Management System

A **C-based Sports Tournament Management System** designed to manage teams, players, knockout matches, tournament progression, match history, standings, and match connections using fundamental **Data Structures and Algorithms**.

The system follows a **knockout tournament format**, where teams compete head-to-head, losers are eliminated, and winners advance to the next round until a champion is determined.

---

## 📌 Project Overview

The **Sports Tournament Management System** is a menu-driven C application that simulates a knockout sports tournament.

The system allows users to:

- Register teams and captains
- Add and manage players
- Search teams using Binary Search
- Manage players using a Linked List
- Schedule upcoming matches using a Queue
- Store completed matches using a Stack
- Generate knockout brackets automatically
- Conduct matches with score input
- Handle penalty shootouts for drawn matches
- Track team wins and elimination rounds
- Display tournament standings
- Represent match relationships using a Graph
- Display tournament match history
- Determine the tournament champion

---

## 🎯 Objectives

- To develop a practical application using **C programming**.
- To demonstrate the use of different **Data Structures**.
- To implement a **knockout tournament system**.
- To provide efficient team searching using **Binary Search**.
- To manage players using a **Linked List**.
- To use a **Queue** for upcoming matches.
- To use a **Stack** for completed match history.
- To represent match relationships using a **Graph**.
- To rank teams using **Bubble Sort**.
- To automatically determine the tournament winner.

---

## ⚙️ Features

### 👥 Team Management

- Add teams before the tournament begins.
- Each team has:
  - Team ID
  - Team Name
  - Captain Name
  - Number of Wins
  - Elimination Status
  - Round Eliminated
- Team registration is locked once the tournament starts.
- Maximum of **16 teams** are supported.

### 🧑‍🤝‍🧑 Player Management

Players are maintained using a **Singly Linked List**.

Available operations:

- Add Player
- Delete Player

Each player contains:

- Player ID
- Player Name
- Team ID

### 🔎 Team Search

Teams can be searched using **Binary Search**.

The program creates a sorted index of team names using `qsort()` and performs binary search without changing the original team array.

### 🏟️ Knockout Tournament

The tournament follows a knockout format.

Supported team counts:

```text
2
4
8
16
```

The number of teams must be a **power of 2**.

For an 8-team tournament:

```text
Quarter-Finals → Semi-Finals → Final
```

For a 16-team tournament:

```text
Round 1 → Quarter-Finals → Semi-Finals → Final
```

### ⚽ Match Management

Upcoming matches are stored using a **Queue**.

Completed matches are stored using a **Stack**.

For each match:

- Enter score for Team 1
- Enter score for Team 2
- Determine winner
- Eliminate loser
- Update winner statistics
- Store match in history
- Advance winner to next round

### 🥅 Penalty Shootout

Draws are not allowed in knockout matches.

If both teams have the same score:

```text
Scores are level! Penalty Shootout!
```

Penalty scores are entered until one team has a higher score.

Example:

```text
Germany 2-2 Portugal

Penalty Shootout:
Germany 4
Portugal 3

Germany wins on penalties.
```

### 📊 Standings

The final standings are generated using **Bubble Sort**.

Teams are ranked according to:

1. Number of knockout wins
2. Round reached when wins are equal

### 🕸️ Match Graph

An **Adjacency Matrix** is used to represent which teams have played against each other.

Example:

```text
Argentina → Brazil France England
Brazil    → Argentina
France    → Spain Argentina
Spain     → France
```

---

# 🧱 Data Structures and Algorithms Used

| Data Structure / Algorithm | Application |
|---|---|
| **Array** | Store teams |
| **Array Queue** | Store upcoming matches |
| **Array Stack** | Store completed matches |
| **Linked List** | Store players |
| **Graph** | Represent team match connections |
| **Adjacency Matrix** | Store graph relationships |
| **Binary Search** | Search teams by name |
| **Bubble Sort** | Generate team standings |
| **qsort()** | Sort team indices for Binary Search |

---

# 🏗️ System Architecture

```text
                 SPORTS TOURNAMENT
                         |
        +----------------+----------------+
        |                |                |
   TEAM MANAGEMENT  PLAYER MANAGEMENT  TOURNAMENT
        |                |                |
      Array          Linked List       Knockout
        |                |                |
 Binary Search    Linear Traversal     Queue
        |                |                |
        +----------------+----------------+
                         |
                    MATCH SYSTEM
                         |
             +-----------+-----------+
             |                       |
          Queue                    Stack
       Upcoming Matches       Match History
             |
          Match Result
             |
       Winner / Eliminated
             |
        Next Tournament Round
             |
           Graph
             |
      Match Relationships
```

---

# 🏆 Knockout Tournament Flow

For an 8-team tournament:

```text
                 8 TEAMS
                    |
          +---------+---------+
          |                   |
     QUARTER-FINALS      QUARTER-FINALS
          |                   |
       Winners             Winners
          |                   |
          +---------+---------+
                    |
               SEMI-FINALS
                    |
                  Winners
                    |
                  FINAL
                    |
                🏆 WINNER
```

---

# 🧪 Sample Tournament

The following sample uses eight national teams:

| ID | Team | Captain |
|---:|---|---|
| 1 | Argentina | Lionel Messi |
| 2 | Brazil | Neymar Jr |
| 3 | France | Kylian Mbappe |
| 4 | Spain | Alvaro Morata |
| 5 | Germany | Manuel Neuer |
| 6 | Portugal | Cristiano Ronaldo |
| 7 | England | Harry Kane |
| 8 | Netherlands | Virgil van Dijk |

---

## Quarter-Finals

```text
Match 1
Argentina 2 - 1 Brazil
Winner: Argentina

Match 2
France 1 - 0 Spain
Winner: France

Match 3
Germany 2 - 2 Portugal
Penalty: Germany 4 - 3 Portugal
Winner: Germany

Match 4
England 3 - 1 Netherlands
Winner: England
```

### Semi-Finals

```text
Match 5
Argentina 2 - 0 France
Winner: Argentina

Match 6
Germany 1 - 2 England
Winner: England
```

### Final

```text
Match 7
Argentina 2 - 1 England
Winner: Argentina
```

## 🏆 Champion

```text
TOURNAMENT CHAMPION : Argentina
Captain             : Lionel Messi
```

---

# 📋 Sample Knockout Bracket

```text
====================================================================
                     KNOCKOUT BRACKET
====================================================================

  --- QUARTER-FINAL (Round 1) ---

  Match 1 | Argentina   2-1 | Brazil
                         Winner: Argentina

  Match 2 | France      1-0 | Spain
                         Winner: France

  Match 3 | Germany     2-2 | Portugal
                         Winner: Germany (Pen 4-3)

  Match 4 | England     3-1 | Netherlands
                         Winner: England


  --- SEMI-FINAL (Round 2) ---

  Match 5 | Argentina   2-0 | France
                         Winner: Argentina

  Match 6 | Germany     1-2 | England
                         Winner: England


  --- FINAL (Round 3) ---

  Match 7 | Argentina   2-1 | England
                         Winner: Argentina


              *** CHAMPION : Argentina ***
====================================================================
```

---

# 📈 Sample Standings

```text
=================================================================
     STANDINGS  (Bubble Sort: Wins / Round Reached)
=================================================================

Rank   Team          Wins     Status
---------------------------------------------------------------
1      Argentina      3       CHAMPION
2      England        2       Eliminated (Rd 3)
3      Germany        1       Eliminated (Rd 2)
4      France         1       Eliminated (Rd 2)
5      Brazil         0       Eliminated (Rd 1)
6      Spain          0       Eliminated (Rd 1)
7      Portugal       0       Eliminated (Rd 1)
8      Netherlands    0       Eliminated (Rd 1)

=================================================================
```

---

# 🕸️ Sample Match Graph

```text
===== KNOCKOUT MATCH GRAPH =====

Argentina       -> Brazil  France  England
Brazil          -> Argentina

France          -> Spain  Argentina
Spain           -> France

Germany         -> Portugal  England
Portugal        -> Germany

England         -> Netherlands  Germany  Argentina
Netherlands     -> England
```

The graph represents the relationships between teams that have played against each other during the tournament.

---

# 🖥️ Menu

The application provides the following menu:

```text
=====================================
   SPORTS TOURNAMENT MANAGEMENT
=====================================

 TEAM
  1.  Add Team
  2.  View All Teams
  3.  Search Team

 PLAYER
  4.  Add Player
  5.  Delete Player
 TOURNAMENT
  6.  Start Tournament
  7.  Play Next Match
  8. Upcoming Matches
  9. Match History
  10. Knockout Bracket

 ANALYTICS
  11. Standings
  12. Match Graph
  13.  Exit
=====================================
```

---

# 🔄 Program Workflow

```text
Start Program
      |
      v
Register Teams
      |
      v
Add Players
      |
      v
Search / View Teams and Players
      |
      v
Start Tournament
      |
      v
Validate Team Count
      |
      +---- Not Power of 2 ----> Add/Remove Teams
      |
      v
Generate Knockout Bracket
      |
      v
Queue Matches
      |
      v
Play Next Match
      |
      v
Enter Scores
      |
      +---- Draw ----> Penalty Shootout
      |
      v
Determine Winner
      |
      v
Eliminate Loser
      |
      v
Store Match in Stack
      |
      v
Advance Winner
      |
      v
Next Round
      |
      v
Final
      |
      v
Declare Champion
      |
      v
Display Standings / History / Graph
```

---

# 💻 Technologies Used

- **Programming Language:** C
- **Compiler:** GCC / MinGW / Turbo C-compatible compiler
- **Concepts:** Data Structures and Algorithms
- **Memory Management:** Dynamic Memory Allocation using `malloc()` and `free()`

---

# 📁 Suggested Project Structure

```text
Sports-Tournament-Management-System/
│
├── sports_tournament.c
├── README.md

# ▶️ How to Run

### 1. Clone the repository

```bash
git clone <your-github-repository-url>
```

### 2. Open the project folder

```bash
cd Sports-Tournament-Management-System
```

### 3. Compile the C program

Using GCC:

```bash
gcc sports_tournament.c -o tournament
```

### 4. Run the program

Windows:

```bash
tournament.exe
```

Linux/macOS:

```bash
./tournament
```

---

# 📌 Requirements

- C compiler such as GCC
- Windows, Linux, or macOS
- Basic command-line/terminal environment

No external libraries are required.

---

# ⚠️ Tournament Rules

1. Minimum **2 teams** are required.
2. Maximum **16 teams** are supported.
3. Number of teams must be a power of 2.
4. Teams cannot be added after tournament start.
5. Knockout matches cannot end in a draw.
6. Penalty shootout determines the winner if scores are equal.
7. Losers are eliminated immediately.
8. Winners advance to the next round.
9. The final winner becomes the tournament champion.

---

# 👨‍💻 Team Members

This project is developed as a group project by:

| No. | Name |
|---:|---|
| 1 | Aswin A|
| 2 | Rithun S Prakash |

# 🎓 Academic Purpose

This project demonstrates the practical implementation of fundamental **Data Structures and Algorithms in C** through a real-world sports tournament scenario.

The project combines multiple concepts into a single application:

```text
Arrays
   +
Linked Lists
   +
Queue
   +
Stack
   +
Graph
   +
Binary Search
   +
Linear Traversal
   +
Bubble Sort
   ↓
Sports Tournament Management System
```

---

# 🚀 Future Enhancements

Possible future improvements include:

- Add player positions and playing formations.
- Enforce a fixed number of players per team.
- Add player search by name.
- Add player statistics such as goals and assists.
- Add match dates and venues.
- Add team logos.
- Add a graphical user interface.
- Store tournament data permanently using files/database.
- Add automatic match scheduling.
- Add detailed player and team statistics.
- Add tournament result export.
- Add support for multiple tournaments.

---

# 🏆 Conclusion

The **Sports Tournament Management System** successfully demonstrates how different data structures and algorithms can be integrated into a practical C application.

The system manages teams and players, generates knockout brackets, schedules matches using a queue, stores completed matches using a stack, searches teams using binary search, manages players through a linked list, represents match relationships using a graph, and generates standings using bubble sort.

The knockout format ensures that winners progress through the tournament while defeated teams are eliminated, ultimately producing a single tournament champion.

---

## ⭐ Project Highlights

```text
✓ Team Management
✓ Player Management
✓ Knockout Tournament
✓ Automatic Bracket Generation
✓ Queue-based Match Scheduling
✓ Stack-based Match History
✓ Linked List Player Management
✓ Binary Search Team Search
✓ Graph-based Match Connections
✓ Bubble Sort Standings
✓ Penalty Shootout Support
✓ Automatic Champion Detection
```

---

## 📜 License

This project is intended primarily for **academic and educational purposes**.