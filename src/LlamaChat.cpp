#include "LlamaChat.h"
#include <stdexcept>
#include <cstring>
#include "common.h"

LlamaChat::LlamaChat(const std::string& model_path) {
    llama_backend_init(false);

    llama_model_params model_params = llama_model_default_params();
    model = llama_load_model_from_file(model_path.c_str(), model_params);

    if (!model) {
        throw std::runtime_error("Failed to load model");
    }

    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048;
    ctx = llama_new_context_with_model(model, ctx_params);

    if (!ctx) {
        llama_free_model(model);
        throw std::runtime_error("Failed to create context");
    }

    n_ctx = llama_n_ctx(ctx);
    tokens.resize(n_ctx);
}

LlamaChat::~LlamaChat() {
    llama_free(ctx);
    llama_free_model(model);
    llama_backend_free();
}

std::string LlamaChat::generate_response(const std::string& prompt) {
    const int max_prompt_tokens = n_ctx / 2;
    int num_prompt_tokens = llama_tokenize(ctx, prompt.c_str(), prompt.length(), tokens.data(), max_prompt_tokens, true);

    if (num_prompt_tokens < 0) {
        throw std::runtime_error("Failed to tokenize prompt");
    }

    if (llama_eval(ctx, tokens.data(), num_prompt_tokens, 0, 4) != 0) {
        throw std::runtime_error("Failed to evaluate prompt");
    }

    std::string response;
    const int max_response_tokens = n_ctx - num_prompt_tokens;
    int num_tokens = num_prompt_tokens;

    for (int i = 0; i < max_response_tokens; ++i) {
        llama_token new_token_id = llama_sample_token(ctx);

        if (new_token_id == llama_token_eos()) {
            break;
        }

        const char* token_str = llama_token_to_str(ctx, new_token_id);
        response += token_str;

        tokens[num_tokens] = new_token_id;
        if (llama_eval(ctx, &tokens[num_tokens], 1, num_tokens, 4) != 0) {
            throw std::runtime_error("Failed to evaluate new token");
        }
        num_tokens++;

        if (response.find('\n') != std::string::npos) {
            break;
        }
    }

    return response;
}