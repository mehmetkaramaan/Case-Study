#include <iostream>
#include <curl/curl.h>
#include <string>

// Callback fonksiyonu verileri yazd�r�r
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // API URL
    std::string url = "https://flights-api.buraky.workers.dev/";

    // HTTP client ba�lat
    CURL* curl = curl_easy_init();
    if (curl) {
        // HTTP iste�i haz�rla
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

        // Response verilerini almak i�in bir string kullan
        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // HTTP iste�i g�nder
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
        } else {
            // HTTP status code'unu al
            long http_status_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status_code);

            // HTTP status code kontrol�
            if (http_status_code != 200) {
                std::cerr << "HTTP status code: " << http_status_code << std::endl;
            } else {
                // Response i�eri�i kontrol�
                std::cout << "Response content:\n" << response_data << std::endl;

                // JSON parsing ve daha fazla test senaryosu eklenebilir
            }
        }

        // HTTP client temizle
        curl_easy_cleanup(curl);
    }

    return 0;
}
// Bu C++ program�nda, belirtilen API'yi test etmek i�in libcurl k�t�phanesini kulland�m.
