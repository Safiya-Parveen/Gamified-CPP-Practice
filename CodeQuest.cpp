#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <limits>
#include <random>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#endif

using namespace std;

// Cross-platform delay: avoids this_thread::sleep_for / thread IntelliSense errors.
static void sleepMs(int milliseconds) {
#ifdef _WIN32
    Sleep(static_cast<DWORD>(milliseconds));
#else
    usleep(static_cast<useconds_t>(milliseconds) * 1000);
#endif
}

struct Question { string prompt; vector<string> options; int answer; string explanation; };
static void pauseAnimation(int milliseconds) {
    const clock_t start = clock();
    while (1000.0 * (clock() - start) / CLOCKS_PER_SEC < milliseconds) { }
}

static void gameBanner(const string& title, const string& subtitle, const string& art) {
    cout << "\033[2J\033[H";
    cout << "\033[38;5;141m  +------------------------------------------------------------+\n";
    cout << "  |                    C O D E Q U E S T                       |\n";
    cout << "  +------------------------------------------------------------+\033[0m\n";
    cout << "\033[38;5;213m" << art << "\033[0m\n";
    cout << "\033[38;5;220m  >>> " << title << " <<<\033[0m\n";
    cout << "\033[38;5;117m  " << subtitle << "\033[0m\n";
    cout << "\033[38;5;141m  ------------------------------------------------------------\033[0m\n";
    cout << "\033[2m  Play smart • Learn a concept • Collect XP • Level up\033[0m\n\n";
}

static void showIntro() {
    const string PURPLE = "\033[38;5;141m";
    const string PINK = "\033[38;5;213m";
    const string GOLD = "\033[38;5;220m";
    const string RESET = "\033[0m";
    const string DIM = "\033[2m";

    cout << "\033[2J\033[H";
    cout << PURPLE;
    cout << "  ================================================\n";
    cout << "   C C C   OOO   DDDD  EEEEE  QQQ   U   U EEEEE\n";
    cout << "  C       O   O  D   D E     Q   Q  U   U E\n";
    cout << "  C       O   O  D   D EEEE  Q Q Q  U   U EEEE\n";
    cout << "  C       O   O  D   D E     Q  QQ  U   U E\n";
    cout << "   C C C   OOO   DDDD  EEEEE  QQQQ   UUU  EEEEE\n";
    cout << "  ================================================\n";
    cout << RESET;
    cout << PINK << "             GAMIFIED C++ PRACTICE PLATFORM\n" << RESET;
    cout << DIM << "        Learn. Solve. Level up. Repeat.\n\n" << RESET;
    cout << GOLD << "        Preparing your next coding quest...\n" << RESET;
    cout << PINK << "        [" << RESET;
    for (int i = 0; i < 24; ++i) {
        cout << PURPLE << "#" << RESET << flush;
        pauseAnimation(18);
    }
    cout << PINK << "] 100%\n\n" << RESET;
    cout << GOLD << "        Welcome, adventurer! Your story starts now.\n" << RESET;
    cout << DIM << "        Tip: choose option 2 to answer quiz questions!\n\n" << RESET;
    pauseAnimation(100);
}

// CodeQuest: console-based OOP learning companion.
// Compile: g++ -std=c++17 codequest.cpp -o codequest
// Run:     ./codequest       (Windows: codequest.exe)

class Quest {
private:
    int id;
    string title, topic, difficulty, description;
    int xpReward;
    bool completed;

public:
    Quest(int id, string title, string topic, string difficulty,
          string description, int xpReward)
        : id(id), title(title), topic(topic), difficulty(difficulty),
          description(description), xpReward(xpReward), completed(false) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getTopic() const { return topic; }
    string getDifficulty() const { return difficulty; }
    string getDescription() const { return description; }
    int getXP() const { return xpReward; }
    bool isCompleted() const { return completed; }
    void markCompleted() { completed = true; }
    void setCompleted(bool value) { completed = value; }

    void display(int index) const {
        cout << index << ". [" << difficulty << "] " << title
             << "  (+" << xpReward << " XP)\n"
             << "   Topic: " << topic << "\n"
             << "   " << description << "\n"
             << "   Status: " << (completed ? "COMPLETED" : "Available") << "\n";
    }
};

class Player {
private:
    string name;
    int xp;
    int completedQuests;
    int currentStreak;
    vector<string> mistakes;
    vector<string> achievements;

public:
    Player(string playerName = "Adventurer")
        : name(playerName), xp(0), completedQuests(0), currentStreak(0) {}

    string getName() const { return name; }
    int getXP() const { return xp; }
    int getLevel() const { return xp / 100 + 1; }
    int getCompletedQuests() const { return completedQuests; }
    int getStreak() const { return currentStreak; }
    const vector<string>& getMistakes() const { return mistakes; }
    const vector<string>& getAchievements() const { return achievements; }

    void setName(const string& newName) { if (!newName.empty()) name = newName; }
    void setXP(int value) { xp = max(0, value); }
    void setCompletedQuests(int value) { completedQuests = max(0, value); }
    void setStreak(int value) { currentStreak = max(0, value); }

    void earnXP(int amount) { xp += amount; }
    void completeQuest() { ++completedQuests; }
    void updateStreak() { ++currentStreak; }

    void addMistake(const string& mistake) {
        if (!mistake.empty()) mistakes.push_back(mistake);
    }

    void addAchievement(const string& achievement) {
        if (find(achievements.begin(), achievements.end(), achievement) == achievements.end())
            achievements.push_back(achievement);
    }

    void displayProfile() const {
        cout << "\n========== PLAYER PROFILE ==========\n";
        cout << "Name             : " << name << "\n";
        cout << "Class            : Code Mage\n";
        cout << "Level            : " << getLevel() << "\n";
        cout << "Experience       : " << xp << " XP\n";
        cout << "Quests completed : " << completedQuests << "\n";
        cout << "Learning streak  : " << currentStreak << " session(s)\n";
        cout << "====================================\n";
    }
};

class CodeQuest {
private:
    Player player;
    vector<Quest> quests;
    vector<string> revisionTopics;
    const string saveFile = "codequest_save.txt";
    const string mistakeFile = "codequest_mistakes.txt";

    void seedQuests() {
        quests.emplace_back(1, "Array Expedition", "Arrays", "EASY",
            "Solve three array problems and identify a pattern.", 40);
        quests.emplace_back(2, "Complexity Cavern", "Time Complexity", "EASY",
            "Compare the time complexity of five code snippets.", 35);
        quests.emplace_back(3, "Linked List Ruins", "Linked Lists", "MEDIUM",
            "Implement insertion and deletion in a linked list.", 60);
        quests.emplace_back(4, "Stack Tower", "Stacks", "MEDIUM",
            "Solve a valid-parentheses problem using a stack.", 50);
        quests.emplace_back(5, "Binary Tree Forest", "Trees", "MEDIUM",
            "Practise preorder, inorder and postorder traversal.", 70);
        quests.emplace_back(6, "Graphfall Citadel", "Graphs", "HARD",
            "Explore BFS and DFS on a small graph.", 90);
    }

