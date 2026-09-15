# 🎮 CodeQuest

### *Turn Coding Practice Into a Game.*

CodeQuest is a **gamified coding consistency platform** designed to help students build a regular habit of practicing **DSA, programming, and coding skills**.

Instead of focusing only on how many problems a student solves, CodeQuest focuses on **consistency, motivation, progress, and engagement** by turning coding practice into a game-like experience.

---

## 💡 Problem Statement

Many students start learning DSA or programming but struggle to practice consistently.

Common problems include:

* Lack of motivation to practice every day
* Losing track of progress
* Difficulty maintaining a coding streak
* Coding practice becoming repetitive
* No immediate sense of achievement after completing practice
* Students solving problems occasionally rather than building a long-term habit

Existing coding platforms are excellent for problem solving, but they are primarily focused on **solving and evaluating coding problems**.

CodeQuest focuses on the missing layer:

> **Building the habit of practicing consistently.**

---

## 🚀 Our Solution

CodeQuest converts coding practice into a **gamified journey**.

Students can:

* Complete coding challenges
* Play coding mini-games
* Earn XP
* Maintain streaks
* Complete daily goals
* Unlock achievements
* Level up their profile
* Track their DSA progress

The goal is to make students think:

> **"I want to practice today so I can progress in my game."**

rather than:

> **"I have to solve another coding problem."**

---

# 🎮 Core Features

## 1. 🕹️ Gamified Practice

Coding practice is converted into quests and challenges.

Students can earn rewards for completing activities such as:

* DSA problems
* Coding challenges
* Debugging challenges
* Logic challenges
* Mini-games
* Daily challenges

---

## 2. 🔥 Consistency & Streaks

CodeQuest tracks how consistently a student practices.

Students can see:

* Current streak
* Longest streak
* Days practiced
* Weekly activity
* Practice calendar

This encourages students to maintain a regular learning routine.

---

## 3. ⭐ XP & Level System

Students earn **Experience Points (XP)** for completing activities.

Example:

| Activity         |      XP |
| ---------------- | ------: |
| Easy Challenge   |  +30 XP |
| Medium Challenge |  +60 XP |
| Hard Challenge   | +100 XP |
| Mini-Game        |  +50 XP |
| Daily Goal       | +100 XP |
| 7-Day Streak     | +200 XP |

Accumulated XP allows the student to **level up**.

This creates a sense of continuous progression.

---

## 4. 🎯 Goals & Quests

Students can set learning goals such as:

> 🎯 Become comfortable with Trees

or

> 🎯 Practice DSA for 7 consecutive days

The platform can break larger goals into smaller quests and activities.

---

## 5. 🏆 Achievements

Students can unlock achievements for reaching milestones.

Examples:

🏅 **First Quest**
Complete your first coding challenge.

🔥 **3-Day Streak**
Practice for three consecutive days.

⚡ **7-Day Streak**
Maintain a seven-day coding streak.

💯 **Problem Solver**
Complete 25 coding problems.

🎮 **Game Master**
Complete multiple coding mini-games.

🧠 **DSA Master**
Reach a target level across multiple DSA topics.

---

## 6. 📊 Progress Tracking

Students can monitor their improvement across different topics.

Example:

```text
Arrays        ███████████░░  75%
Strings       ████████░░░░  60%
Recursion     █████░░░░░░░  40%
Trees         ███░░░░░░░░░  25%
Graphs        ██░░░░░░░░░░  20%
```

This helps students understand **where they are improving and where they need more practice**.

---

# ⚔️ CodeQuest Mini-Games

CodeQuest is not limited to traditional coding questions.

The platform includes short interactive games designed around programming concepts.

### 🐛 Debug Rush

Find and fix a bug before the timer runs out.

### ⚡ Code Rush

Complete coding challenges within a limited amount of time.

### 🧩 Logic Arena

Solve programming and logical puzzles.

### 🗺️ DSA Quest

Progress through different DSA levels by completing challenges.

### 🎯 Daily Challenge

A new short challenge that encourages students to return every day.

---

# 🔗 Working Alongside Existing Coding Platforms

