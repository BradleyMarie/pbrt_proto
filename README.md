# PBRT Proto

[![Test Status](https://github.com/BradleyMarie/pbrt_proto/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/BradleyMarie/pbrt_proto/actions/workflows/tests.yml)
[![License](https://img.shields.io/badge/License-MPL--2.0-blue.svg)](https://github.com/BradleyMarie/libfbsdf/master/LICENSE)

When writing a renderer, two of the inevitable problems to be addressed are
finding scenes to work with and writing the logic to parse them into the
renderer. The models from the [PBRT](https://www.pbrt.org/) renderer are a
natural fit for this since they are freely licensed and have a structure that is
well-documented, text-based, and easy to understand. However, while these models
are easy to work with, the actual nuts and bolts of writing a parser for them is
both tricky and (from the perspective of a renderer developer) not particularly
interesting.

This library, PBRT Proto, is an attempt to solve this problem once and for all
by creating a projection of the PBRT format as a
[Protocol Buffer](https://protobuf.dev/) and then providing libraries and tools
that can be used to perform the conversion.

Protocol Buffers are a great for this because they allow the serialization to be
expressed in a way that is independent programming language and enable all of
the tedious logic of handling their serialization and deserialization to be
taken cared of by their code generators (which exist for all of the most
popular programming languages). Additionally, they support both binary and JSON
encodings and as a consequence of having been honed at Google for over a decade
the serializations produced are highly efficient (both in terms of the size of
the serialized data as well as their CPU and memory cost at runtime).

# Current Status

PBRT Proto has full support for the PBRT v3 inputs. Support for v1, v2, and v4
inputs is on the roadmap for the future; however, there is no immediate plan to
undertake that work.

Additionally, while PBRT Proto fully supports PBRT v3 inputs, their actual
Protocol Buffer projection is still a work in progress and is not stable.

# File Extension

PBRT Proto files use the following extension format. For JSON-encoded files, the
extension is `.pbrt.v#.txtpb`. For example for PBRT v3 models, it would be
`.pbrt.v3.txtpb`. For binary-encoded files, the pattern is the same but the
extension terminates with `.binpb`. For example, `.pbrt.v3.binpb` would be the
extension for converted v3 models.

# Principals

In designing the conversion logic and the Protocol Buffer projection, the
following roughly ordered set of principals was used to guide the process.

1) The converted model represents exactly the same scene before and after
   conversion.
2) The conversion may validate, clamp, drop data, and otherwise sanitize the
   PBRT input as long but only if those changes have no impact on the rendered
   output.
3) Conversion is only permitted to fail if PBRT would have failed to render,
   otherwise the conversion must succeed.
4) Validation annotations will be satisfied on all converted models, and renders
   should categorically reject PBRT Proto models with contents that do not
   satisfy them.
5) Protocol Buffers will be designed to minimize the amount of parsing logic
   that renderers will need to write. Any static data needed for parsing will
   be provided as a protocol buffer to ease portability across languages.
6) The structure of the Protocol Buffer projections should map as closely as
   possible to their PBRT equivalents; however, when the additional structure
   that Protocol Buffers provide enables the elimination of classes of errors it
   is permissible to do so.