    static string trim(const string& value) {
        size_t first = value.find_first_not_of(" \t\r\n");
        if (first == string::npos) return "";
        size_t last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    void unlockAchievements() {
        if (player.getCompletedQuests() >= 1) player.addAchievement("First Steps");
        if (player.getCompletedQuests() >= 3) player.addAchievement("Quest Explorer");
        if (player.getCompletedQuests() >= 5) player.addAchievement("DSA Pathfinder");
        if (player.getXP() >= 250) player.addAchievement("XP Collector");
        if (player.getLevel() >= 5) player.addAchievement("Rising Legend");
    }

    void saveData() const {
        ofstream out(saveFile);
        if (!out) {
            cout << "Warning: could not save progress.\n";
            return;
        }
        out << quoted(player.getName()) << '\n';
        out << player.getXP() << ' ' << player.getCompletedQuests() << ' '
            << player.getStreak() << '\n';
        for (const auto& q : quests) out << q.getId() << ' ' << q.isCompleted() << '\n';
        out << "ACHIEVEMENTS " << player.getAchievements().size() << '\n';
        for (const auto& a : player.getAchievements()) out << quoted(a) << '\n';
        out << "MISTAKES " << player.getMistakes().size() << '\n';
        for (const auto& m : player.getMistakes()) out << quoted(m) << '\n';
        out << "REVISION " << revisionTopics.size() << '\n';
        for (const auto& r : revisionTopics) out << quoted(r) << '\n';
    }

    void loadData() {
        ifstream in(saveFile);
        if (!in) return;

        string loadedName;
        if (!(in >> quoted(loadedName))) return;
        player.setName(loadedName);

        int xp, completed, streak;
        if (in >> xp >> completed >> streak) {
            player.setXP(xp);
            player.setCompletedQuests(completed);
            player.setStreak(streak);
        }

        for (size_t i = 0; i < quests.size(); ++i) {
            int id, done;
            if (!(in >> id >> done)) break;
            for (auto& storedQuest : quests) {
                if (storedQuest.getId() == id) {
                    storedQuest.setCompleted(done != 0);
                    break;
                }
            }
        }

        string section;
        size_t count;
        if (in >> section >> count && section == "ACHIEVEMENTS") {
            string item;
            for (size_t i = 0; i < count && in >> quoted(item); ++i)
                player.addAchievement(item);
        }
        if (in >> section >> count && section == "MISTAKES") {
            string item;
            for (size_t i = 0; i < count && in >> quoted(item); ++i)
                player.addMistake(item);
        }
        if (in >> section >> count && section == "REVISION") {
            string item;
            for (size_t i = 0; i < count && in >> quoted(item); ++i)
                revisionTopics.push_back(item);
        }
    }

    void showQuests(bool recommendationsOnly = false) const {
        cout << "\n--------------- QUEST BOARD ---------------\n";
        cout << "These are the available quests. To answer questions, select option 2 from the main menu.\n\n";
        int shown = 0;
        for (size_t i = 0; i < quests.size(); ++i) {
            const Quest& q = quests[i];
            if (q.isCompleted()) continue;
            if (recommendationsOnly && player.getLevel() < 3 &&
                q.getDifficulty() == "HARD") continue;
            q.display(++shown);
        }
        if (shown == 0) cout << "All quests completed! You are a legend.\n";
        cout << "-------------------------------------------\n";
    }

    vector<Question> questionsFor(const string& topic) const {
        if (topic == "Arrays") return {
            {"Which structure stores elements in contiguous memory?", {"Linked list", "Array", "Tree", "Graph"}, 1, "Arrays use contiguous memory and support O(1) indexed access."},
            {"Typical complexity of accessing arr[i]?", {"O(1)", "O(log n)", "O(n)", "O(n^2)"}, 0, "Direct indexing takes constant time."},
            {"Useful technique for a pair in a sorted array?", {"DFS", "Two pointers", "Heapify", "Tree traversal"}, 1, "Two pointers move inward based on the current sum."}
        };
        if (topic == "Time Complexity") return {
            {"One loop over n elements usually takes?", {"O(1)", "O(log n)", "O(n)", "O(n^2)"}, 2, "A single pass is linear."},
            {"Binary search on sorted data takes?", {"O(n)", "O(log n)", "O(n log n)", "O(1) always"}, 1, "Each step halves the search range."},
            {"Two nested loops each running n times take?", {"O(log n)", "O(n)", "O(n^2)", "O(2n)"}, 2, "n iterations repeated n times gives n squared."}
        };
        if (topic == "Linked Lists") return {
            {"A singly linked-list node stores?", {"Only an index", "Data and next pointer", "Two child pointers", "A hash table"}, 1, "It stores data and a pointer to the next node."},
            {"Insert at the head typically takes?", {"O(1)", "O(log n)", "O(n)", "O(n^2)"}, 0, "Only a couple of pointers need updating."},
            {"Without a tail pointer, which is slow?", {"Insert at head", "Read head", "Reach last node", "Check empty"}, 2, "You must traverse the list to reach the last node."}
        };
        if (topic == "Stacks") return {
            {"A stack follows which rule?", {"FIFO", "LIFO", "Random", "Sorted"}, 1, "Last In, First Out."},
            {"Which operation removes the top item?", {"push", "peek", "pop", "enqueue"}, 2, "pop removes the top item."},
            {"Valid parentheses are commonly checked with a...?", {"Stack", "Queue only", "Graph", "Trie"}, 0, "Push opening brackets and match them on closing brackets."}
        };
        if (topic == "Trees") return {
            {"Inorder traversal of a BST gives?", {"Sorted ascending values", "Random order", "Level order", "Always descending"}, 0, "Left, root, right visits BST values in sorted order."},
            {"A binary-tree node has at most how many children?", {"1", "2", "3", "Unlimited"}, 1, "Binary means at most two children."},
            {"Which traversal visits root first?", {"Inorder", "Postorder", "Preorder", "None"}, 2, "Preorder is root, left, right."}
        };
        return {
            {"BFS commonly uses a...?", {"Stack", "Queue", "Heap", "Trie"}, 1, "A queue processes vertices in FIFO order."},
            {"DFS can use recursion or a...?", {"Queue", "Stack", "Sorted array", "Priority table"}, 1, "DFS uses the call stack or an explicit stack."},
            {"BFS finds shortest paths by edge count in...?", {"Unweighted graphs", "Every weighted graph", "Only trees", "No graph"}, 0, "BFS explores unweighted graphs layer by layer."}
        };
    }

    void completeQuest() {
        showQuests();
        cout << "\nPick a quest number to START ITS QUIZ (0 to cancel): ";
        int choice;
        if (!(cin >> choice)) { clearInput(); return; }
        if (choice <= 0) return;
        vector<int> available;
        for (size_t i = 0; i < quests.size(); ++i)
            if (!quests[i].isCompleted()) available.push_back(static_cast<int>(i));
        if (choice > static_cast<int>(available.size())) { cout << "That quest number is not on the board.\n"; return; }

        Quest& q = quests[available[choice - 1]];
        vector<Question> quiz = questionsFor(q.getTopic());
        int correct = 0;
        cout << "\n+============== QUEST QUIZ ==============+\n";
        cout << "QUEST: " << q.getTitle() << " | TOPIC: " << q.getTopic() << "\n";
        cout << "Your questions start below. Read each one and enter A, B, C or D.\n\n";
        cout << "Answer all " << quiz.size() << " questions with A, B, C or D.\n\n";
        for (size_t i = 0; i < quiz.size(); ++i) {
            cout << "Q" << i + 1 << ". " << quiz[i].prompt << "\n";
            for (size_t j = 0; j < quiz[i].options.size(); ++j)
                cout << "   " << char('A' + j) << ") " << quiz[i].options[j] << "\n";
            cout << "Your answer: "; char answer; cin >> answer; answer = static_cast<char>(toupper(answer));
            while (answer < 'A' || answer > 'D') { cout << "Enter A, B, C or D: "; cin >> answer; answer = static_cast<char>(toupper(answer)); }
            if (answer - 'A' == quiz[i].answer) {
                ++correct; cout << "\033[32mCorrect!\033[0m " << quiz[i].explanation << "\n\n";
            } else {
                player.addMistake(q.getTopic() + " - " + quiz[i].prompt + " | Answer: " + char('A' + quiz[i].answer));
                cout << "\033[33mNot quite. Correct: " << char('A' + quiz[i].answer) << ") " << quiz[i].options[quiz[i].answer] << "\033[0m\nWhy: " << quiz[i].explanation << "\n\n";
            }
        }
        cout << "\n SCORE ["; for (int i = 0; i < static_cast<int>(quiz.size()); ++i) cout << (i < correct ? '#' : '-');
        cout << "] " << correct << "/" << quiz.size() << "\n";
        if (correct * 100 >= static_cast<int>(quiz.size()) * 60) {
            q.markCompleted(); player.earnXP(q.getXP()); player.completeQuest(); player.updateStreak(); unlockAchievements();
            cout << "\033[32mQUEST CLEARED! +" << q.getXP() << " XP\033[0m\nTotal XP: " << player.getXP() << " | Level " << player.getLevel() << "\n";
        } else cout << "You need 60% to clear this quest. Review the explanations and retry — no progress lost!\n";
        saveData();
    }

    // Mini-game 1: continuous real-time Snake. Arrow keys or WASD control movement.
    // Windows uses _kbhit/_getch; Linux/macOS uses a temporary non-canonical terminal mode.
    void playSnakeQuest() {
        const int W = 30, H = 16;
        vector<pair<int,int>> snake = {{W/2, H/2}, {W/2-1, H/2}, {W/2-2, H/2}};
        int fx = 7, fy = 5, score = 0;
        // Static obstacles make later runs more strategic; bonus food appears after every 3 regular foods.
        vector<pair<int,int>> obstacles = {{8,4},{8,5},{17,7},{18,7},{23,3},{24,3}};
        int bfx = -1, bfy = -1, bonusTicks = 0;
        char direction = 'D', pendingDirection = 'D';
        bool gameOver = false, quitGame = false;
        int tickMs = 145;
        srand(static_cast<unsigned int>(time(nullptr)));

#ifndef _WIN32
        termios oldTerm{}, rawTerm{};
        bool terminalChanged = false;
        if (isatty(STDIN_FILENO)) {
            tcgetattr(STDIN_FILENO, &oldTerm);
            rawTerm = oldTerm;
            rawTerm.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
            rawTerm.c_cc[VMIN] = 0;
            rawTerm.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &rawTerm);
            terminalChanged = true;
        }
#endif
        auto restoreTerminal = [&]() {
#ifndef _WIN32
            if (terminalChanged) {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
                terminalChanged = false;
            }
#endif
        };
        auto keyAvailable = [&]() -> bool {
#ifdef _WIN32
            return _kbhit() != 0;
#else
            fd_set set;
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);
            timeval tv{0, 0};
            return select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv) > 0;
#endif
        };
        auto readKey = [&]() -> int {
#ifdef _WIN32
            int ch = _getch();
            if (ch == 0 || ch == 224) { int ext = _getch(); return 1000 + ext; }
            return ch;
#else
            unsigned char ch = 0;
            if (read(STDIN_FILENO, &ch, 1) == 1) return ch;
            return -1;
#endif
        };
        auto placeFood = [&]() {
            bool occupied = true;
            while (occupied) {
                occupied = false;
                fx = 1 + rand() % (W - 2);
                fy = 1 + rand() % (H - 2);
                for (const auto& part : snake)
                    if (part.first == fx && part.second == fy) { occupied = true; break; }
                for (const auto& wall : obstacles)
                    if (wall.first == fx && wall.second == fy) { occupied = true; break; }
                if (fx == bfx && fy == bfy) occupied = true;
            }
        };
        auto placeBonusFood = [&]() {
            bool occupied = true;
            while (occupied) {
                occupied = false;
                bfx = 1 + rand() % (W - 2);
                bfy = 1 + rand() % (H - 2);
                for (const auto& part : snake)
                    if (part.first == bfx && part.second == bfy) { occupied = true; break; }
                for (const auto& wall : obstacles)
                    if (wall.first == bfx && wall.second == bfy) { occupied = true; break; }
                if (bfx == fx && bfy == fy) occupied = true;
            }
            bonusTicks = 45;
        };
        auto render = [&]() {
            cout << "\033[2J\033[H";
            cout << "\033[38;5;213m+";
            for (int x=0; x<W; ++x) cout << "=";
            cout << "+\033[0m\n";
            cout << "\033[38;5;141m  S N A K E   Q U E S T\033[0m"
                 << "    \033[38;5;220mScore: " << score << "   XP: " << player.getXP()
                 << "   Speed: " << (tickMs <= 90 ? "INSANE" : tickMs <= 120 ? "FAST" : "NORMAL")
                 << "\033[0m\n";
            cout << "\033[2m  WASD/arrows = move | Q = quit | * food | O bonus (+20 XP) | # obstacle\033[0m\n";
            for (int y=0; y<H; ++y) {
                cout << "\033[38;5;141m#\033[0m";
                for (int x=0; x<W; ++x) {
                    char cell = ' ';
                    if (x == fx && y == fy) cell = '*';
                    if (x == bfx && y == bfy) cell = 'O';
                    for (const auto& wall : obstacles)
                        if (wall.first == x && wall.second == y) cell = '#';
                    for (size_t i=0; i<snake.size(); ++i) {
                        if (snake[i].first == x && snake[i].second == y) {
                            cell = (i == 0 ? '@' : 'o'); break;
                        }
                    }
                    if (cell == '@') cout << "\033[38;5;220m@\033[0m";
                    else if (cell == 'o') cout << "\033[38;5;213mo\033[0m";
                    else if (cell == '*') cout << "\033[38;5;46m*\033[0m";
                    else if (cell == 'O') cout << "\033[38;5;220mO\033[0m";
                    else if (cell == '#') cout << "\033[38;5;196m#\033[0m";
                    else cout << cell;
                }
                cout << "\033[38;5;141m#\033[0m\n";
            }
            cout << "\033[38;5;141m+";
            for (int x=0; x<W; ++x) cout << "=";
            cout << "+\033[0m\n";
            cout << "\033[2mEvery 4 foods = faster snake. O disappears after 45 ticks. Hit an obstacle and face a DSA rescue!\033[0m\n";
            cout.flush();
        };

        cout << "\033[2J\033[H";
        cout << "\033[38;5;213m";
        cout << "  =========================================\n";
        cout << "       S N A K E   Q U E S T  ARCADE\n";
        cout << "  =========================================\n\033[0m";
        cout << "Real-time Snake! Use WASD or arrow keys. No Enter needed.\n";
        cout << "Eat food, grow longer, collect XP, and avoid your body, walls, and obstacles.\n";
        cout << "Golden bonus food gives extra XP; a crash can be rescued with a DSA question.\n";
        cout << "Press any key to start (Q to return): ";
        cout.flush();
        while (!keyAvailable()) sleepMs(10);
        int startKey = readKey();
        if (toupper(static_cast<unsigned char>(startKey)) == 'Q') {
            restoreTerminal();
            return;
        }

        while (!gameOver && !quitGame) {
            // Consume all available input; arrow-key sequences are supported on Windows.
            while (keyAvailable()) {
                int raw = readKey();
                char move = static_cast<char>(toupper(static_cast<unsigned char>(raw)));
#ifdef _WIN32
                if (raw == 1000 + 72) move = 'W';
                if (raw == 1000 + 80) move = 'S';
                if (raw == 1000 + 75) move = 'A';
                if (raw == 1000 + 77) move = 'D';
#else
                if (raw == 27 && keyAvailable()) { // ANSI arrow key sequence
                    int b1 = readKey(), b2 = readKey();
                    if (b1 == '[') {
                        if (b2 == 'A') move = 'W';
                        if (b2 == 'B') move = 'S';
                        if (b2 == 'D') move = 'A';
                        if (b2 == 'C') move = 'D';
                    }
                }
#endif
                if (move == 'Q') { quitGame = true; break; }
                if ((move == 'W' || move == 'A' || move == 'S' || move == 'D') &&
                    !((direction == 'W' && move == 'S') || (direction == 'S' && move == 'W') ||
                      (direction == 'A' && move == 'D') || (direction == 'D' && move == 'A'))) {
                    pendingDirection = move;
                }
            }
            if (quitGame) break;
            direction = pendingDirection;
            int nx = snake.front().first, ny = snake.front().second;
            if (direction == 'W') --ny;
            if (direction == 'S') ++ny;
            if (direction == 'A') --nx;
            if (direction == 'D') ++nx;

            bool ate = (nx == fx && ny == fy);
            bool ateBonus = (nx == bfx && ny == bfy);
            bool hit = nx <= 0 || nx >= W-1 || ny <= 0 || ny >= H-1;
            for (const auto& wall : obstacles)
                if (wall.first == nx && wall.second == ny) { hit = true; break; }
            size_t bodyLimit = snake.size() - ((ate || ateBonus) ? 0 : 1);
            for (size_t i=0; i<bodyLimit; ++i)
                if (snake[i].first == nx && snake[i].second == ny) { hit = true; break; }

            if (hit) {
                render();
                restoreTerminal();
                cout << "\n\033[38;5;213mCRASH! Answer this DSA rescue question to continue!\033[0m\n";
                vector<Question> rescue = {
                    {"Which data structure follows FIFO?", {"Stack", "Queue", "Tree", "Graph"}, 1, "A queue is First In, First Out."},
                    {"Binary search on sorted data takes:", {"O(n)", "O(n^2)", "O(log n)", "O(1) always"}, 2, "Binary search halves the search range each step."},
                    {"Which operation removes the top stack item?", {"push", "pop", "enqueue", "peek"}, 1, "pop removes the top item."},
                    {"BFS commonly uses which structure?", {"Queue", "Stack only", "Hash only", "Array sort"}, 0, "A queue explores vertices level by level."}
                };
                const Question& q = rescue[rand() % rescue.size()];
                cout << q.prompt << "\n";
                for (size_t i=0; i<q.options.size(); ++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
                cout << "Answer (A-D): ";
                char a; cin >> a; a = static_cast<char>(toupper(static_cast<unsigned char>(a)));
                if (a >= 'A' && a <= 'D' && a-'A' == q.answer) {
                    player.earnXP(10); score += 1;
                    cout << "\033[32mCorrect! +10 XP. Extra life unlocked!\033[0m " << q.explanation << "\n";
                    snake = {{W/2,H/2},{W/2-1,H/2},{W/2-2,H/2}};
                    direction = pendingDirection = 'D';
                    placeFood();
                    gameOver = false;
                    cout << "Press Enter to resume the game...";
                    clearInput(); cin.get();
#ifndef _WIN32
                    if (isatty(STDIN_FILENO)) {
                        tcgetattr(STDIN_FILENO, &oldTerm);
                        rawTerm = oldTerm;
                        rawTerm.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
                        rawTerm.c_cc[VMIN] = 0; rawTerm.c_cc[VTIME] = 0;
                        tcsetattr(STDIN_FILENO, TCSANOW, &rawTerm);
                        terminalChanged = true;
                    }
#endif
                } else {
                    player.addMistake(string("Snake Quest - ") + q.prompt + " | Answer: " + char('A'+q.answer));
                    cout << "Not quite! Correct answer: " << char('A'+q.answer) << ") " << q.options[q.answer] << "\n";
                    cout << "Why: " << q.explanation << "\nGame over! Final score: " << score << "\n";
                    gameOver = true;
                }
                saveData();
                continue;
            }

            snake.insert(snake.begin(), {nx, ny});
            if (ate || ateBonus) {
                if (ate) {
                    ++score;
                    player.earnXP(5);
                    if (score % 4 == 0 && tickMs > 75) tickMs -= 12;
                    placeFood();
                    if (score % 3 == 0 && bfx < 0) placeBonusFood();
                } else {
                    score += 3;
                    player.earnXP(20);
                    bfx = bfy = -1;
                    bonusTicks = 0;
                    cout << "\a\033[38;5;220mBONUS FOOD! +20 XP\033[0m\n";
                }
            } else snake.pop_back();
            if (bonusTicks > 0 && --bonusTicks == 0) bfx = bfy = -1;

            render();
            sleepMs(tickMs);
            saveData();
        }
        restoreTerminal();
        cout << "\n\033[38;5;220mSnake Quest ended! Score: " << score
             << " | XP earned this run: " << score*5 << " (+ rescue bonuses) | Total XP: "
             << player.getXP() << "\033[0m\n";
        cout << "Press Enter to return to the arcade...";
        clearInput(); cin.get();
    }

    // Mini-game 2: Bug Hunt - move around a tiny grid, collect bugs, and answer a coding question for each catch.
    void playBugBash() {
        gameBanner("BUG HUNT | CATCH THE GLITCHES", "Move with W/A/S/D, collect the bugs, then solve a quick coding challenge.",
                   "       [ PLAYER ]  --->  *bug*  --->  [ FIX + XP ]");
        const int W = 16, H = 8;
        pair<int,int> playerPos = {1, 1};
        vector<pair<int,int>> bugs = {{4,1},{10,2},{6,5},{13,6}};
        vector<pair<int,int>> powerUps = {{7,2},{2,6},{12,1}};
        vector<pair<int,int>> hazards = {{5,3},{11,5},{14,2}};
        int caught = 0, moves = 0, roundXP = 0, lives = 3, combo = 0;
        const int maxMoves = 30;
        vector<Question> bugQuestions = {
            {"A loop visits an array of n items. Which condition avoids index n?", {"i <= n", "i < n", "i = n", "i > n"}, 1, "Valid indices are 0 through n-1, so use i < n."},
            {"Which operator compares two values in C++?", {"=", "==", "+=", "->"}, 1, "== compares values; = assigns a value."},
            {"What should you check before calling pop() on a stack?", {"Whether it is empty", "Whether it is sorted", "Its capacity is infinite", "Nothing ever"}, 0, "Checking emptiness prevents stack underflow."},
            {"Which keyword safely releases a dynamically allocated single object?", {"remove", "free only", "delete", "clear"}, 2, "In C++, memory allocated with new is released with delete."}
        };
        auto draw = [&]() {
            cout << "\033[2J\033[H";
            cout << "\033[38;5;213m  BUG BASH: GLITCH LAB\033[0m   \033[38;5;220mBugs: " << caught << "/3\033[0m   Lives: " << lives << "   Moves: " << moves << "/" << maxMoves << "   XP: +" << roundXP << "\n";
            cout << "  Legend: @ you   x bug   + XP   ! virus   # boundary\n  +" << string(W, '-') << "+\n";
            for (int y=0; y<H; ++y) {
                cout << "  |";
                for (int x=0; x<W; ++x) {
                    char cell = ' ';
                    if (playerPos.first==x && playerPos.second==y) cell='@';
                    else {
                        bool isBug=false, isPower=false, isHazard=false;
                        for (auto b: bugs) if (b.first==x && b.second==y) isBug=true;
                        for (auto u: powerUps) if (u.first==x && u.second==y) isPower=true;
                        for (auto h: hazards) if (h.first==x && h.second==y) isHazard=true;
                        if (isBug) cell='x'; else if (isPower) cell='+'; else if (isHazard) cell='!';
                    }
                    if (cell=='@') cout << "\033[38;5;213m@\033[0m";
                    else if (cell=='x') cout << "\033[38;5;196mx\033[0m";
                    else if (cell=='+') cout << "\033[38;5;220m+\033[0m";
                    else if (cell=='!') cout << "\033[38;5;196m!\033[0m";
                    else cout << cell;
                }
                cout << "|\n";
            }
            cout << "  +" << string(W, '-') << "+\n";
            cout << "Collect 3 bugs. + gives 5 XP; ! is a virus and costs one life.\n";
        };
        while (caught < 3 && moves < maxMoves && lives > 0) {
            draw();
            cout << "Move (W/A/S/D, Q to quit): ";
            char key; cin >> key; key = static_cast<char>(toupper(static_cast<unsigned char>(key)));
            if (key=='Q') break;
            int nx=playerPos.first, ny=playerPos.second;
            if (key=='W') --ny; else if (key=='S') ++ny; else if (key=='A') --nx; else if (key=='D') ++nx;
            else { cout << "Use W, A, S, D, or Q.\n"; sleepMs(650); continue; }
            if (nx<0 || nx>=W || ny<0 || ny>=H) {
                cout << "\033[38;5;196mLab wall! Stay inside the grid.\033[0m\n"; sleepMs(450); continue;
            }
            playerPos={nx,ny}; ++moves;
            auto hazardIt=find(hazards.begin(), hazards.end(), playerPos);
            if (hazardIt!=hazards.end()) {
                hazards.erase(hazardIt);
                --lives;
                combo = 0;
                cout << "\n\033[38;5;196mVIRUS HIT! -1 life. Lives remaining: " << lives << "\033[0m\n";
                if (lives > 0) {
                    playerPos = {1,1};
                    cout << "Respawning at the lab entrance...\n";
                }
                sleepMs(650);
            }
            auto bugIt=find(bugs.begin(), bugs.end(), playerPos);
            if (bugIt!=bugs.end()) {
                bugs.erase(bugIt); ++caught;
                const Question& q=bugQuestions[(caught-1)%bugQuestions.size()];
                cout << "\n\033[38;5;196mBUG CAUGHT!\033[0m Before you squash it, solve this coding challenge:\n" << q.prompt << "\n";
                for (size_t i=0;i<q.options.size();++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
                cout << "Your answer: "; char a; cin >> a; a=static_cast<char>(toupper(static_cast<unsigned char>(a)));
                if (a>='A' && a<='D' && a-'A'==q.answer) {
                    ++combo;
                    int reward = 15 + (combo >= 2 ? 5 : 0);
                    player.earnXP(reward); roundXP+=reward;
                    cout << "\033[32mBUG FIXED! +" << reward << " XP\033[0m";
                    if (combo >= 2) cout << " COMBO x" << combo << "!";
                    cout << " " << q.explanation << "\n";
                } else {
                    combo = 0;
                    player.addMistake(string("Bug Hunt - ")+q.prompt+" | Answer: "+char('A'+q.answer));
                    cout << "The bug escaped your understanding! Correct answer: " << char('A'+q.answer) << ") " << q.options[q.answer] << "\nWhy: " << q.explanation << "\n";
                }
                cout << "Press Enter to continue hunting..."; clearInput(); cin.get();
            }
            auto powerIt=find(powerUps.begin(), powerUps.end(), playerPos);
            if (powerIt!=powerUps.end()) {
                powerUps.erase(powerIt); player.earnXP(5); roundXP+=5;
                cout << "\033[38;5;220mPOWER-UP collected! +5 XP\033[0m\n"; sleepMs(550);
            }
        }
        if (caught>=3) {
            cout << "\n\033[38;5;220mLAB CLEARED! Three bugs caught and investigated.\033[0m\n";
            player.addAchievement("Bug Hunter");
            if (lives == 3) { player.earnXP(10); roundXP += 10; cout << "PERFECT RUN BONUS: +10 XP\n"; }
        } else if (lives == 0) {
            cout << "\n\033[38;5;196mLAB OVERRUN! The viruses used up all your lives. Try again!\033[0m\n";
        } else cout << "\nHunt ended. You caught " << caught << " bugs. Try again to clear the lab!\n";
        cout << "XP earned this hunt: " << roundXP << " | Total XP: " << player.getXP() << "\n";
        saveData();
    }

    // Mini-game 3: Memory Matrix - remember a sequence, then answer a DSA question.
    void playMemoryMatrix() {
        gameBanner("MEMORY MATRIX", "Train your focus, remember the pattern, then prove your DSA knowledge.",
                   "       [ 4 ]  [ 1 ]  [ 7 ]  [ 2 ]  [ 9 ]\n             MEMORY CELLS:  *  *  *  *  *");
        cout << "\033[38;5;220mMEMORY ROUND: Memorise the sequence. It will disappear shortly!\033[0m\n";
        vector<int> seq = {4, 1, 7, 2, 9};
        cout << "\033[38;5;220m";
        for (int n : seq) cout << n << "  ";
        cout << "\033[0m\n";
        pauseAnimation(1800);
        cout << "\033[2J\033[HSequence hidden! Enter the five numbers separated by spaces: ";
        vector<int> guess(5); bool memoryCorrect = true;
        for (int &n : guess) { if (!(cin >> n)) { clearInput(); cout << "Invalid input.\n"; return; } }
        for (size_t i = 0; i < seq.size(); ++i) if (guess[i] != seq[i]) memoryCorrect = false;
        if (memoryCorrect) { player.earnXP(15); cout << "\033[32mPerfect memory! +15 XP\033[0m\n"; }
        else cout << "Sequence was: 4 1 7 2 9. Keep practising!\n";
        Question q = {"Which data structure is commonly used to undo the most recent action?", {"Queue", "Stack", "Graph", "Array only"}, 1, "Undo operations follow Last In, First Out, so a stack is a natural fit."};
        cout << "\nBONUS DSA CARD: " << q.prompt << "\n";
        for (size_t i=0; i<q.options.size(); ++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
        cout << "Answer: "; char a; cin >> a; a = static_cast<char>(toupper(static_cast<unsigned char>(a)));
        if (a >= 'A' && a <= 'D' && a-'A' == q.answer) { player.earnXP(10); cout << "Correct! +10 XP. " << q.explanation << "\n"; }
        else { player.addMistake("Memory Matrix - " + q.prompt + " | Answer: B"); cout << "Correct answer: B. " << q.explanation << "\n"; }
        saveData();
    }

    // Mini-game 4: Binary Treasure - use binary-search thinking to find the treasure.
    void playBinaryTreasure() {
        gameBanner("BINARY TREASURE", "Find the hidden treasure using smart search strategy.",
                   "       [1...............16...............31]\n                       ^ TREASURE HUNT ^");
        cout << "\033[38;5;220mMISSION: Find the treasure in at most 5 guesses.\033[0m\n";
        int target = 19, attempts = 0, guess = 0;
        while (attempts < 5 && guess != target) {
            cout << "Guess " << attempts+1 << "/5: ";
            if (!(cin >> guess)) { clearInput(); cout << "Enter a number.\n"; return; }
            ++attempts;
            if (guess < target) cout << "Too low! Search the RIGHT half.\n";
            else if (guess > target) cout << "Too high! Search the LEFT half.\n";
            else { player.earnXP(20); cout << "\033[32mTREASURE FOUND! +20 XP\033[0m\n"; }
        }
        if (guess != target) cout << "The treasure was at 19. Binary search halves the search range each step.\n";
        Question q = {"What is binary search's time complexity on sorted data?", {"O(n)", "O(n^2)", "O(log n)", "O(1) always"}, 2, "Each comparison halves the remaining search space."};
        cout << "\nTREASURE KNOWLEDGE CHECK: " << q.prompt << "\n";
        for (size_t i=0; i<q.options.size(); ++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
        cout << "Answer: "; char a; cin >> a; a = static_cast<char>(toupper(static_cast<unsigned char>(a)));
        if (a >= 'A' && a <= 'D' && a-'A' == q.answer) { player.earnXP(10); cout << "Correct! +10 XP. " << q.explanation << "\n"; }
        else { player.addMistake("Binary Treasure - " + q.prompt + " | Answer: C"); cout << "Correct answer: C. " << q.explanation << "\n"; }
        saveData();
    }

    // Mini-game 5: Output Predictor - predict small C++ snippets.
    void playOutputPredictor() {
        vector<Question> rounds = {
            {"What does this print? int x=3; cout << x*2;", {"3", "6", "9", "Error"}, 1, "3 multiplied by 2 is 6."},
            {"What is the final value of sum? int sum=0; for(int i=1;i<=3;i++) sum+=i;", {"3", "5", "6", "123"}, 2, "The loop adds 1 + 2 + 3 = 6."},
            {"What does v[0] access in a vector v?", {"The last item", "The first item", "The size", "No item"}, 1, "C++ vectors use zero-based indexing."}
        };
        int score = 0;
        gameBanner("OUTPUT PREDICTOR", "Read the code carefully. Predict what the compiler will print.",
                   "       int x = 3;\n       cout << x * 2;  // ???");
        cout << "\033[38;5;220m3 ROUNDS  |  10 XP EACH  |  THINK BEFORE YOU ANSWER\033[0m\n\n";
        for (const auto& q : rounds) {
            cout << q.prompt << "\n";
            for (size_t i=0; i<q.options.size(); ++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
            cout << "Your prediction: "; char a; cin >> a; a = static_cast<char>(toupper(static_cast<unsigned char>(a)));
            if (a >= 'A' && a <= 'D' && a-'A' == q.answer) { ++score; player.earnXP(10); cout << "\033[32mCorrect! +10 XP.\033[0m " << q.explanation << "\n\n"; }
            else { player.addMistake("Output Predictor - " + q.prompt + " | Answer: " + char('A'+q.answer)); cout << "Correct: " << char('A'+q.answer) << ") " << q.options[q.answer] << ". " << q.explanation << "\n\n"; }
        }
        if (score == 3) player.addAchievement("Output Oracle");
        cout << "Round result: " << score << "/3.\n"; saveData();
    }

    // Mini-game 6: LeetCode Boss Battle - question cards inspired by common patterns.
    void playLeetCodeBoss() {
        vector<Question> bosses = {
            {"Two Sum: which approach is usually O(n) average time?", {"Two nested loops", "Hash map of seen values", "Sort after every check", "Bubble sort"}, 1, "A hash map stores values already seen and checks the complement."},
            {"Valid Parentheses is most naturally solved using a...", {"Stack", "Queue", "Heap", "Graph"}, 0, "A stack matches the most recent opening bracket first."},
            {"Number of Islands on a grid is commonly solved with...", {"BFS/DFS flood fill", "Binary search only", "Merge sort", "A stack of sorted numbers only"}, 0, "BFS or DFS visits connected land cells."},
            {"Best Time to Buy and Sell Stock: what should you track while scanning?", {"Maximum array length", "Minimum price so far", "Every pair in a matrix", "Only the final price"}, 1, "Track the lowest price so far and the best profit."}
        };
        int score=0;
        gameBanner("LEETCODE BOSS BATTLE", "Face common coding patterns and choose the strongest strategy.",
                   "          .-=========-.\n          |  B O S S  |\n          '-=========-'");
        cout << "\033[38;5;220m4 BOSS ROUNDS  |  15 XP PER HIT  |  READY?\033[0m\n\n";
        for (const auto& q : bosses) {
            cout << "BOSS: " << q.prompt << "\n";
            for (size_t i=0; i<q.options.size(); ++i) cout << char('A'+i) << ") " << q.options[i] << "\n";
            cout << "Choose your strategy: "; char a; cin >> a; a = static_cast<char>(toupper(static_cast<unsigned char>(a)));
            if (a >= 'A' && a <= 'D' && a-'A' == q.answer) { ++score; player.earnXP(15); cout << "\033[32mBoss hit! +15 XP.\033[0m " << q.explanation << "\n\n"; }
            else { player.addMistake("LeetCode Boss - " + q.prompt + " | Answer: " + char('A'+q.answer)); cout << "Boss defence held! Correct: " << char('A'+q.answer) << "). " << q.explanation << "\n\n"; }
        }
        if (score == 4) { player.addAchievement("Pattern Slayer"); cout << "\033[38;5;220mALL BOSSES DEFEATED — Pattern Slayer unlocked!\033[0m\n"; }
        cout << "Boss battle score: " << score << "/4\n"; saveData();
    }

    // Mini-game 7: Sorting Sprint - choose the next smallest item in selection-sort rounds.
    void playSortingSprint() {
        gameBanner("SORTING SPRINT", "Pick the smallest remaining value and build a sorted array.",
                   "       [ 7 ] [ 2 ] [ 5 ] [ 1 ]  --->  [ ? ? ? ? ]");
        cout << "\033[38;5;220m4 QUICK ROUNDS  |  5 XP PER CORRECT PICK\033[0m\n";
        vector<int> values = {7, 2, 5, 1};
        vector<int> expected = {1, 2, 5, 7};
        int correct=0;
        for (size_t i=0; i<expected.size(); ++i) {
            cout << "Remaining values: ";
            for (int v : values) cout << v << " ";
            cout << "\nWhich value should be selected next? "; int answer;
            if (!(cin >> answer)) { clearInput(); cout << "Enter a number.\n"; return; }
            if (answer == expected[i]) { ++correct; player.earnXP(5); cout << "Correct pick! +5 XP.\n"; }
            else { player.addMistake("Sorting Sprint - expected next smallest value " + to_string(expected[i])); cout << "Not quite: the smallest remaining value is " << expected[i] << ".\n"; }
            auto it = find(values.begin(), values.end(), expected[i]); if (it != values.end()) values.erase(it);
        }
        cout << "Sprint result: " << correct << "/4 sorted picks. Selection sort repeatedly chooses the minimum.\n";
        if (correct == 4) player.addAchievement("Sorting Sprinter");
        saveData();
    }

    void showMiniGameArcade() {
        int choice = -1;
        while (choice != 0) {
            cout << "\n\033[38;5;141m========== CODEQUEST ARCADE ==========\033[0m\n";
            cout << "1. Snake Quest       - continuous arcade survival + DSA rescue\n";
            cout << "2. Bug Bash          - find and fix coding bugs\n";
            cout << "3. Memory Matrix     - remember the sequence + theory card\n";
            cout << "4. Binary Treasure   - guess the number using binary-search logic\n";
            cout << "5. Output Predictor  - predict C++ code output\n";
            cout << "6. LeetCode Boss     - battle common problem-solving patterns\n";
            cout << "7. Sorting Sprint    - sort values through quick decisions\n";
            cout << "0. Return to Adventure Menu\nChoose a mini-game: ";
            if (!(cin >> choice)) { clearInput(); cout << "Enter a number from 0 to 7.\n"; continue; }
            if (choice == 1) playSnakeQuest();
            else if (choice == 2) playBugBash();
            else if (choice == 3) playMemoryMatrix();
            else if (choice == 4) playBinaryTreasure();
            else if (choice == 5) playOutputPredictor();
            else if (choice == 6) playLeetCodeBoss();
            else if (choice == 7) playSortingSprint();
            else if (choice != 0) cout << "Choose a number from 0 to 7.\n";
        }
    }

    void showLeetCodeMaterial() const {
        cout << "\n============= LEETCODE PRACTICE PATH =============\n";
        cout << "EASY\n";
        cout << "1. Two Sum — arrays/hash map; look up the complement.\n";
        cout << "2. Best Time to Buy and Sell Stock — track minimum price so far.\n";
        cout << "3. Valid Parentheses — stack and bracket matching.\n";
        cout << "4. Binary Search — sorted array and shrinking search range.\n";
        cout << "5. Merge Two Sorted Lists — linked-list pointers.\n\n";
        cout << "MEDIUM\n";
        cout << "6. 3Sum — sorting, two pointers, duplicate handling.\n";
        cout << "7. Longest Substring Without Repeating Characters — sliding window.\n";
        cout << "8. Add Two Numbers — linked lists and carry.\n";
        cout << "9. Binary Tree Level Order Traversal — queue/BFS.\n";
        cout << "10. Number of Islands — grid traversal with BFS/DFS.\n\n";
        cout << "Routine: try 20 minutes first, write your approach, code it, test edge cases, then check time/space complexity.\n";
        cout << "Search the exact titles on LeetCode for the original statements and test cases.\n";
        cout << "==================================================\n";
    }

    void logMistake() {
        clearInput();
        cout << "Describe a mistake or concept you want to revisit: ";
        string mistake;
        getline(cin, mistake);
        mistake = trim(mistake);
        if (mistake.empty()) {
            cout << "Nothing entered; no mistake recorded.\n";
            return;
        }
        player.addMistake(mistake);
        ofstream out(mistakeFile, ios::app);
        if (out) out << mistake << '\n';
        cout << "Saved. Mistakes are data, not failure. Add this to your revision list too!\n";
        saveData();
    }

    void showMistakes() const {
        cout << "\n------------- MISTAKE JOURNAL -------------\n";
        if (player.getMistakes().empty()) cout << "No mistakes logged yet. That is okay!\n";
        for (size_t i = 0; i < player.getMistakes().size(); ++i)
            cout << i + 1 << ". " << player.getMistakes()[i] << '\n';
        cout << "-------------------------------------------\n";
    }

    void addRevision() {
        clearInput();
        cout << "Topic to revise: ";
        string topic;
        getline(cin, topic);
        topic = trim(topic);
        if (topic.empty()) {
            cout << "Topic cannot be empty.\n";
            return;
        }
        revisionTopics.push_back(topic);
        saveData();
        cout << "Added to revision queue. Suggested review: tomorrow, in 3 days, and in 7 days.\n";
    }

    void showRevisions() const {
        cout << "\n------------- REVISION SCHEDULE ------------\n";
        if (revisionTopics.empty()) cout << "Your revision queue is empty. Add a topic after studying.\n";
        for (size_t i = 0; i < revisionTopics.size(); ++i)
            cout << i + 1 << ". " << revisionTopics[i]
                 << "  | Reviews: tomorrow, +3 days, +7 days\n";
        cout << "--------------------------------------------\n";
    }

    void comebackMode() {
        cout << "\n============= COMEBACK MODE =============\n";
        cout << "Missed a few days? No guilt. Restart small.\n";
        cout << "Your 3-step comeback mission:\n";
        cout << "  1. Revise one old concept for 10 minutes.\n";
        cout << "  2. Solve one easy problem.\n";
        cout << "  3. Write down one thing you learned.\n";
        cout << "A comeback is built with one small action—not a perfect streak.\n";
        cout << "=========================================\n";
    }

    void showWorlds() const {
        cout << "\n=============== DSA WORLDS ===============\n";
        const vector<string> worlds = {
            "The Array Meadows", "Linked List Ruins",
            "The Treewilds", "Graphfall Citadel"
        };
        const vector<int> levels = {1, 2, 4, 6};
        for (size_t i = 0; i < worlds.size(); ++i) {
            cout << (player.getLevel() >= levels[i] ? "[UNLOCKED] " : "[LOCKED - level ")
                 << (player.getLevel() >= levels[i] ? "" : to_string(levels[i]) + "] ")
                 << worlds[i] << '\n';
        }
        cout << "=========================================\n";
    }

    void showAchievements() const {
        cout << "\n=============== ACHIEVEMENTS =============\n";
        if (player.getAchievements().empty()) cout << "Complete quests to unlock your first badge.\n";
        for (const auto& a : player.getAchievements()) cout << "  * " << a << '\n';
        cout << "=========================================\n";
    }

    void clearInput() const {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Game 8: Code Breaker - crack a randomly generated three-digit code.
    void playCodeBreaker() {
        gameBanner("CODE BREAKER", "Crack the vault code using logic and feedback", "       [ ? ] [ ? ] [ ? ]   ->   VAULT");
        random_device rd; mt19937 gen(rd()); uniform_int_distribution<int> digit(0, 9);
        int secret = digit(gen) * 100 + digit(gen) * 10 + digit(gen);
        int tries = 7;
        cout << "You have 7 attempts. Each guess receives digit-position feedback.\n";
        while (tries > 0) {
            int guess; cout << "\nAttempts left " << tries << "/7 | Enter a 3-digit code (or -1 to leave): ";
            if (!(cin >> guess)) { clearInput(); cout << "Enter a number.\n"; continue; }
            if (guess == -1) return;
            if (guess < 0 || guess > 999) { cout << "Use a number from 000 to 999.\n"; continue; }
            if (guess == secret) {
                cout << "\033[38;5;46mVAULT UNLOCKED! +30 XP\033[0m\n"; player.earnXP(30); player.addAchievement("Code Cracker"); saveData(); return;
            }
            int a=guess/100, b=(guess/10)%10, c=guess%10;
            int x=secret/100, y=(secret/10)%10, z=secret%10;
            int exact=(a==x)+(b==y)+(c==z);
            cout << "Correct positions: " << exact << "/3. ";
            if (guess < secret) cout << "Hint: secret is HIGHER.\n"; else cout << "Hint: secret is LOWER.\n";
            --tries;
        }
        cout << "Vault stayed locked. The code was " << setw(3) << setfill('0') << secret << setfill(' ') << ".\n";
        cout << "Theory checkpoint: binary search works on sorted data by repeatedly halving the search range.\n";
        player.addMistake("Code Breaker: remember to use feedback systematically rather than guessing randomly."); saveData();
    }

    // Game 9: Algorithm Runner - choose a lane while an obstacle course advances.
    void playAlgorithmRunner() {
        gameBanner("ALGORITHM RUNNER", "Choose the correct path to dodge algorithm traps", "     O----O----O----O----O----O");
        vector<Question> qs = {
            {"Which structure follows FIFO?", {"Stack","Queue","Tree","Heap"},1,"A queue is First In, First Out."},
            {"Which search needs sorted data?", {"Linear search","DFS","Binary search","BFS"},2,"Binary search repeatedly halves a sorted search range."},
            {"Which traversal commonly uses a queue?", {"BFS","DFS","Inorder","Quick sort"},0,"BFS explores level by level using a queue."},
            {"Average quicksort time complexity?", {"O(n^2) always","O(log n)","O(n log n)","O(1)"},2,"Quicksort averages O(n log n), though worst case can be O(n^2)."},
            {"Which is best for matching brackets?", {"Queue","Stack","Graph","Array only"},1,"A stack tracks the most recent opening bracket."}
        };
        int lives=3, score=0;
        for (int i=0; i<(int)qs.size() && lives>0; ++i) {
            cout << "\nDISTANCE " << (i+1)*100 << "m | Lives: "; for(int l=0;l<lives;++l) cout << "♥ "; cout << "\n";
            cout << "Obstacle ahead! " << qs[i].prompt << "\n";
            for(int j=0;j<4;++j) cout << char('A'+j) << ". " << qs[i].options[j] << "\n";
            cout << "Choose your lane (A-D): "; char a; cin >> a; a=static_cast<char>(toupper(static_cast<unsigned char>(a)));
            if(a>='A' && a<='D' && a-'A'==qs[i].answer) { ++score; player.earnXP(10); cout << "\033[38;5;46mClean dodge! +10 XP\033[0m " << qs[i].explanation << "\n"; }
            else { --lives; player.addMistake("Algorithm Runner: " + qs[i].prompt + " | Correct: " + qs[i].options[qs[i].answer]); cout << "Crash! Correct answer: " << char('A'+qs[i].answer) << ". " << qs[i].explanation << "\n"; }
            sleepMs(250);
        }
        cout << "\nFINISH LINE! Score " << score << "/5.\n";
        if(score>=4) { player.addAchievement("Algorithm Athlete"); player.earnXP(15); cout << "Bonus +15 XP!\n"; }
        saveData();
    }

    // Game 10: DSA Dungeon - clear three rooms to reach the final chest.
    void playDSADungeon() {
        gameBanner("DSA DUNGEON", "Clear three rooms. Each correct answer unlocks the next door.", "       [ ROOM 1 ] -> [ ROOM 2 ] -> [ BOSS CHEST ]");
        vector<Question> qs = {
            {"What is the time complexity of accessing arr[i]?", {"O(n)","O(1)","O(log n)","O(n log n)"},1,"Array index access is O(1)."},
            {"Which data structure is used for function calls?", {"Queue","Graph","Call stack","Hash set"},2,"Function calls and local frames are managed by the call stack."},
            {"Which traversal finds the shortest path in an unweighted graph?", {"BFS","DFS","Inorder","Selection sort"},0,"BFS explores nodes in increasing distance from the source."}
        };
        int cleared=0;
        for(int i=0;i<(int)qs.size();++i) {
            cout << "\n\033[38;5;213mROOM " << i+1 << "/3\033[0m | Door sealed!\n" << qs[i].prompt << "\n";
            for(int j=0;j<4;++j) cout << char('A'+j) << ". " << qs[i].options[j] << "\n";
            cout << "Answer (A-D): "; char a; cin >> a; a=static_cast<char>(toupper(static_cast<unsigned char>(a)));
            if(a>='A' && a<='D' && a-'A'==qs[i].answer) { ++cleared; player.earnXP(15); cout << "Door unlocked! +15 XP. " << qs[i].explanation << "\n"; }
            else { player.addMistake("DSA Dungeon: " + qs[i].prompt + " | Correct: " + qs[i].options[qs[i].answer]); cout << "Door remains locked, but a scroll reveals the answer: " << char('A'+qs[i].answer) << ". " << qs[i].explanation << "\n"; }
        }
        if(cleared==3) { cout << "\n\033[38;5;220mBOSS CHEST UNLOCKED! +25 XP\033[0m\n"; player.earnXP(25); player.addAchievement("Dungeon Delver"); }
        else cout << "Dungeon complete: " << cleared << "/3 rooms cleared. Return stronger and try again!\n";
        saveData();
    }

    void home() const {
        cout << "\033[38;5;141m\n+================================================+\n";
        cout << "|                   CODEQUEST                    |\n";
        cout << "|             Build a daily coding habit through play     |\n";
        cout << "+================================================+\033[0m\n";
        cout << "  Welcome, \033[38;5;213m" << player.getName() << "\033[0m!\n";
        cout << "  \033[38;5;220mLevel " << player.getLevel() << "  |  " << player.getXP()
             << " XP  |  " << player.getCompletedQuests() << " quests cleared\033[0m\n";
        cout << "  Progress: [";
        int progress = player.getXP() % 100;
        for (int i = 0; i < 10; ++i) cout << (progress >= (i + 1) * 10 ? "#" : "-");
        cout << "] to next level\n";
        cout << "  \033[2mToday's rule: one small quest is still a win.\033[0m\n";
    }

public:
    CodeQuest() : player("Adventurer") {
        seedQuests();
        loadData();
        unlockAchievements();
    }

    void run() {
        int choice = -1;
        while (choice != 0) {
            home();
            cout << "\033[38;5;141m\n  ========== GAME ADVENTURE MENU ==========\033[0m\n";
            cout << "  Pick a game and learn while you play!\n\n";
            cout << "  \033[38;5;213m1.\033[0m Snake Quest          - eat, grow, and answer rescue questions\n";
            cout << "  \033[38;5;213m2.\033[0m Bug Bash             - spot C++ bugs\n";
            cout << "  \033[38;5;213m3.\033[0m Memory Matrix        - remember the sequence\n";
            cout << "  \033[38;5;213m4.\033[0m Binary Treasure      - find treasure with binary-search logic\n";
            cout << "  \033[38;5;213m5.\033[0m Output Predictor     - predict what C++ code prints\n";
            cout << "  \033[38;5;213m6.\033[0m LeetCode Boss Battle - solve coding-pattern challenges\n";
            cout << "  \033[38;5;213m7.\033[0m Sorting Sprint       - practise selection sort\n";
            cout << "  \033[38;5;213m8.\033[0m Code Breaker         - crack a logic vault\n";
            cout << "  \033[38;5;213m9.\033[0m Algorithm Runner     - dodge algorithm traps\n";
            cout << "  \033[38;5;213m10.\033[0m DSA Dungeon         - clear learning rooms\n";
            cout << "  \033[38;5;220m0. Save and exit\033[0m\n";
            cout << "  =========================================\n";
            cout << "Choose a game (0-10): ";

            if (!(cin >> choice)) {
                clearInput();
                cout << "Please enter a number from 0 to 10.\n";
                continue;
            }

            switch (choice) {
                case 1: playSnakeQuest(); break;
                case 2: playBugBash(); break;
                case 3: playMemoryMatrix(); break;
                case 4: playBinaryTreasure(); break;
                case 5: playOutputPredictor(); break;
                case 6: playLeetCodeBoss(); break;
                case 7: playSortingSprint(); break;
                case 8: playCodeBreaker(); break;
                case 9: playAlgorithmRunner(); break;
                case 10: playDSADungeon(); break;
                case 0:
                    saveData();
                    cout << "Progress saved. See you next time, adventurer!\n";
                    break;
                default:
                    cout << "Invalid choice. Choose a game from 1 to 10.\n";
            }
            if (choice != 0) {
                cout << "\nPress Enter to return to the game menu...";
                clearInput();
                cin.get();
            }
        }
    }
};

int main() {
    showIntro();
    CodeQuest app;
    app.run();
    return 0;
}
