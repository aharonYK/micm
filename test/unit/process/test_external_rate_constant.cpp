#include <gtest/gtest.h>

#include <micm/process/external_rate_constant.hpp>

TEST(ExternalRateConstant, DefaultConstructor)
{
  micm::ExternalRateConstant external{};
}

TEST(ExternalRateConstant, CalculateWithSystem)
{
  micm::ExternalRateConstant external{};
  auto k = external.calculate(micm::System());
  std::cout << k << std::endl;
}