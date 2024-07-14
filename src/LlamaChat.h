#pragma once
#include <string>
#include <vector>
#include "../llama.cpp/include/llama.h"

class LlamaChat {
public:
    LlamaChat(const std::string& model_path);
    ~LlamaChat();
    std::string generate_response(const std::string& prompt);

private:
    llama_context* ctx;
    llama_model* model;
    std::vector<llama_token> tokens;
    int n_ctx;
};