#include <harc/lexer_cuda/Lexer_cuda.hpp>
#include <harc/lexer_cuda/Lexing_cuda.hpp>

#include <harc/Translation_unit_cuda.hpp>
#include "harc/cli/CLI.hpp"

#include <harc/common/OS_utils.hpp>

#include <cuda_runtime.h>

#include <thread>
#include <mutex>
#include <vector>

namespace harc::cuda {

    std::vector<Translation_unit> translation_units;

    std::mutex translation_units_mutex;


    std::vector<std::string_view> tokenization_queue{};

    std::mutex tokenization_queue_mutex;


    void tokenization_worker(int device_index, cudaStream_t stream) {
        cudaSetDevice(device_index);

        cudaDeviceProp device_properties;
        cudaGetDeviceProperties(&device_properties, device_index);

        std::uint32_t warp_size = device_properties.warpSize;

        while (true) {
            std::string_view source_path{};

            {   // Get path of source file to process
                std::lock_guard lk{tokenization_queue_mutex};

                if (tokenization_queue.empty()) {
                    tokenization_queue_mutex.unlock();
                    break;
                }

                source_path = tokenization_queue.back();
                tokenization_queue.pop_back();
            }

            Translation_unit unit{source_path};
            compute_tokenization_buffer_sizes(unit);
            cudaMallocAsync(&unit.device_source_buffer, unit.source_buffer_size, stream);

            cudaMemsetAsync(
                unit.device_source_buffer,
                0,
                unit.source_buffer_size,
                stream
            );

            cudaStreamSynchronize(stream);

            {   // Emplace work
                std::lock_guard lk{translation_units_mutex};
                translation_units.push_back(unit);
            }

            //Print out timing statistics
            std::to_string(unit.timings.required_utf8_latency);

            HARC_LOG_INFO(
                "Translation unit kernel timings:\nDecoding utf8 latency: {}ms",
                std::to_string(unit.timings.utf8_decode_latency)
            );
        }
    }

    Error_code run_compiler() {
        // Get device count
        int device_count;
        cudaGetDeviceCount(&device_count);


        // This is currently an arbitrary amount
        constexpr int kernels_per_device = 8;


        // Create streams
        std::vector<std::array<cudaStream_t, kernels_per_device>> device_streams;
        device_streams.resize(device_count);

        for (int i = 0; i < device_count; ++i) {
            for (int j = 0; j < kernels_per_device; ++j) {
                cudaStreamCreate(device_streams[i].data() + j);
            }
        }


        // Populate tokenization queue
        tokenization_queue.insert(
            tokenization_queue.end(),
            cli::arguments.source_paths.begin(),
            cli::arguments.source_paths.end()
        );


        // Launch threads
        std::vector<std::thread> threads;
        threads.reserve(device_count * kernels_per_device);

        for (int i = 0; i < device_count; ++i) {
            for (int j = 0; j < kernels_per_device; ++j) {
                threads.emplace_back(
                    tokenization_worker,
                    i,
                    device_streams[i][j]
                );
            }
        }

        // Wait on threads to complete
        for (auto& thread : threads) {
            thread.join();
        }

        return Error_code::NO_ERROR;
    }

}