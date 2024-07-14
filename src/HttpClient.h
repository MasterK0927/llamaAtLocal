#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>

class HttpClient {
public:
    HttpClient(const std::string& host, int port) : cli(host, port) {}

    std::string chat(const std::string& message) {
        nlohmann::json j = {
            {"prompt", message},
            {"n_predict", 128}
        };

        auto res = cli.Post("/completion", j.dump(), "application/json");

        if (res && res->status == 200) {
            auto response_json = nlohmann::json::parse(res->body);
            return response_json["content"].get<std::string>();
        } else {
            return "Error: Unable to get response from server";
        }
    }

private:
    httplib::Client cli;
};