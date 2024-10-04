#include <harc/Logging.hpp>

namespace harc {

    void initialize_loggers() {
        quill::Config config;
        config.enable_console_colours = false;

        quill::configure(config);
        quill::start();

        auto private_handler = quill::stderr_handler();
        private_handler->set_pattern(
            "%(level_name):\nSourced from function '%(function_name)' on line %(lineno) in %(pathname)\n%(message)\n",
            "",
            quill::Timezone::LocalTime
        );

        impl::private_logger = quill::create_logger(
            "private",
            std::move(private_handler)
        );
    }

}
