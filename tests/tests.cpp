// Copyright 2024 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#include "tests/units/teller.ordinary.test.hpp"
#include "tests/units/teller.lvalue_reference.test.hpp"
#include "tests/units/signal.ordinary.test.hpp"
#include "tests/units/signal.lvalue_reference.test.hpp"

int main()
{
    //Broadercast::TestTellerOrdinary::test();
    //Broadercast::TestTellerLvalueReference::test();
    //Broadercast::TestSignalOrdinary::test();
    Broadercast::TestSignalLvalueReference::test();
    return 0; 
}