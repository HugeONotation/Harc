#ifndef HARMONIA_INLINING_HPP
#define HARMONIA_INLINING_HPP

#include <harc/ir/IR.hpp>

namespace harc::ir {

    ///
    /// \param caller Function performing the call
    /// \param callee Candidate for inlining
    /// \return true if function should be inlined, false otherwise
    [[nodiscard]]
    bool should_inline(const Function& caller, const Function& callee);

}

#endif //HARMONIA_INLINING_HPP
