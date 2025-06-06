
#include "render.h"
#include "../log/log.h"

namespace tire {
    Render::Render() {
        log::info("render initialization started...");
    }

    Render::~Render() {
        log::info("Render === native render object destoyed");
    }

    void Render::setWindow(ANativeWindow *window) {
        std::lock_guard lock{mutex_};
        msg_ = RenderThreadMessage::MSG_WINDOW_SET;
        window_ = window;
    }

    void Render::start() {
        worker_ = std::make_unique<std::thread>(threadStartCallback, this);
    }

    void Render::stop() {
        {
            std::lock_guard lock{mutex_};
            msg_ = RenderThreadMessage::MSG_RENDER_LOOP_EXIT;
        }
        worker_->join();
    }

    void Render::run() {
        bool renderingEnabled = true;

        preLoop();

        while (renderingEnabled) {
            std::lock_guard lock{mutex_};
            switch (msg_) {
                case RenderThreadMessage::MSG_WINDOW_SET:
                    init();
                    break;

                case RenderThreadMessage::MSG_RENDER_LOOP_EXIT:
                    renderingEnabled = false;
                    clean();
                    break;

                default:
                    break;
            }

            msg_ = RenderThreadMessage::MSG_NONE;

            if (ready_) {
                preFrame();
                frame();
                swapBuffers();
                postFrame();
            }
        }

        postLoop();
    }

    void Render::threadStartCallback(void *self) {
        auto *renderer = static_cast<Render *>(self);
        renderer->run();
        renderer->worker_.reset();
    }

}


