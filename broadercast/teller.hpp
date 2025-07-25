// Copyright 2025 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_TELLER_H
#define BROADERCAST_TELLER_H

#include "cue/decipher.hpp"
#include "conceptrodon/functivore/concepts/member_function_pointer_probe.hpp"
#include "conceptrodon/mouldivore/concepts/confess.hpp"
#include "conceptrodon/mouldivore/concepts/deceive.hpp"
#include "conceptrodon/functivore/apply_return_type.hpp"
#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>
#include "broadercast/cue/decipher.hpp"

namespace Broadercast {

template
<
    template<typename...> class FunctionWrapper=std::function,
    template<typename...> class TheVector=std::vector
>
struct Teller
{
    template<typename...Parameters>
    struct ProtoMold
    {
        using TypeSignature = Conceptrodon::Functivore::ApplyReturnType<void>::Mold<Parameters...>;
        using Function = FunctionWrapper<Conceptrodon::Functivore::ApplyReturnType<void>::Mold<std::remove_reference_t<Parameters>&..., Cue::Decipher<Parameters>...>>;
        using Vector = TheVector<Function>;

        template <typename Execute>
        requires std::invocable<Execute, Parameters...>
        void connect(Execute&& exec)
        { vector.emplace_back(wrap(std::forward<Execute>(exec))); }

        template <typename ObjectPointer, typename Execute>
        void connect(ObjectPointer&& object_pointer, Execute&& exec)
        { vector.emplace_back(wrap(std::forward<ObjectPointer>(object_pointer), std::forward<Execute>(exec))); }

        template <typename Activate>
        Function wrap(Activate&& activate)
        {
            return [activate, this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                activate(activate((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...));
                return;
            };
        }
        
        template <typename Activate>
        requires Conceptrodon::Mouldivore::Confess<std::is_class, std::remove_cvref_t<Activate>>
        Function wrap(Activate&& activate)
        {
            if constexpr (std::is_lvalue_reference_v<Activate>)
            {
                return [activate, this]
                (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
                {
                    activate((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                    return;
                };
            }
    
            else
            {
                return [activate=std::move(activate), this]
                (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
                {
                    activate((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                    return;
                };
            }
        }
        
        template <typename Counter>
        Function wrap(std::nullptr_t)
        {
            return [this]
            (std::remove_reference_t<Parameters>&..., Cue::Decipher<Parameters>...deciphers) mutable -> void
            { return; };
        }
            
        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Confess<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer, this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                (object_pointer ->* activate)((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                return;
            };
        }

        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Deceive<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        && std::invocable<decltype(std::declval<ObjectPointer>().operator->*(std::declval<Activate>())), Parameters...>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer = std::move(object_pointer), this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                (object_pointer ->* activate)((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                return;
            };
        }

        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Deceive<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        && Conceptrodon::Mouldivore::Confess<std::is_lvalue_reference, ObjectPointer>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer, this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                (object_pointer.get() ->* activate)((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                return;
            };
        }

        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Deceive<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        && std::invocable<decltype(std::declval<ObjectPointer>().operator->*(std::declval<Activate>())), Parameters...>
        && Conceptrodon::Mouldivore::Confess<std::is_lvalue_reference, ObjectPointer>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer, this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                (object_pointer ->* activate)((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                return;
            };
        }

        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Deceive<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer = std::move(object_pointer), this]
            (std::remove_reference_t<Parameters>&...args, Cue::Decipher<Parameters>...deciphers) mutable -> void
            {
                (object_pointer.get() ->* activate)((deciphers.isForwardSafe() ? std::forward<Parameters>(args) : args)...);
                return;
            };
        }

        template<typename...Args>
        requires std::invocable<TypeSignature, Args...>
        void execute(Args&&...args)
        {
            for (auto& slot : vector)
            {
                slot(args..., Cue::Decipher<Parameters>{std::type_identity<Args>{}}...);
            }
        }
        
        Vector vector;
    };
};

}

#endif