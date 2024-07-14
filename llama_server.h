// llama_server.h
#ifndef LLAMA_SERVER_H
#define LLAMA_SERVER_H

#include <thread>
#include <string>

class LlamaServer {
public:
    LlamaServer();
    ~LlamaServer();
    void run();
    void stop();

private:
    void handleClient(int client_socket);
    std::string processInput(const std::string& input);
    bool running;
    int server_fd;
    std::thread serverThread;
    std::string context;  // Add this member to store the context
};

std::string handle_context(const std::string& input, std::string& context);

#endif // LLAMA_SERVER_H
