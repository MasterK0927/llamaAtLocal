#include "TerminalUI.h"
#include "LlamaChat.h"
#include <iostream>
#include <filesystem>

int main() {
    std::string modelPath = std::filesystem::current_path() / "share" / "llamachat" / "model.gguf";
    
    try {
        LlamaChat llama(modelPath);
        TerminalUI ui;
        
        while (true) {
            ui.display();
            std::string input = ui.getInput();
            
            if (input == "exit") break;
            
            ui.addMessage("User: " + input);
            std::string response = llama.generate_response(input);
            ui.addMessage("Llama: " + response);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}