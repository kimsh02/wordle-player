#include "nytimesfetcher.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <cstdlib>
#include <ctime>
#include <vector>

std::string NYTimesFetcher::fetch(void) const
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL	   *curl = curl_easy_init();
	std::string readBuffer{};
	if (curl) {
		std::string url{ buildURL() };
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	auto jsonData = nlohmann::json::parse(readBuffer);
	return jsonData[key].get<std::string>();
}

size_t NYTimesFetcher::write_callback(void *contents, size_t size, size_t nmemb,
				      void *userp)
{
	static_cast<std::string *>(userp)->append(static_cast<char *>(contents),
						  size * nmemb);
	return size * nmemb;
}

std::string NYTimesFetcher::buildURL(void) const
{
	/* Get current date */
	std::time_t t = std::time(nullptr);
	std::tm	    tm;
	localtime_r(&t, &tm);
	char date[DATE_LEN];
	std::strftime(date, DATE_LEN, "%Y-%m-%d", &tm);

	/* Build url */
	std::vector<const char *> buffers{ nytimesURL, date, jsonExt };

	std::string url{};
	url.reserve(URL_LEN);
	for (const auto &s : buffers) {
		url.append(s);
	}
	return url;
}