CodeQuest is **not intended to replace platforms such as LeetCode, CodeChef, or HackerRank**.

Instead, it acts as a **gamification and consistency layer** around coding practice.

```text
        LeetCode
           │
        CodeChef
           │
       HackerRank
           │
           ▼
     ┌─────────────┐
     │  CodeQuest  │
     └─────────────┘
           │
     ┌─────┼─────┐
     ▼     ▼     ▼
    XP   Streak  Goals
     │     │     │
     └─────┼─────┘
           ▼
     Achievements
           │
           ▼
      Level Up 🎮
```

The important difference is:

> **LeetCode helps students solve problems. CodeQuest helps students build the habit of solving them consistently.**

Where supported, external-platform activity can be connected using **official APIs or permitted integration methods** rather than unauthorized scraping.

---

# 🧠 Personal Progress System

CodeQuest focuses on more than simply counting solved problems.

The platform considers factors such as:

* Practice frequency
* Current streak
* Completed goals
* XP earned
* Topic-wise progress
* Completed quests
* Achievements
* Mini-game performance

This creates a more engaging representation of a student's coding journey.

---

# 🏗️ System Architecture

```text
                  ┌───────────────┐
                  │    Student    │
                  └───────┬───────┘
                          │
                          ▼
                  ┌───────────────┐
                  │   Frontend    │
                  │ HTML/CSS/JS   │
                  └───────┬───────┘
                          │
                          ▼
                  ┌───────────────┐
                  │ CodeQuest     │
                  │ Game Engine   │
                  └───────┬───────┘
                          │
             ┌────────────┼────────────┐
             ▼            ▼            ▼
          Player        Games       Progress
             │            │            │
             ▼            ▼            ▼
            XP         Rewards      Streaks
             │                         │
             └──────────┬──────────────┘
                        ▼
                  Achievements
```

---

# 💻 OOP Implementation

The backend/core logic of CodeQuest is designed using **C++ Object-Oriented Programming concepts**.

### Main OOP Concepts

* **Encapsulation**
* **Abstraction**
* **Inheritance**
* **Polymorphism**
* **Constructors**
* **Classes and Objects**
* **File Handling**

Example conceptual structure:

```text
Player
   │
   ├── name
   ├── XP
   ├── level
   └── streak


Game
  │
  ├── DebugRush
  ├── CodeRush
  ├── LogicArena
  └── DSAQuest
```

The abstract `Game` class can define common game functionality, while individual games implement their own behavior.

This allows the project to demonstrate genuine OOP concepts rather than using C++ only as a basic programming language.

---

# 🛠️ Technology Stack

### Frontend

* HTML
* CSS
* JavaScript

### Core Logic

* C++

### OOP Concepts

* Classes & Objects
* Encapsulation
* Inheritance
* Abstraction
* Polymorphism
* File Handling

### Planned Supporting Technologies

* Backend
* Database
* Authentication
* APIs
* Progress analytics

---

# 📁 Project Structure

```text
CodeQuest/
│
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
│
├── cpp/
│   ├── Player.h
│   ├── Game.h
│   ├── DebugRush.h
│   └── main.cpp
│
├── assets/
│
└── README.md
```

---

# 🎓 Academic Project

CodeQuest demonstrates how **Object-Oriented Programming can be combined with modern web technologies and gamification** to solve a real student problem.

The project focuses on:

> **Consistency → Engagement → Practice → Progress → Skill Development**

Rather than simply creating another coding-question platform, CodeQuest attempts to make regular coding practice **motivating, measurable, and game-like**.

---

# 🎮 CodeQuest Philosophy

```text
       PRACTICE
          ↓
       COMPLETE
          ↓
       EARN XP
          ↓
      MAINTAIN STREAK
          ↓
      COMPLETE QUESTS
          ↓
      UNLOCK ACHIEVEMENTS
          ↓
        LEVEL UP
          ↓
    BUILD CONSISTENCY
```

### **Learn. Play. Practice. Level Up.**

## ⚔️ CodeQuest — Make Consistency Your Superpower.
