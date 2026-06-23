#include <fstream>

#include "absl/log/check.h"
#include "absl/status/status_matchers.h"
#include "absl/strings/string_view.h"
#include "gmock/gmock.h"
#include "google/protobuf/descriptor.pb.h"
#include "gtest/gtest.h"
#include "pbrt_proto/testing/proto_matchers.h"
#include "pbrt_proto/v1/defaults.pb.h"
#include "pbrt_proto/v1/v1.pb.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace pbrt_proto::v1 {
namespace {

using ::absl_testing::IsOkAndHolds;
using ::absl_testing::StatusIs;
using ::bazel::tools::cpp::runfiles::Runfiles;
using ::google::protobuf::Descriptor;
using ::google::protobuf::EqualsProto;
using ::google::protobuf::FieldDescriptor;
using ::google::protobuf::ParseTextOrDie;

class DefaultsTest : public testing::Test {
 public:
  DefaultsTest() {
    std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest());
    std::ifstream istream(
        runfiles->Rlocation("_main/pbrt_proto/v1/defaults.binpb"),
        std::ios::in | std::ios::binary);
    CHECK(defaults_.ParseFromIstream(&istream));
  }

  Defaults defaults_;
};

TEST_F(DefaultsTest, Global) {
  bool found_accelerator = false;
  bool found_camera = false;
  bool found_film = false;
  bool found_pixel_filter = false;
  bool found_sampler = false;
  bool found_surface_integrator = false;
  bool found_volume_integrator = false;
  for (auto& directive : defaults_.global_defaults().directives()) {
    if (directive.has_accelerator()) {
      EXPECT_NE(directive.accelerator().accelerator_type_case(),
                Accelerator::ACCELERATOR_TYPE_NOT_SET);
      EXPECT_FALSE(found_accelerator);
      found_accelerator = true;
    } else if (directive.has_camera()) {
      EXPECT_NE(directive.camera().camera_type_case(),
                Camera::CAMERA_TYPE_NOT_SET);
      EXPECT_FALSE(found_camera);
      found_camera = true;
    } else if (directive.has_film()) {
      EXPECT_NE(directive.film().film_type_case(), Film::FILM_TYPE_NOT_SET);
      EXPECT_FALSE(found_film);
      found_film = true;
    } else if (directive.has_pixel_filter()) {
      EXPECT_NE(directive.pixel_filter().pixel_filter_type_case(),
                PixelFilter::PIXEL_FILTER_TYPE_NOT_SET);
      EXPECT_FALSE(found_pixel_filter);
      found_pixel_filter = true;
    } else if (directive.has_sampler()) {
      EXPECT_NE(directive.sampler().sampler_type_case(),
                Sampler::SAMPLER_TYPE_NOT_SET);
      EXPECT_FALSE(found_sampler);
      found_sampler = true;
    } else if (directive.has_surface_integrator()) {
      EXPECT_NE(directive.surface_integrator().surface_integrator_type_case(),
                SurfaceIntegrator::SURFACE_INTEGRATOR_TYPE_NOT_SET);
      EXPECT_FALSE(found_surface_integrator);
      found_surface_integrator = true;
    } else if (directive.has_volume_integrator()) {
      EXPECT_NE(directive.volume_integrator().volume_integrator_type_case(),
                VolumeIntegrator::VOLUME_INTEGRATOR_TYPE_NOT_SET);
      EXPECT_FALSE(found_volume_integrator);
      found_volume_integrator = true;
    }
  }
  EXPECT_TRUE(found_accelerator);
  EXPECT_TRUE(found_camera);
  EXPECT_TRUE(found_film);
  EXPECT_TRUE(found_pixel_filter);
  EXPECT_TRUE(found_sampler);
  EXPECT_TRUE(found_surface_integrator);
  EXPECT_TRUE(found_volume_integrator);
}

TEST_F(DefaultsTest, DefaultMaterial) {
  EXPECT_NE(defaults_.default_material().material_type_case(),
            Material::MATERIAL_TYPE_NOT_SET);
}

