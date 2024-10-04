#ifndef HARC_LOGGING_HPP
#define HARC_LOGGING_HPP

#if !defined(__NVCC__)
    #define QUILL_DISABLE_NON_PREFIXED_MACROS
    #include <quill/Quill.h>
#endif

namespace harc {

    namespace impl {

        #if !defined(__NVCC__)
        inline quill::Logger* private_logger = nullptr;
        #endif

    }

    ///
    /// Initialize internal logging facilities
    ///
    void initialize_loggers();

    //=====================================================
    // Logging macros
    //=====================================================

    #if defined(__NVCC__)
    // Can't log errors on the GPU!

    #define HARC_LOG_CRITICAL(...)
    #define HARC_LOG_ERROR(...)
    #define HARC_LOG_WARNING(...)
    #define HARC_LOG_INFO(...)
    #define HARC_LOG_DEBUG(...)

    #else

    #define HARC_LOG_CRITICAL(...) QUILL_LOG_CRITICAL(harc::impl::private_logger, __VA_ARGS__)
    #define HARC_LOG_ERROR(...)    QUILL_LOG_ERROR(harc::impl::private_logger, __VA_ARGS__)
    #define HARC_LOG_WARNING(...)  QUILL_LOG_WARNING(harc::impl::private_logger, __VA_ARGS__)
    #define HARC_LOG_INFO(...)     QUILL_LOG_INFO(harc::impl::private_logger, __VA_ARGS__)
    #define HARC_LOG_DEBUG(...)    QUILL_LOG_DEBUG(harc::impl::private_logger, __VA_ARGS__)

    #endif

}

#endif //HARC_LOGGING_HPP
