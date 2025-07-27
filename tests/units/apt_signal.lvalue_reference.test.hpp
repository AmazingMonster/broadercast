// Copyright 2024 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_TESTS_UNIT_APT_SIGNAL_LVALUE_REFERENCE_H
#define BROADERCAST_TESTS_UNIT_APT_SIGNAL_LVALUE_REFERENCE_H

#include "broadercast/apt_signal.hpp"
#include <iostream>
#include <memory>
#include <ostream>


namespace Broadercast {
namespace TestAptSignalLvalueReference {
    

struct Argument
{
    Argument() = default;

    Argument(Argument const & argument)
    { std::cout << "Argument copy constructed" << std::endl; }
    
    Argument(Argument const && argument)
    { std::cout << "Argument move constructed" << std::endl; }
};

struct Caller
{
    Caller() = default;

    Caller(Caller const & caller)
    { std::cout << "Caller copy constructed" << std::endl; }
    
    Caller(Caller const && caller)
    { std::cout << "Caller move constructed" << std::endl; }

    bool operator()(Argument const & p, bool r)
    {    
        std::cout << "Function object called" << std::endl;
        return r;
    }

    bool fun(Argument const & p, bool r)
    { 
        std::cout << "Pointer to member function called" << std::endl;
        return r; 
    }
};

template<typename C>
struct CallerPtr
{
    CallerPtr(C* the_c): c{the_c} {}
    
    template<typename R, typename...Ps>
    std::function<R(Ps...)> operator->*(R(C::*fun)(Ps...))
    {
        return [fun, this](Ps...ps) -> R
        {
            return (c->*fun)(ps...);
        };
    }

    C& operator*() { return *c; }

    C* c;
};

inline bool fun(Argument const & p, bool r)
{ 
    std::cout << "Function called" << std::endl;
    return r; 
}

inline auto Lambda
{
    [](Argument const & p, bool r)
    { 
        std::cout << "Lambda called" << std::endl;
        return r; 
    }
};

inline void test()
{
    AptSignal<Argument const &, bool> signal {};

    Caller caller{};
    Caller* c_ptr {&caller};
    std::shared_ptr<Caller> smart_c_ptr {std::make_shared<Caller>()};
    CallerPtr<Caller> caller_ptr{&caller};

    static_assert(std::invocable<decltype(std::declval<CallerPtr<Caller>>().operator->*(std::declval<decltype(&Caller::fun)>())), Argument, bool>);

    signal.connect(Caller{});
    signal.connect(caller);
    signal.connect(fun);
    signal.connect(Lambda);
    signal.connect(std::make_shared<Caller>(), &Caller::fun);
    signal.connect(smart_c_ptr, &Caller::fun);
    signal.connect(&caller, &Caller::fun);
    signal.connect(c_ptr, &Caller::fun);
    signal.connect(CallerPtr<Caller>{&caller}, &Caller::fun);
    signal.connect(caller_ptr, &Caller::fun);
    
    std::map<int, std::string> instruction
    {
        {1, "Rvalue function object"},
        {2, "Lvalue function object"},
        {3, "Function"},
        {4, "Lambda"},
        {5, "Rvalue smart pointer to object and pointer to member function"},
        {6, "Lvalue smart pointer to object and pointer to member function"},
        {7, "Rvalue pointer to object and pointer to member function"},
        {8, "Lvalue pointer to object and pointer to member function"},
        {9, "Rvalue pointer to object like and pointer to member function"},
        {10, "Lvalue pointer to object like and pointer to member function"}
    };

    while (true)
    {
        Argument a {};

        char v;

        for (auto const & pair : instruction)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }


        std::cin >> v;
        if (v == 'L' || v == 'l')
        {
            std::cout << "Lvalue argument" << std::endl;
            signal.execute(a, true);
        }
        else if (v == 'P' || v == 'p')
        {
            std::cout << "Prvalue argument" << std::endl;
            signal.execute(Argument{}, true);
        }
        else if (v == 'R' || v == 'r')
        {
            std::cout << "Rvalue reference argument" << std::endl;
            signal.execute(std::move(a), true);
        }
        else if (v == 'E' || v == 'e')
        {
            int x;
            std::cin >> x;
            signal.disconnect(x);
            instruction.erase(x);
        } 
    }
}

}}

#endif