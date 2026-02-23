#ifndef _PBRT_PROTO_SHARED_FILMS_
#define _PBRT_PROTO_SHARED_FILMS_

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "pbrt_proto/pbrt.pb.h"
#include "pbrt_proto/shared/parser.h"

namespace pbrt_proto {

void RemoveRgbFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output);

void RemoveRgbFilmV2(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output);

void RemoveRgbFilmV3(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output);

absl::Status RemoveRgbFilmV4(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    RgbFilm& output);

absl::Status RemoveGBufferFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    GBufferFilm& output);

absl::Status RemoveSpectralFilmV1(
    absl::flat_hash_map<absl::string_view, Parameter>& parameters,
    SpectralFilm& output);

}  // namespace pbrt_proto

#endif  // _PBRT_PROTO_SHARED_FILMS_
