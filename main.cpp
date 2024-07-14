#include "llama_server.h"
#include <iostream>
#include <vector>
#include <string>

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

int main(int argc, char *argv[]) {
    std::string model_path = "models/phi-2.Q2_K.gguf";
    if (!load_model(model_path)) {
        return -1;
    }

    LlamaServer server;
    server.run();
    return 0;
}
