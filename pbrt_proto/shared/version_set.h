#ifndef _PBRT_PROTO_SHARED_VERSION_SET_
#define _PBRT_PROTO_SHARED_VERSION_SET_

#include <bitset>
#include <initializer_list>
#include <string>

namespace pbrt_proto {

class VersionSet {
 private:
  static constexpr int kMinVersion = 1;
  static constexpr int kMaxVersion = 4;
  static constexpr size_t kNumBits = kMaxVersion + 1;

 public:
  constexpr VersionSet(unsigned long long state) : state_(state & ~1ull) {}

  std::string AsCpp() const {
    return "::pbrt_proto::VersionSet(0b" + state_.to_string() + "ull)";
  }

  bool Supported(int version) const {
    if (version < kMinVersion || version > kMaxVersion) {
      return false;
    }

    return state_[version];
  }

 private:
  std::bitset<kNumBits> state_;
};

constexpr VersionSet AllVersions() { return VersionSet(~0ull); }

constexpr VersionSet NoVersions() { return VersionSet(0ull); }

constexpr VersionSet SupportedVersions(std::initializer_list<int> versions) {
  unsigned long long result = 0;
  for (int version : versions) {
    if (version >= 0 && version < 64) {
      result |= (1u << version);
    }
  }
  return VersionSet(result);
}

constexpr VersionSet MinVersion(int min_ver) {
  if (min_ver > 63) {
    return VersionSet(0ull);
  }

  if (min_ver <= 0) {
    return AllVersions();
  }

  return VersionSet(~((1ull << min_ver) - 1ull));
}

constexpr VersionSet MaxVersion(int max_ver) {
  if (max_ver <= 0) {
    return VersionSet(0ull);
  }

  if (max_ver > 63) {
    return AllVersions();
  }

  return VersionSet((1ull << (max_ver + 1)) - 1ull);
}

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_VERSION_SET_
