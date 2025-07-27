// Copyright 2024 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#include "tests/units/apt_teller.ordinary.test.hpp"
#include "tests/units/apt_teller.lvalue_reference.test.hpp"
#include "tests/units/apt_signal.ordinary.test.hpp"
#include "tests/units/apt_signal.lvalue_reference.test.hpp"

int main()
{
    //Broadercast::TestAptTellerOrdinary::test();
    //Broadercast::TestAptTellerLvalueReference::test();
    //Broadercast::TestAptSignalOrdinary::test();
    Broadercast::TestAptSignalLvalueReference::test();
    return 0; 
}