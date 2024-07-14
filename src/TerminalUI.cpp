#include "TerminalUI.h"
#include <algorithm>

TerminalUI::TerminalUI() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    chatWindow = newwin(maxY - 3, maxX, 0, 0);
    inputWindow = newwin(3, maxX, maxY - 3, 0);

    scrollok(chatWindow, TRUE);
    scrollok(inputWindow, TRUE);
}

TerminalUI::~TerminalUI() {
    endwin();
}

void TerminalUI::display() {
    wclear(chatWindow);
    int maxY, maxX;
    getmaxyx(chatWindow, maxY, maxX);

    int startY = std::max(0, static_cast<int>(messages.size()) - maxY);
    for (size_t i = startY; i < messages.size(); ++i) {
        wprintw(chatWindow, "%s\n", messages[i].c_str());
    }
    wrefresh(chatWindow);

    wclear(inputWindow);
    box(inputWindow, 0, 0);
    mvwprintw(inputWindow, 1, 1, "Enter message: ");
    wrefresh(inputWindow);
}

std::string TerminalUI::getInput() {
    char input[256];
    echo();
    wgetnstr(inputWindow, input, sizeof(input) - 1);
    noecho();
    return std::string(input);
}

void TerminalUI::addMessage(const std::string& message) {
    messages.push_back(message);
}