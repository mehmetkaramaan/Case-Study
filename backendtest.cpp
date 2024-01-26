#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json; 

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://flights-api.example.com/flights");

      
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

       
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        
        res = curl_easy_perform(curl);

      
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
        } else {
            
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            
            if (http_code == 200) {
              
                try {
                    json jsonResponse = json::parse(response);

                    // Check response structure
                    if (jsonResponse.contains("data") && jsonResponse["data"].is_array()) {
                        for (const auto& flight : jsonResponse["data"]) {
                            if (!flight.contains("id") || !flight.contains("from") || !flight.contains("to") || !flight.contains("date")) {
                                std::cerr << "Invalid response format!" << std::endl;
                                break;
                            }
                        }
                    } else {
                        std::cerr << "Invalid response format!" << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing JSON response: " << e.what() << std::endl;
                }

                
                char* content_type;
                res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
                if ((res == CURLE_OK) && content_type) {
                    if (std::string(content_type) == "application/json") {
                        std::cout << "Content-Type header is correct." << std::endl;
                    } else {
                        std::cerr << "Invalid Content-Type header!" << std::endl;
                    }
                }
            } else {
                std::cerr << "HTTP Error: " << http_code << std::endl;
            }
        }

        
        curl_easy_cleanup(curl);
    }

    
    curl_global_cleanup();

    return 0;
}

