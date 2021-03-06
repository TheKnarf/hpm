#include "install.h"
#include "variables.h"
#include "list.h"
#include "process.h"
#include "filepath.h"

#include <sys/stat.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void wpm_install(const char* name, const char* path)
{
	printf("Downloading: %s\n", path);

	mkdir(WPM_DEPENDENCY_INSTALL_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	CURL *curl = curl_easy_init();

	if(curl)
	{
		filepath* filepath = new_filepath(name, WPM_DEPENDENCY_INSTALL_DIR);
		char* file_path = fullpath(filepath);

		printf("Writing to: %s\n", file_path);
		FILE *fp = fopen(file_path, "wb");

		curl_easy_setopt(curl, CURLOPT_URL, path);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		CURLcode res = curl_easy_perform(curl);

		fclose(fp);
		free(file_path);

		if(res == CURLE_OK) {
			process(filepath);
		}

		free_filepath(filepath);
	}
	else
	{
		fprintf(stderr, "curl error\n");
	}

	curl_easy_cleanup(curl);
}

void wpm_handle_requirement(const char* key, const json_t* obj)
{
	if(json_is_string(obj))
	{
		wpm_install(key, json_string_value(obj));
	}
}

void wpm_install_from_file()
{
	json_t* root = load_wpm_json();
	process_json_file(root, wpm_handle_requirement);
	json_decref(root);
}