#include <harc/Harc.hpp>

#include <harc/Config.hpp>
#include <harc/Translation_unit.hpp>
#include <harc/cli/CLI.hpp>
#include <harc/common/OS_utils.hpp>
#include <harc/Logging.hpp>
#include <harc/Error_reporting.hpp>

#include <harc/lexer/Lexer.hpp>
//#include <harc/Harc_cuda.hpp>

#include <harc/parser/Parser.hpp>
#include <harc/parser/Printer.hpp>

#include <harc/Errors.hpp>

#include <thread>
#include <mutex>
#include <chrono>

#include <vector>

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#if HARC_CUDA
    #include <cuda_runtime.h>
#endif

namespace harc {

    ///
    /// An enum which is used to indicate which stage within the compilation
    /// process should be performed next for the associated translation unit.
    ///
    enum class Stage : std::uint8_t {
        NULL_STEP,
        TOKENIZATION,
        PARSING,
        BACKEND
    };

    ///
    /// A struct meant to represent all information which is required to.
    ///
    struct Task {

        Task() = default;
        Task(const Task&) = delete;
        Task(Task&&) noexcept = default;
        ~Task() = default;

        Task& operator=(const Task&) = delete;
        Task& operator=(Task&&) noexcept = default;

        Stage stage = Stage::NULL_STEP;
        Translation_unit unit;
    };

    ///
    /// Perform compiler frontend work on the specified translation unit.
    ///
    /// \param unit Translation unit to perform frontend work for
    void run_frontend(Translation_unit& unit);

    ///
    /// Perform compiler backend work on teh specified translation unit.
    ///
    /// \param unit Translation unit to perform backend work for
    void run_backend(Translation_unit& unit);

    //=====================================================
    // Shared Worker State
    //=====================================================

    ///
    /// Queue containing units that need to be processed
    ///
    std::vector<Task> work_queue;

    ///
    /// Mutex used to protect work_queue variable
    ///
    std::mutex work_queue_mutex;

    ///
    /// Used to indicate if worker threads should terminate once there is no
    /// more work available for them to do.
    ///
    /// Set to false, when Harc is run as a client since Harc should then wait
    /// for the host to send more work in the future.
    ///
    std::atomic_bool terminate_once_idle = false;

    //=====================================================
    // CPU Worker State
    //=====================================================

    ///
    /// A list of all the threads which are used for CPU compilation
    ///
    std::vector<std::thread> cpu_worker_threads;

    //=====================================================
    // GPU Worker State
    //=====================================================

    ///
    /// A list of all the threads which are used for managing CUDA devices
    ///
    std::vector<std::thread> cuda_worker_threads;

    //=====================================================
    // Server state
    //=====================================================

    //=====================================================
    // Client Worker State
    //=====================================================

    //=====================================================
    // Worker threads
    //=====================================================

    ///
    /// Function which is run for CPU threads used in compilation process
    ///
    void cpu_worker() {
        while (true) {
            // Retrieve task from queue
            Task task{};
            {
                std::lock_guard lk{work_queue_mutex};

                if (!work_queue.empty()) {
                    task = std::move(work_queue.back());
                    work_queue.pop_back();
                }
            }

            // Respond to empty queue.
            if (Stage::NULL_STEP == task.stage) {
                if (terminate_once_idle) {
                    break;
                } else {
                    //TODO: Replace with wait for wake-up event
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(100ms);
                    continue;
                }
            }

            using clk = std::chrono::steady_clock;

            // Compile on CPU
            auto& unit = task.unit;
            if (Stage::TOKENIZATION == task.stage) {

                //TODO: Time work
                auto begin = clk::now();
                auto tokenization = lex::lex(unit.source_path, std::string_view{unit.source});
                auto end = clk::now();

                std::uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

                //TODO: Report duration
                //TODO: Create parsing task
                {
                    std::scoped_lock lk{work_queue_mutex};

                    work_queue.emplace_back();
                    work_queue.back().stage = Stage::PARSING;
                    work_queue.back().unit = std::move(unit);
                }
            }

            if (Stage::PARSING == task.stage) {
                auto begin = clk::now();
                auto tokenization = parser::parse(unit);
            }

            //TODO: Wait for the front end to run on all modules
            if (Stage::BACKEND == task.stage) {
                //TODO: Invoke compiler back end
            }
        }
    }