void CheckField(const Descriptor& descriptor, const FieldDescriptor& field) {
  SCOPED_TRACE(field.name());

  bool found_something = false;
  for (int f = 0; f < descriptor.field_count(); f++) {
    const FieldDescriptor* field_descriptor = descriptor.field(f);
    if (!field_descriptor) {
      continue;
    }

    if (field_descriptor->name() != field.name()) {
      continue;
    }

    if (field_descriptor->is_map()) {
      for (int ff = 0; ff < field_descriptor->message_type()->field_count();
           ff++) {
        if (field_descriptor->message_type()->field(ff)->name() == "value") {
          field_descriptor = field_descriptor->message_type()->field(ff);
          break;
        }
      }
    }

    EXPECT_EQ(!!field_descriptor->message_type(), !!field.message_type());

    if (field_descriptor->message_type() && field.message_type()) {
      EXPECT_EQ(field_descriptor->message_type()->full_name(),
                field.message_type()->full_name());
    }

    found_something = true;
  }

  EXPECT_TRUE(found_something);
}

void CheckAllFields(const Descriptor* descriptor0,
                    const Descriptor* descriptor1) {
  for (int f = 0; f < descriptor0->field_count(); f++) {
    const FieldDescriptor* field_descriptor = descriptor0->field(f);
    if (!field_descriptor) {
      continue;
    }

    if (field_descriptor->name() == "name" ||
        field_descriptor->name() == "overrides") {
      continue;
    }

    CheckField(*descriptor1, *field_descriptor);
  }
}

TEST(Defaults, Accelerator) {
  CheckAllFields(Accelerator::GetDescriptor(),
                 Defaults::DefaultAccelerators::GetDescriptor());
}

TEST(Defaults, AreaLightSource) {
  CheckAllFields(AreaLightSource::GetDescriptor(),
                 Defaults::DefaultAreaLightSources::GetDescriptor());
}

TEST(Defaults, Film) {
  CheckAllFields(Film::GetDescriptor(),
                 Defaults::DefaultFilms::GetDescriptor());
}

TEST(Defaults, FloatTexture) {
  CheckAllFields(FloatTexture::GetDescriptor(),
                 Defaults::DefaultFloatTextures::GetDescriptor());
}

TEST(Defaults, LightSource) {
  CheckAllFields(LightSource::GetDescriptor(),
                 Defaults::DefaultLightSources::GetDescriptor());
}

TEST(Defaults, Material) {
  CheckAllFields(Material::GetDescriptor(),
                 Defaults::DefaultMaterials::GetDescriptor());
}

TEST(Defaults, PixelFilter) {
  CheckAllFields(PixelFilter::GetDescriptor(),
                 Defaults::DefaultPixelFilters::GetDescriptor());
}

TEST(Defaults, Sampler) {
  CheckAllFields(Sampler::GetDescriptor(),
                 Defaults::DefaultSamplers::GetDescriptor());
}

TEST(Defaults, Shape) {
  CheckAllFields(Shape::GetDescriptor(),
                 Defaults::DefaultShapes::GetDescriptor());
}

TEST(Defaults, SpectrumTexture) {
  CheckAllFields(SpectrumTexture::GetDescriptor(),
                 Defaults::DefaultSpectrumTextures::GetDescriptor());
}

TEST(Defaults, SurfaceIntegrator) {
  CheckAllFields(SurfaceIntegrator::GetDescriptor(),
                 Defaults::DefaultSurfaceIntegrators::GetDescriptor());
}

TEST(Defaults, Volume) {
  CheckAllFields(Volume::GetDescriptor(),
                 Defaults::DefaultVolumes::GetDescriptor());
}

TEST(Defaults, VolumeIntegrator) {
  CheckAllFields(VolumeIntegrator::GetDescriptor(),
                 Defaults::DefaultVolumeIntegrators::GetDescriptor());
}

}  // namespace
}  // namespace pbrt_proto::v1
