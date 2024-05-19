//this

#include <chrono>
#include <thread>

#include "util_deps/gg_upload_util.hpp"
#include "util_deps/gg_api_util.hpp"
#include "util_deps/gg_download_util.hpp"

void delayOneSecond() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    std::string msg = gg::network_custom("request");
    msg = "check_key;"+msg;
    std::cout<<gg::network_custom(msg)<<std::endl;
    std::cout<<gg::upload_file("temp.GIF")<<std::endl;
    std::string key_store = gg::network_custom("get_v");
    std::cout<<key_store<<std::endl;
    std::cout<<gg::util_download_file(key_store)<<std::endl;
    return EXIT_SUCCESS;
}