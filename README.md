# PBRT Proto

[![Test Status](https://github.com/BradleyMarie/pbrt_proto/actions/workflows/tests.yml/badge.svg?branch=main)](https://github.com/BradleyMarie/pbrt_proto/actions/workflows/tests.yml)
[![License](https://img.shields.io/badge/License-MPL--2.0-blue.svg)](https://github.com/BradleyMarie/libfbsdf/master/LICENSE)
[![Made by Humans](https://img.shields.io/badge/Made%20by%20Humans-gray.svg)](#)

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

PBRT Proto has full support for the PBRT v3 inputs. Some work has been done to
support v1, v2, and v4; however, there is no immediate plan to complete their
implementation.

Additionally, while PBRT Proto fully supports PBRT v3 inputs, their actual
Protocol Buffer projection is still a work in progress and is not stable.

# File Extension

PBRT Proto files use the following extension format. For JSON-encoded files, the
extension is `.pbrt.v#.txtpb`. For example for PBRT v3 models, it would be
`.pbrt.v3.txtpb`. For binary-encoded files, the pattern is the same but the
extension terminates with `.binpb`. For example, `.pbrt.v3.binpb` would be the
extension for converted v3 models.

# Defaults

Do not rely on the default values defined in `pbrt.proto` or the default
instance of any directive to correctly represent the default state of that
directive since the default value for any directive can vary by PBRT version.
They may be correct; however, they should be considered informational at best
and may be removed in future pbrt_proto versions. The only supported way to get
a directive in its default state is to use the `defaults.txtpb` and
`defaults.binpb` files included with each supported PBRT version.

One notable pseudo-exception involves any directive containing a
`MeasuredScatteringPreset` field. For these directives, the default state
depends on the specific value chosen in that field. Accordingly, the
`defaults.txtpb` and `defaults.binpb` files provide multiple default
configurations for each directive, organized as a map keyed by the
`MeasuredScatteringPreset` values. Because these fields are guaranteed to always
initialize to `DEFAULT`, a user-provided directive containing a
`MeasuredScatteringPreset` can be used directly to look up its corresponding
default value in the defaults map using the value of the
`MeasuredScatteringPreset` as the key.

# Principles

In designing the conversion logic and the Protocol Buffer projection, the
following roughly ordered set of principles was used to guide the process.

## Protobuf Projection

1) If the input model is syntactically valid, the converted model should be
   represented perfectly; however, the exact data types used to describe the
   scene may differ from the types used in PBRT as long as there is no loss of
   data on the valid range of inputs.
2) Protocol Buffers will be designed to minimize the amount of parsing logic
   that renderers will need to write. Any static data needed for parsing will
   be provided as a protocol buffer to ease portability across languages.
3) The structure of the Protocol Buffer projections should map as closely as
   possible to their PBRT equivalents; however, when the additional structure
   that Protocol Buffers provide enables the elimination of classes of errors it
   is permissible to do so.

## Conversion Process

1) The conversion may validate, clamp, drop data, and otherwise sanitize the
   input as long as those changes have no impact on the rendered output.
2) The conversion is only permitted to fail if PBRT would have failed to render,
   otherwise the conversion must succeed.
3) The conversion only is required to support the range of inputs supported by
   PBRT even if the Protocol Buffers use types with larger ranges.

# Expected Incompatibilities

1) In PBRT Proto, the `Include` directive works like the `Import` directive in
   that it cannot be used in the middle of a different directive and that the
   included files must start and end with complete directives. This means that
   some of the the volumetric example scenes from PBRTv1, PBRTv2, and PBRTv3 are
   incompatible with PBRT Proto without modification.
