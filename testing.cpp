//this

#include "util_deps/gg_upload_util.hpp"
#include "util_deps/gg_api_util.hpp"

int main() {
    std::string msg = gg::network_custom("request");
    msg = "check_key;"+msg;
    std::cout<<gg::network_custom(msg)<<std::endl;
    gg::upload_file("temp.GIF");
    return EXIT_SUCCESS;
}