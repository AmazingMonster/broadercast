// Copyright 2025 Feng Mofan
// SPDX-License-Identifier: Apache-2.0

#ifndef BROADERCAST_APT_SIGNAL_H
#define BROADERCAST_APT_SIGNAL_H

#include "broadercast/signal.hpp"

namespace Broadercast
{

template<typename...Args>
using AptSignal = Signal<std::function, std::map>::Mold<Args...>;

}

#endif