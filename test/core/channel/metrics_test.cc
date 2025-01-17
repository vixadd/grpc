// Copyright 2024 The gRPC Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/core/lib/channel/metrics.h"

#include <memory>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/match.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "test/core/util/fake_stats_plugin.h"
#include "test/core/util/test_config.h"

namespace grpc_core {
namespace {

class MetricsTest : public ::testing::Test {
 public:
  void TearDown() override {
    GlobalInstrumentsRegistryTestPeer::ResetGlobalInstrumentsRegistry();
    GlobalStatsPluginRegistryTestPeer::ResetGlobalStatsPluginRegistry();
  }
};

TEST_F(MetricsTest, UInt64Counter) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  auto uint64_counter_handle = GlobalInstrumentsRegistry::RegisterUInt64Counter(
      "uint64_counter", "A simple uint64 counter.", "unit", kLabelKeys,
      kOptionalLabelKeys, true);
  constexpr absl::string_view kLabelValues[] = {"label_value_1",
                                                "label_value_2"};
  constexpr absl::string_view kOptionalLabelValues[] = {
      "optional_label_value_1", "optional_label_value_2"};
  constexpr absl::string_view kDomain1To4 = "domain1.domain2.domain3.domain4";
  constexpr absl::string_view kDomain2To4 = "domain2.domain3.domain4";
  constexpr absl::string_view kDomain3To4 = "domain3.domain4";
  auto plugin1 = MakeStatsPluginForTarget(kDomain1To4);
  auto plugin2 = MakeStatsPluginForTarget(kDomain2To4);
  auto plugin3 = MakeStatsPluginForTarget(kDomain3To4);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain1To4, ""))
      .AddCounter(uint64_counter_handle, 1, kLabelValues, kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain2To4, ""))
      .AddCounter(uint64_counter_handle, 2, kLabelValues, kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain3To4, ""))
      .AddCounter(uint64_counter_handle, 3, kLabelValues, kOptionalLabelValues);
  EXPECT_THAT(plugin1->GetCounterValue(uint64_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(1));
  EXPECT_THAT(plugin2->GetCounterValue(uint64_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(3));
  EXPECT_THAT(plugin3->GetCounterValue(uint64_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(6));
}

TEST_F(MetricsTest, DoubleCounter) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  auto double_counter_handle = GlobalInstrumentsRegistry::RegisterDoubleCounter(
      "double_counter", "A simple double counter.", "unit", kLabelKeys,
      kOptionalLabelKeys, true);
  constexpr absl::string_view kLabelValues[] = {"label_value_1",
                                                "label_value_2"};
  constexpr absl::string_view kOptionalLabelValues[] = {
      "optional_label_value_1", "optional_label_value_2"};
  constexpr absl::string_view kDomain1To4 = "domain1.domain2.domain3.domain4";
  constexpr absl::string_view kDomain2To4 = "domain2.domain3.domain4";
  constexpr absl::string_view kDomain3To4 = "domain3.domain4";
  auto plugin1 = MakeStatsPluginForTarget(kDomain1To4);
  auto plugin2 = MakeStatsPluginForTarget(kDomain2To4);
  auto plugin3 = MakeStatsPluginForTarget(kDomain3To4);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain1To4, ""))
      .AddCounter(double_counter_handle, 1.23, kLabelValues,
                  kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain2To4, ""))
      .AddCounter(double_counter_handle, 2.34, kLabelValues,
                  kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain3To4, ""))
      .AddCounter(double_counter_handle, 3.45, kLabelValues,
                  kOptionalLabelValues);
  EXPECT_THAT(plugin1->GetCounterValue(double_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(1.23));
  EXPECT_THAT(plugin2->GetCounterValue(double_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(3.57));
  EXPECT_THAT(plugin3->GetCounterValue(double_counter_handle, kLabelValues,
                                       kOptionalLabelValues),
              ::testing::Optional(7.02));
}

TEST_F(MetricsTest, UInt64Histogram) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  auto uint64_histogram_handle =
      GlobalInstrumentsRegistry::RegisterUInt64Histogram(
          "uint64_histogram", "A simple uint64 histogram.", "unit", kLabelKeys,
          kOptionalLabelKeys, true);
  constexpr absl::string_view kLabelValues[] = {"label_value_1",
                                                "label_value_2"};
  constexpr absl::string_view kOptionalLabelValues[] = {
      "optional_label_value_1", "optional_label_value_2"};
  constexpr absl::string_view kDomain1To4 = "domain1.domain2.domain3.domain4";
  constexpr absl::string_view kDomain2To4 = "domain2.domain3.domain4";
  constexpr absl::string_view kDomain3To4 = "domain3.domain4";
  auto plugin1 = MakeStatsPluginForTarget(kDomain1To4);
  auto plugin2 = MakeStatsPluginForTarget(kDomain2To4);
  auto plugin3 = MakeStatsPluginForTarget(kDomain3To4);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain1To4, ""))
      .RecordHistogram(uint64_histogram_handle, 1, kLabelValues,
                       kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain2To4, ""))
      .RecordHistogram(uint64_histogram_handle, 2, kLabelValues,
                       kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain3To4, ""))
      .RecordHistogram(uint64_histogram_handle, 3, kLabelValues,
                       kOptionalLabelValues);
  EXPECT_THAT(plugin1->GetHistogramValue(uint64_histogram_handle, kLabelValues,
                                         kOptionalLabelValues),
              ::testing::Optional(::testing::UnorderedElementsAre(1)));
  EXPECT_THAT(plugin2->GetHistogramValue(uint64_histogram_handle, kLabelValues,
                                         kOptionalLabelValues),
              ::testing::Optional(::testing::UnorderedElementsAre(1, 2)));
  EXPECT_THAT(plugin3->GetHistogramValue(uint64_histogram_handle, kLabelValues,
                                         kOptionalLabelValues),
              ::testing::Optional(::testing::UnorderedElementsAre(1, 2, 3)));
}

