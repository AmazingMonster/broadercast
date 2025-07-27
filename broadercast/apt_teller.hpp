// Copyright 2025 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_APT_TELLER_H
#define BROADERCAST_APT_TELLER_H

#include "broadercast/teller.hpp"

namespace Broadercast
{

template<typename...Args>
using AptTeller = Teller<std::function, std::vector>::Mold<Args...>;

}

#endif