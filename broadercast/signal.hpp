// Copyright 2025 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_SIGNAL_H
#define BROADERCAST_SIGNAL_H

#include "conceptrodon/functivore/concepts/member_function_pointer_probe.hpp"
#include "conceptrodon/mouldivore/concepts/confess.hpp"
#include "conceptrodon/mouldivore/concepts/deceive.hpp"
#include "conceptrodon/functivore/apply_return_type.hpp"
#include <concepts>
#include <functional>
#include <type_traits>
#include <map>
#include <utility>

namespace Broadercast {

template
<
    template<typename...> class FunctionWrapper=std::function,
    template<typename...> class TheMap=std::map
>
struct Signal
{
    template<typename...Parameters>
    struct ProtoMold
    {
        using TypeSignature = Conceptrodon::Functivore::ApplyReturnType<void>::Mold<Parameters...>;
        using Function = FunctionWrapper<Conceptrodon::Functivore::ApplyReturnType<void>::Mold<std::remove_reference_t<Parameters>&...>>;
        using Map = TheMap<size_t, Function>;

        ProtoMold(): counter{0} {}

        template <typename Execute>
        requires std::invocable<Execute, Parameters...>
        size_t connect(Execute&& exec)
        {
            counter++;
            map.emplace(counter, wrap(std::forward<Execute>(exec)));
            return counter;
        }

        template <typename ObjectPointer, typename Execute>
        size_t connect(ObjectPointer&& object_pointer, Execute&& exec)
        {
            counter++;
            map.emplace(counter++, wrap(std::forward<ObjectPointer>(object_pointer), std::forward<Execute>(exec)));
            return counter;
        }

        auto disconnect(size_t key)
        { return map.erase(key); }

        template <typename Activate>
        Function wrap(Activate&& activate)
        {
            return [activate, this]
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                activate(args...);
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
                (std::remove_reference_t<Parameters>&...args) mutable -> void
                {
                    activate(args...);
                    return;
                };
            }
    
            else
            {
                return [activate=std::move(activate), this]
                (std::remove_reference_t<Parameters>&...args) mutable -> void
                {
                    activate(args...);
                    return;
                };
            }
        }
        
        template <typename Counter>
        Function wrap(std::nullptr_t)
        {
            return [this]
            (std::remove_reference_t<Parameters>&...) mutable -> void
            { return; };
        }
            
        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Confess<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer, this]
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                (object_pointer ->* activate)(args...);
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
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                (object_pointer ->* activate)(args...);
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
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                (object_pointer.get() ->* activate)(args...);
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
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                (object_pointer ->* activate)(args...);
                return;
            };
        }

        template <typename Activate, typename ObjectPointer>
        requires Conceptrodon::Functivore::MemberFunctionPointerProbe<Activate>
        && Conceptrodon::Mouldivore::Deceive<std::is_pointer, std::remove_cvref_t<ObjectPointer>>
        Function wrap(ObjectPointer&& object_pointer, Activate&& activate)
        {
            return [activate, object_pointer = std::move(object_pointer), this]
            (std::remove_reference_t<Parameters>&...args) mutable -> void
            {
                (object_pointer.get() ->* activate)(args...);
                return;
            };
        }

        template<typename...Args>
        requires std::invocable<TypeSignature, Args...>
        void execute(Args&&...args)
        {
            for (auto& slot : map)
            {
                slot.second(args...);
            }
        }
        
        size_t counter;
        Map map;
    };

    template<typename...Args>
    using Mold = ProtoMold<Args...>;
};

}

#endif