TEST_F(MetricsTest, DoubleHistogram) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  auto double_histogram_handle =
      GlobalInstrumentsRegistry::RegisterDoubleHistogram(
          "double_histogram", "A simple double histogram.", "unit", kLabelKeys,
          kOptionalLabelKeys, true);
  constexpr absl::string_view kLabelValues[] = {"label_value_1",
                                                "label_value_2"};
  constexpr absl::string_view kOptionalLabelValues[] = {
      "optional_label_value_1", "optional_label_value_2"};
  constexpr absl::string_view kDomain1To4 = "domain1.domain2.domain3.domain4";
  constexpr absl::string_view kDomain2To4 = "domain2.domain3.domain4";
  constexpr absl::string_view kDomain3To4 = "domain3.domain4";
  auto plugin1 = MakeStatsPluginForTarget(kDomain1To4);
  auto plugin2 = MakeStatsPluginForTarget(kDomain2To4);
  auto plugin3 = MakeStatsPluginForTarget(kDomain3To4);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain1To4, ""))
      .RecordHistogram(double_histogram_handle, 1.23, kLabelValues,
                       kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain2To4, ""))
      .RecordHistogram(double_histogram_handle, 2.34, kLabelValues,
                       kOptionalLabelValues);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain3To4, ""))
      .RecordHistogram(double_histogram_handle, 3.45, kLabelValues,
                       kOptionalLabelValues);
  EXPECT_THAT(plugin1->GetHistogramValue(double_histogram_handle, kLabelValues,
                                         kOptionalLabelValues),
              ::testing::Optional(::testing::UnorderedElementsAre(1.23)));
  EXPECT_THAT(plugin2->GetHistogramValue(double_histogram_handle, kLabelValues,
                                         kOptionalLabelValues),
              ::testing::Optional(::testing::UnorderedElementsAre(1.23, 2.34)));
  EXPECT_THAT(
      plugin3->GetHistogramValue(double_histogram_handle, kLabelValues,
                                 kOptionalLabelValues),
      ::testing::Optional(::testing::UnorderedElementsAre(1.23, 2.34, 3.45)));
}

TEST_F(MetricsTest, DisableByDefaultMetricIsNotRecordedByFakeStatsPlugin) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  auto double_histogram_handle =
      GlobalInstrumentsRegistry::RegisterDoubleHistogram(
          "double_histogram", "A simple double histogram.", "unit", kLabelKeys,
          kOptionalLabelKeys, /*enable_by_default=*/false);
  constexpr absl::string_view kLabelValues[] = {"label_value_1",
                                                "label_value_2"};
  constexpr absl::string_view kOptionalLabelValues[] = {
      "optional_label_value_1", "optional_label_value_2"};
  constexpr absl::string_view kDomain1To4 = "domain1.domain2.domain3.domain4";
  auto plugin = MakeStatsPluginForTarget(kDomain1To4);
  GlobalStatsPluginRegistry::GetStatsPluginsForChannel(
      StatsPlugin::ChannelScope(kDomain1To4, ""))
      .RecordHistogram(double_histogram_handle, 1.23, kLabelValues,
                       kOptionalLabelValues);
  EXPECT_EQ(plugin->GetHistogramValue(double_histogram_handle, kLabelValues,
                                      kOptionalLabelValues),
            absl::nullopt);
}

using MetricsDeathTest = MetricsTest;

TEST_F(MetricsDeathTest, RegisterTheSameMetricNameWouldCrash) {
  const absl::string_view kLabelKeys[] = {"label_key_1", "label_key_2"};
  const absl::string_view kOptionalLabelKeys[] = {"optional_label_key_1",
                                                  "optional_label_key_2"};
  (void)GlobalInstrumentsRegistry::RegisterDoubleHistogram(
      "double_histogram", "A simple double histogram.", "unit", kLabelKeys,
      kOptionalLabelKeys, true);
  EXPECT_DEATH(GlobalInstrumentsRegistry::RegisterDoubleHistogram(
                   "double_histogram", "A simple double histogram.", "unit",
                   kLabelKeys, kOptionalLabelKeys, true),
               "Metric name double_histogram has already been registered.");
}

}  // namespace
}  // namespace grpc_core

int main(int argc, char** argv) {
  grpc::testing::TestEnvironment env(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}
