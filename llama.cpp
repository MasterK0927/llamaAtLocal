#include "llama_server.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function to load the model file
bool load_model(const std::string& model_path) {
    std::ifstream model_file(model_path, std::ios::binary);
    if (!model_file.is_open()) {
        std::cerr << "Failed to open model file: " << model_path << std::endl;
        return false;
    }

    // Dummy load simulation
    std::cout << "Model file " << model_path << " loaded successfully." << std::endl;
    model_file.close();
    return true;
}

// Function to handle context
std::string handle_context(const std::string& input, std::vector<std::string>& context) {
    context.push_back(input);
    // Limit the context size to the last 5 messages for simplicity
    if (context.size() > 5) {
        context.erase(context.begin());
    }
    std::string combined_context;
    for (const auto& msg : context) {
        combined_context += msg + " ";
    }
    return combined_context;
}

int main(int argc, char *argv[]) {
    std::string model_path = "models/llama_model.gguf";
    if (!load_model(model_path)) {
        return -1;
    }

    LlamaServer server;
    server.run();
    return 0;
}