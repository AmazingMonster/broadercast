// Copyright 2025 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_CUE_DECIPHER_H
#define BROADERCAST_CUE_DECIPHER_H

#include <memory>
#include <type_traits>
#include "conceptrodon/mouldivore/concepts/confess.hpp"
#include "conceptrodon/mouldivore/concepts/deceive.hpp"

namespace Broadercast {
namespace Cue {

template<typename Supposed>
struct Decipher
{
    template<typename...Args>
    Decipher(Args...) {}

    bool isForwardSafe()
    { return true; }
};

template<typename Supposed>
requires Conceptrodon::Mouldivore::Deceive<std::is_lvalue_reference, Supposed>
&&  Conceptrodon::Mouldivore::Deceive<std::is_rvalue_reference, Supposed>
struct Decipher<Supposed>
{
    struct ArgumentConcept
    {
        virtual bool isForwardSafe() = 0;
    virtual ~ArgumentConcept() = default;
    };

    template<typename Provided>
    struct Argument: public ArgumentConcept
    {
        bool isForwardSafe() override
        { return true; }
    };

    template<typename Provided>
    requires Conceptrodon::Mouldivore::Confess<std::is_lvalue_reference, Provided>
    struct Argument<Provided>: public ArgumentConcept
    {
        bool isForwardSafe() override
        { return false; }
    };
    
    template<typename Provided>
    Decipher(std::type_identity<Provided>)
    : is_forward_safe {std::make_unique<Argument<Provided>>(Argument<Provided>{})} {}

    bool isForwardSafe()
    { return is_forward_safe -> isForwardSafe(); }

    std::unique_ptr<ArgumentConcept> is_forward_safe;
};

}}

#endif