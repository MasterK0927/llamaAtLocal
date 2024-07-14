#include "web_search.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>

// Replace with your own API key securely
const std::string API_KEY = "YOUR_API_KEY";
const std::string SEARCH_ENGINE_URL = "https://api.bing.microsoft.com/v7.0/search?q=";

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string extract_field(const std::string& json, const std::string& field_name) {
    std::string field = "\"" + field_name + "\":\"";
    auto start = json.find(field);
    if (start == std::string::npos) return "";
    start += field.length();
    auto end = json.find("\"", start);
    if (end == std::string::npos) return "";
    return json.substr(start, end - start);
}

std::string perform_web_search(const std::string& query) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = SEARCH_ENGINE_URL + curl_easy_escape(curl, query.c_str(), query.length());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Ocp-Apim-Subscription-Key: " + API_KEY).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        return "Failed to initialize CURL.";
    }

    // Manually parse JSON response for web page titles and URLs
    std::string results;
    size_t pos = 0;
    while ((pos = readBuffer.find("\"name\"", pos)) != std::string::npos) {
        std::string name = extract_field(readBuffer.substr(pos), "name");
        std::string url = extract_field(readBuffer.substr(pos), "url");
        if (!name.empty() && !url.empty()) {
            results += name + ": " + url + "\n";
        }
        pos += name.length() + url.length();
    }

    return results.empty() ? "No results found." : results;
}
