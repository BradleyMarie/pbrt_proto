#ifndef _PBRT_PROTO_SHARED_FILMS_
#define _PBRT_PROTO_SHARED_FILMS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

absl::Status RemoveRgbFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, RgbFilm& output);

absl::Status RemoveGBufferFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, GBufferFilm& output);

absl::Status RemoveSpectralFilm(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    int pbrt_version, SpectralFilm& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_FILMS_
