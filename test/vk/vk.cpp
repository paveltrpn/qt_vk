
#include <memory>
#include <filesystem>

#include "log/log.h"
#include "render/vk/rendervk.h"

int main( int argc, char **argv ) {
    std::unique_ptr<tire::Render> rndr;
    try {
        rndr = std::make_unique<tire::RenderVK>();
        // rndr->displayRenderInfo();

        rndr->run();
    } catch ( const std::exception &e ) {
        tire::log::error( "caught exception: {}", e.what() );
        return 1;
    }

    return 0;
}
