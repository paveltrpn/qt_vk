
#pragma once

#include <thread>
#include <mutex>

#include <android_native_app_glue.h>

#include "timer.h"

namespace tire {
    struct Render {
        Render();

        Render(const Render &other) = delete;

        Render(Render &&other) = delete;

        Render &operator=(const Render &other) = delete;

        Render &operator=(Render &&other) = delete;

        virtual ~Render();

        virtual void init() = 0;

        virtual void clean() = 0;

        virtual void preLoop() = 0;

        void setWindow(ANativeWindow *window);

        void start();

        void stop();

        void run();

        virtual void postLoop() = 0;

        static void threadStartCallback(void *self);

    protected:
        virtual void preFrame() = 0;

        virtual void frame() = 0;

        virtual void postFrame() = 0;

        virtual void swapBuffers() = 0;

    protected:
        ANativeWindow *window_{};
        bool ready_{false};
        Timer timer_{};

    private:
        enum class RenderThreadMessage {
            MSG_NONE = 0,
            MSG_WINDOW_SET,
            MSG_RENDER_LOOP_EXIT
        };

        std::unique_ptr<std::thread> worker_;
        std::mutex mutex_;
        RenderThreadMessage msg_{};
    };
}
