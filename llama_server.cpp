#include "llama_server.h"
#include "web_search.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

LlamaServer::LlamaServer() : running(false), server_fd(-1) {}

LlamaServer::~LlamaServer() {
    stop();
}

void LlamaServer::run() {
    running = true;
    serverThread = std::thread([this]() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(12345);

        if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            return;
        }

        if (listen(server_fd, 3) < 0) {
            std::cerr << "Listen failed" << std::endl;
            return;
        }

        std::cout << "Server started on port 12345" << std::endl;

        while (running) {
            int client_socket = accept(server_fd, nullptr, nullptr);
            if (client_socket >= 0) {
                std::thread(&LlamaServer::handleClient, this, client_socket).detach();
            }
        }
    });
}

void LlamaServer::handleClient(int client_socket) {
    char buffer[1024] = {0};
    while (running && read(client_socket, buffer, 1024) > 0) {
        std::string input(buffer);
        std::string response = processInput(input);
        send(client_socket, response.c_str(), response.size(), 0);
        memset(buffer, 0, 1024);
    }
    close(client_socket);
}

std::string LlamaServer::processInput(const std::string& input) {
    // Handle context
    std::string context_str = handle_context(input, context);

    // Check if the input requires a web search
    if (input.find("search:") == 0) {
        std::string query = input.substr(7);  // Extract the search query
        std::string search_result = perform_web_search(query);
        return search_result;
    }

    // Dummy response simulating model output
    return "Processed: " + context_str;
}

void LlamaServer::stop() {
    running = false;
    if (server_fd != -1) {
        close(server_fd);
        server_fd = -1;
    }
    if (serverThread.joinable()) {
        serverThread.join();
    }
}