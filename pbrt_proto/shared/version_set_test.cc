#include "pbrt_proto/shared/version_set.h"

#include "gtest/gtest.h"

namespace pbrt_proto {
namespace {

TEST(VersionSet, TestAllBits) {
  VersionSet versions(~1ull);
  EXPECT_FALSE(versions.Supported(-1));
  EXPECT_FALSE(versions.Supported(0));
  EXPECT_TRUE(versions.Supported(1));
  EXPECT_TRUE(versions.Supported(2));
  EXPECT_TRUE(versions.Supported(3));
  EXPECT_TRUE(versions.Supported(4));
  EXPECT_FALSE(versions.Supported(5));
}

TEST(VersionSet, AsCpp) {
  VersionSet versions(~1ull);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b11110ull)", versions.AsCpp());
}

TEST(AllVersions, Test) {
  VersionSet versions = AllVersions();
  EXPECT_FALSE(versions.Supported(-1));
  EXPECT_FALSE(versions.Supported(0));
  EXPECT_TRUE(versions.Supported(1));
  EXPECT_TRUE(versions.Supported(2));
  EXPECT_TRUE(versions.Supported(3));
  EXPECT_TRUE(versions.Supported(4));
  EXPECT_FALSE(versions.Supported(5));
}

TEST(NoVersions, Test) {
  VersionSet versions = NoVersions();
  EXPECT_FALSE(versions.Supported(-1));
  EXPECT_FALSE(versions.Supported(0));
  EXPECT_FALSE(versions.Supported(1));
  EXPECT_FALSE(versions.Supported(2));
  EXPECT_FALSE(versions.Supported(3));
  EXPECT_FALSE(versions.Supported(4));
  EXPECT_FALSE(versions.Supported(5));
}

TEST(SupportedVersions, Test) {
  VersionSet versions = SupportedVersions({0, 1, 3, 5});
  EXPECT_EQ("::pbrt_proto::VersionSet(0b01010ull)", versions.AsCpp());
}

TEST(MinVersion, TooLow) {
  VersionSet versions = MinVersion(0);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b11110ull)", versions.AsCpp());
}

TEST(MinVersion, TooHigh) {
  VersionSet versions = MinVersion(5);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b00000ull)", versions.AsCpp());
}

TEST(MinVersion, Valid) {
  VersionSet versions = MinVersion(2);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b11100ull)", versions.AsCpp());
}

TEST(MaxVersion, TooLow) {
  VersionSet versions = MaxVersion(0);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b00000ull)", versions.AsCpp());
}

TEST(MaxVersion, TooHigh) {
  VersionSet versions = MaxVersion(5);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b11110ull)", versions.AsCpp());
}

TEST(MaxVersion, Valid) {
  VersionSet versions = MaxVersion(2);
  EXPECT_EQ("::pbrt_proto::VersionSet(0b00110ull)", versions.AsCpp());
}

}  // namespace
}  // namespace pbrt_proto
