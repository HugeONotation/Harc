#ifndef HARMONIA_GRID_SYNCHRONIZATION_HPP
#define HARMONIA_GRID_SYNCHRONIZATION_HPP

#include <cstdint>

namespace harc::cuda {

    //=====================================================
    // Bit-wise algorithms
    //=====================================================

    [[nodiscard]]
    __device__ __forceinline__
    std::uint32_t bit_ceil(std::uint32_t x) {
        auto sh = (64 - __clzll(std::uint64_t(x) - 1));
        return std::uint64_t(1) << sh;
    }

    //=====================================================
    // Scan algorithms
    //=====================================================

    ///
    /// Computes a Boolean prefix-sum of an array in-place, looking for elements
    /// that equal to the target value
    ///
    /// Assumes a 1-d grid whose size is not greater than the length of buffer
    /// parameter
    ///
    /// \param buffer Buffer containing information
    /// \param target Value to count
    __device__
    inline void inclusive_boolean_scan_across_grid(std::uint32_t* buffer) {
        //__shared__ std::uint32_t shared_buffer[1024];

        // Compute relevant indices
        std::uint32_t warp_index = threadIdx.x % warpSize;
        std::size_t thread_index = blockDim.x * blockIdx.x + threadIdx.x;

        // Scan across single block

        // Propagate sums bottom-up in block

        // Begin by computing scan on a per-warp level
        std::uint32_t warp_count = __ballot_sync(0xffffffff, bool(buffer[thread_index]));
        std::uint32_t warp_mask = (std::uint64_t(1) << warp_index) - 1;

        std::uint32_t warp_sum = __popc(warp_count & warp_mask);

        // Sum across entire block
        /*
        for (std::uint32_t i = warpSize; i < blockDim.x; i *= 2) {
            //TODO: Optimize this. This is prone to bank conflicts
            if (thread_index % i == (i - 1)) {
                shared_buffer[] = thread_index / i;
            }
            __syncthreads();
            shared_buffer[thread_index ] = warp_sum;

            auto read_address = (threadIdx.x / i) * i - 1;
            auto is_thread_active = (threadIdx.x / i) & 0x1;
            if (is_thread_active) {
                shared_buffer[threadIdx.x] += read_address;
            }
        }
        */

        __syncthreads();

        // Propagate sums top-down

        // Scan across grid

        // Propagate sums bottom-up

        // Propagate sums top-down

    }

    __device__
    inline void inclusive_prefix_sum(std::uint32_t* buffer);

    //=====================================================
    // Searching/Finding algorithms
    //=====================================================

    template<class Int>
    [[nodiscard]]
    __device__
    const Int* lower_bound(const Int* array, std::uint32_t size, Int val) {
        //Perform binary search on the first array
        std::uint32_t begin = 0;
        std::uint32_t pivot;

        // Number of iterations is ceil(log2(size))
        for (int i = 0; i < bit_ceil(size); ++i) {
            std::uint32_t half = (size >> 1);

            pivot = begin + half;
            begin = begin + ((size - half) & -std::int32_t(array[pivot] < val));

            size = half;
        }

        return array + pivot;
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    const Int* lower_bound(const Int (&array)[N], Int val) {
        return lower_bound<Int>(array, N, val);
    }

    template<class Int>
    [[nodiscard]]
    __device__
    bool lower_bound_find(const Int* array, std::uint32_t size, Int val) {
        const Int* pos = lower_bound(array, size, val);
        return (pos != (array + size)) && (*pos == val);
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    bool lower_bound_find(const Int (&array)[N], Int val) {
        return lower_bound_find(array, N, val);
    }

    //=====================================================
    // Array utilities
    //=====================================================

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    constexpr std::size_t extent(const Int (&dummy)[N]) {
        return N;
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    const Int* begin(const Int (&dummy)[N]) {
        return dummy;
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    Int* begin(Int (&dummy)[N]) {
        return dummy;
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    const Int* end(const Int (&dummy)[N]) {
        return dummy + N;
    }

    template<class Int, std::size_t N>
    [[nodiscard]]
    __device__
    Int* end(Int (&dummy)[N]) {
        return dummy + N;
    }

}

#endif //HARMONIA_GRID_SYNCHRONIZATION_HPP
