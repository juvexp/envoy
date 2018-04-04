#ifndef GFE_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_
#define GFE_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// TODO: Implement QuicFlagSaverImpl.
class QuicFlagSaverImpl {};

using QuicTestImpl = ::testing::Test;
template <class T>
using QuicTestWithParamImpl = ::testing::TestWithParam<T>;

#endif  // GFE_QUIC_PLATFORM_IMPL_QUIC_TEST_IMPL_H_
