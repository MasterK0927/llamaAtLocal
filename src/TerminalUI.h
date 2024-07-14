#pragma once
#include <ncurses.h>
#include <string>
#include <vector>

class TerminalUI {
public:
    TerminalUI();
    ~TerminalUI();
    void display();
    std::string getInput();
    void addMessage(const std::string& message);

private:
    std::vector<std::string> messages;
    WINDOW* inputWindow;
    WINDOW* chatWindow;
};