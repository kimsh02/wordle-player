#pragma once

#include <string>

class NYTimesFetcher {
    private:
	static constexpr std::size_t DATE_LEN = 11;
	static constexpr std::size_t URL_LEN  = 53;
	static constexpr const char *nytimesURL =
		"https://www.nytimes.com/svc/wordle/v2/";
	static constexpr const char *jsonExt = ".json";
	static constexpr const char *key     = "solution";
	static size_t write_callback(void *, size_t, size_t, void *);

	std::string buildURL(void) const;

    public:
	std::string fetch(void) const;
};