    #if HARC_CUDA
    ///
    /// Function which is run to assign work to CUDA streams
    ///
    void cuda_worker(int device_index, cudaStream_t stream) {
        cudaSetDevice(device_index);

        while (true) {
            Task task{};
            {
                std::lock_guard lk{work_queue_mutex};

                if (!work_queue.empty()) {
                    task = work_queue.back();
                    work_queue.pop_back();
                }
            }

            if (Stage::NULL_STEP == task.step) {
                if (terminate_once_idle) {
                    break;
                } else {
                    //TODO: Replace with wait for wake-up event
                    using namespace std::chrono_literals;
                    std::this_thread::sleep_for(100ms);
                    continue;
                }
            }

            if (Stage::TOKENIZATION == task.step) {
                //TODO: Invoke Tokenization kernels
            }
        }
    }
    #endif

    //=====================================================
    // Helper functions
    //=====================================================

    void create_cpu_worker_threads() {
        auto thread_count = std::thread::hardware_concurrency();

        for (std::size_t i = 0; i < thread_count; ++i) {
            cpu_worker_threads.emplace_back(cpu_worker);
        }
    }

    #if HARC_CUDA
    void create_cuda_worker_threads() {
        int device_count;
        cudaGetDeviceCount(&device_count);

        // This is currently an arbitrary number in need of refinement
        constexpr int streams_per_device = 8;

        // Create CUDA streams
        std::vector<std::array<cudaStream_t, streams_per_device>> device_streams;
        device_streams.resize(device_count);

        for (int i = 0; i < device_count; ++i) {
            for (int j = 0; j < streams_per_device; ++j) {
                cudaStreamCreate(device_streams[i].data() + j);
            }
        }

        // Launch threads
        cuda_worker_threads.reserve(device_count * streams_per_device);

        for (int i = 0; i < device_count; ++i) {
            for (int j = 0; j < streams_per_device; ++j) {
                cuda_worker_threads.emplace_back(
                    cuda_worker,
                    i,
                    device_streams[i][j]
                );
            }
        }
    }
    #endif

    void create_compilation_tasks(std::span<const std::string_view> source_paths) {
        for (auto& source_path : source_paths) {
            work_queue.emplace_back();
            auto& task = work_queue.back();
            task.stage = Stage::TOKENIZATION;
            task.unit.source_path = source_path;

            //TODO: Check if a cache entry exists for the specified translation
            //unit and create tasks in response to that.

            #if HARC_POSIX
            Text_file_mapping mapping = map_text_file(source_path);
            if (mapping.error_code != Error_code::NO_ERROR) {
                //TODO: Log error
                continue;
            }

            task.unit.source = mapping.text;
            unmap_file(mapping);

            #elif
            std::ifstream fin{source_path.data()};
            t.seekg(0, std::ios::end);
            std::size_t size = t.tellg();

            std::string buffer(size, ' ');
            t.seekg(0);
            t.read(&buffer[0], size);

            back.unit.source = std::move(buffer);

            #endif
        }

        //if (!Message_buffer::is_empty()) {
        //    return Error_code::GENERAL_INTERNAL_ERRORR;
        //}
    }

    //=====================================================
    // Core Harc functions
    //=====================================================

    void run_client(const Config& config) {
        terminate_once_idle = false;

        create_cpu_worker_threads();
        #if HARC_CUDA
        create_cuda_worker_threads();
        #endif

        // Wait for threads to complete their work
        for (auto& th : cpu_worker_threads) {
            th.join();
        }

        #if HARC_CUDA
        for (auto& th : cuda_worker_threads) {
            th.join();
        }
        #endif
    }

    void run_pure_server(
        std::span<const std::string_view> source_paths,
        const Config& config
    ) {
        if (source_paths.empty()) {
            //TODO: Log error
            return;
        }

        terminate_once_idle = true;

        create_compilation_tasks(source_paths);
    }

    void run_server(
        std::span<const std::string_view> source_paths,
        const Config& config
    ) {
        if (source_paths.empty()) {
            //TODO: Log error
            return;
        }

        terminate_once_idle = true;

        create_compilation_tasks(source_paths);

        create_cpu_worker_threads();
        #if HARC_CUDA
        create_cuda_worker_threads();
        #endif

        // Wait for threads to complete their work
        for (auto& th : cpu_worker_threads) {
            th.join();
        }

        #if HARC_CUDA
        for (auto& th : cuda_worker_threads) {
            th.join();
        }
        #endif
    }

    void run_locally(
        std::span<const std::string_view> source_paths,
        const Config& config
    ) {
        if (source_paths.empty()) {
            HARC_LOG_ERROR("No file paths specified.");
            return;
        }

        terminate_once_idle = true;

        create_compilation_tasks(source_paths);
        create_cpu_worker_threads();

        #if HARC_CUDA
        create_cuda_worker_threads();
        #endif

        // Wait for threads to complete their work
        for (auto& th : cpu_worker_threads) {
            th.join();
        }

        #if HARC_CUDA
        for (auto& th : cuda_worker_threads) {
            th.join();
        }
        #endif
    }

}
