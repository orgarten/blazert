//
// Created by ogarten on 14/05/2020.
//

#include <blazert/embree/primitives/EmbreePlane.h>
#include "../catch.hpp"
#include "../test_helpers.h"

using namespace blazert;

TEMPLATE_TEST_CASE("EmbreePlane", "[Bounding Box, Intersections]", float) {

  const float d1 = 2.f;
  const float d2 = 2.f;

  auto D = rtcNewDevice("verbose=0,start_threads=1,threads=4,set_affinity=1");
  auto S = rtcNewScene(D);

  SECTION("bounding box") {
    SECTION("center at origin") {
      Vec3r<TestType> center{0.f, 0.f, 0.f};
      SECTION("non-rotated") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        // plane z = 0, -2<x,y<2
        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(-1.f));
        REQUIRE(bound.lower_y == Approx(-1.f));
        REQUIRE(bound.lower_z == Approx(-std::numeric_limits<float>::min()));
        REQUIRE(bound.upper_x == Approx(1.f));
        REQUIRE(bound.upper_y == Approx(1.f));
        REQUIRE(bound.upper_z == Approx(std::numeric_limits<float>::min()));
      }
      SECTION("rotated about y-axis") {
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        Mat3r<TestType> rot{{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}};

        // defines plane x=0
        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(-std::numeric_limits<float>::min()));
        REQUIRE(bound.lower_y == Approx(-1.f));
        REQUIRE(bound.lower_z == Approx(-1.f));
        REQUIRE(bound.upper_x == Approx(std::numeric_limits<float>::min()));
        REQUIRE(bound.upper_y == Approx(1.f));
        REQUIRE(bound.upper_z == Approx(1.f));
      }
      SECTION("rotated about r=normalized(1,1,0)") {
        // plane on z=0, later rotated to x = 0
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        const Vec3r<TestType> axis{static_cast<TestType>(1 /  std::sqrt(2)), static_cast<TestType>(1 /  std::sqrt(2)), 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(-1.f));
        REQUIRE(bound.lower_y == Approx(-1.f));
        REQUIRE(bound.lower_z == Approx(-std::sqrt(2)));
        REQUIRE(bound.upper_x == Approx(1.f));
        REQUIRE(bound.upper_y == Approx(1.f));
        REQUIRE(bound.upper_z == Approx(std::sqrt(2)));
      }
      SECTION("rotated in xy-plane") {
        const Vec3r<TestType> axis{0, 0, 1};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 4);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(-std::sqrt(2)));
        REQUIRE(bound.lower_y == Approx(-std::sqrt(2)));
        REQUIRE(bound.lower_z == Approx(0).margin(0.0000001));
        REQUIRE(bound.upper_x == Approx(std::sqrt(2)));
        REQUIRE(bound.upper_y == Approx(std::sqrt(2)));
        REQUIRE(bound.upper_z == Approx(0).margin(0.0000001));
      }
    }
    SECTION("shifted center") {
      Vec3r<TestType> center{1.f, 1.f, 1.f};
      SECTION("non-rotated") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        // plane z = 0, -2<x,y<2
        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(0.f));
        REQUIRE(bound.lower_y == Approx(0.f));
        REQUIRE(bound.lower_z == Approx(1.f));
        REQUIRE(bound.upper_x == Approx(2.f));
        REQUIRE(bound.upper_y == Approx(2.f));
        REQUIRE(bound.upper_z == Approx(1.f));
      }
      SECTION("rotated about y-axis") {
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        Mat3r<TestType> rot{{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}};

        // defines plane x=0
        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(1.f));
        REQUIRE(bound.lower_y == Approx(0.f));
        REQUIRE(bound.lower_z == Approx(0.f));
        REQUIRE(bound.upper_x == Approx(1.f));
        REQUIRE(bound.upper_y == Approx(2.f));
        REQUIRE(bound.upper_z == Approx(2.f));
      }
      SECTION("rotated about r=normalized(1,1,0)") {
        // plane on z=0, later rotated to x = 0
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        const Vec3r<TestType> axis{static_cast<TestType>(1 /  std::sqrt(2)), static_cast<TestType>(1 /  std::sqrt(2)), 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(0).margin(std::numeric_limits<TestType>::epsilon()));
        REQUIRE(bound.lower_y == Approx(0).margin(std::numeric_limits<TestType>::epsilon()));
        REQUIRE(bound.lower_z == Approx(1 -  std::sqrt(2)));
        REQUIRE(bound.upper_x == Approx(2));
        REQUIRE(bound.upper_y == Approx(2));
        REQUIRE(bound.upper_z == Approx(1 +  std::sqrt(2)));
      }
      SECTION("rotated in xy-plane") {
        const Vec3r<TestType> axis{0, 0, 1};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 4);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCBounds bound;
        rtcGetSceneBounds(S, &bound);

        REQUIRE(bound.lower_x == Approx(1 - std::sqrt(2)));
        REQUIRE(bound.lower_y == Approx(1 - std::sqrt(2)));
        REQUIRE(bound.lower_z == Approx(1));
        REQUIRE(bound.upper_x == Approx(1 + std::sqrt(2)));
        REQUIRE(bound.upper_y == Approx(1 + std::sqrt(2)));
        REQUIRE(bound.upper_z == Approx(1));
      }
    }
  }
  SECTION("INTERSECTS") {
    SECTION("center at origin") {
      Vec3r<TestType> center{0.f, 0.f, 0.f};
      SECTION("non-rotated"){
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{0.f, 0.f, 5.f};
        Vec3r<TestType> dir1{0.f, 0.f, -1.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        REQUIRE(rayhit1.ray.tfar == Approx(5));
        REQUIRE(rayhit1.hit.Ng_x == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_z == Approx(1.f));
      }
      SECTION("rotated about (0,1,0)") {
        // matrix which rotates the plane 45 degrees about the z-axis ( x = 0 is now
        // plane eq)
        const Vec3r<TestType> axis{0, 1, 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{5.f, 0.f, 0.f};
        Vec3r<TestType> dir1{-1.f, 0.f, 0.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        REQUIRE(rayhit1.ray.tfar == Approx(5));
        REQUIRE(rayhit1.hit.Ng_x == Approx(1.f));
        REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_z == Approx(0.f).margin(0.0001));
      }
      SECTION("rotated about normalized(1,1,0), edge hit") {
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        const Vec3r<TestType> axis{static_cast<TestType>(1 / std::sqrt(2)), static_cast<TestType>(1 / std::sqrt(2)), 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{-5.f, 5.f, 0.5f};
        Vec3r<TestType> dir1{1.f, -1.f, 0.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        // hits from the negative x direction -> normal vector should point towards
        // that direction
        REQUIRE(rayhit1.ray.tfar == Approx(5 * std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_x == Approx(-1 / std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_y == Approx(1 / std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_z == Approx(0.f).margin(0.0001));// for rotated planes, you can expect small numerical
        // error of size 1e16
      }
      SECTION("non-rotated, ray origin outside plane boundaries") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        SECTION("outside on positive x-axis") {
          Vec3r<TestType> org1{4.f, 0.f, 4.f};
          Vec3r<TestType> dir1{-1.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(4 * std::sqrt(2.f)));
          REQUIRE(rayhit1.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f));
        }
        SECTION("outside on positive x-axis") {
          Vec3r<TestType> org2{-4.f, 0.f, -4.f};
          Vec3r<TestType> dir2{1.f, 0.f, 1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};
          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(4. * std::sqrt(2.f)));

          REQUIRE(rayhit2.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_z == Approx(-1.f));
        }
      }
      SECTION("non-rotated, edge intersection") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        SECTION("edge: x max") {
          // edge at x_max
          Vec3r<TestType> org1{1.f, 0.f, 4.f};
          Vec3r<TestType> dir1{0.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(4));
          REQUIRE(rayhit1.hit.Ng_x == Approx(1.f / std::sqrt(2)));
          REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: x min") {
          // edge at x_min
          Vec3r<TestType> org2{-1.f, 0.f, 4.f};
          Vec3r<TestType> dir2{0.f, 0.f, -1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};

          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(4));
          REQUIRE(rayhit2.hit.Ng_x == Approx(-1.f / std::sqrt(2)));
          REQUIRE(rayhit2.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: y max") {
          // edge at y_max
          Vec3r<TestType> org3{0.f, 1.f, 4.f};
          Vec3r<TestType> dir3{0.f, 0.f, -1.f};
          RTCRay ray3{org3[0], org3[1], org3[2], 0, dir3[0], dir3[1], dir3[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit3;
          hit3.geomID = RTC_INVALID_GEOMETRY_ID;
          hit3.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit3{ray3, hit3};
          rtcIntersect1(S, &context, &rayhit3);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit3.ray.tfar == Approx(4));
          REQUIRE(rayhit3.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit3.hit.Ng_y == Approx(1.f / std::sqrt(2)));
          REQUIRE(rayhit3.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: y min") {      // edge at y_min
          Vec3r<TestType> org4{0.f, -1.f, 4.f};
          Vec3r<TestType> dir4{0.f, 0.f, -1.f};
          RTCRay ray4{org4[0], org4[1], org4[2], 0, dir4[0], dir4[1], dir4[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit4;
          hit4.geomID = RTC_INVALID_GEOMETRY_ID;
          hit4.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit4{ray4, hit4};
          rtcIntersect1(S, &context, &rayhit4);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit4.ray.tfar == Approx(4));
          REQUIRE(rayhit4.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit4.hit.Ng_y == Approx(-1.f / std::sqrt(2)));
          REQUIRE(rayhit4.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
      }
      SECTION("non-rotated, corner intersection") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        SECTION("corner: x max, y max") {
          // corner at x_max, y_max
          Vec3r<TestType> org1{1.f, 1.f, 4.f};
          Vec3r<TestType> dir1{0.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(4));
          REQUIRE(rayhit1.hit.Ng_x == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit1.hit.Ng_y == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x min, y max") {
          // corner at x_min, y_max
          Vec3r<TestType> org2{-1.f, 1.f, 4.f};
          Vec3r<TestType> dir2{0.f, 0.f, -1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};
          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(4));

          REQUIRE(rayhit2.hit.Ng_x == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit2.hit.Ng_y == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit2.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x max, y min") {
          // corner at x_max, y_min
          Vec3r<TestType> org3{1.f, -1.f, 4.f};
          Vec3r<TestType> dir3{0.f, 0.f, -1.f};
          RTCRay ray3{org3[0], org3[1], org3[2], 0, dir3[0], dir3[1], dir3[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit3;
          hit3.geomID = RTC_INVALID_GEOMETRY_ID;
          hit3.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit3{ray3, hit3};
          rtcIntersect1(S, &context, &rayhit3);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit3.ray.tfar == Approx(4));
          REQUIRE(rayhit3.hit.Ng_x == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit3.hit.Ng_y == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit3.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x min, y min") {
          // corner at x_min, y_min
          Vec3r<TestType> org4{-1.f, -1.f, 4.f};
          Vec3r<TestType> dir4{0.f, 0.f, -1.f};
          RTCRay ray4{org4[0], org4[1], org4[2], 0, dir4[0], dir4[1], dir4[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit4;
          hit4.geomID = RTC_INVALID_GEOMETRY_ID;
          hit4.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit4{ray4, hit4};
          rtcIntersect1(S, &context, &rayhit4);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit4.ray.tfar == Approx(4));
          REQUIRE(rayhit4.hit.Ng_x == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit4.hit.Ng_y == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit4.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
      }
    }
    SECTION("shifted center") {
      Vec3r<TestType> center{1.f, 1.f, 1.f};
      SECTION("non-rotated"){
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{1.f, 1.f, 5.f};
        Vec3r<TestType> dir1{0.f, 0.f, -1.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        REQUIRE(rayhit1.ray.tfar == Approx(4));
        REQUIRE(rayhit1.hit.Ng_x == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_z == Approx(1.f));
      }
      SECTION("rotated about (0,1,0)") {
        // matrix which rotates the plane 45 degrees about the z-axis ( x = 0 is now
        // plane eq)
        const Vec3r<TestType> axis{0, 1, 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{5.f, 1.f, 1.f};
        Vec3r<TestType> dir1{-1.f, 0.f, 0.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        REQUIRE(rayhit1.ray.tfar == Approx(4));
        REQUIRE(rayhit1.hit.Ng_x == Approx(1.f));
        REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
        REQUIRE(rayhit1.hit.Ng_z == Approx(0.f).margin(0.0001));
      }
      SECTION("rotated about normalized(1,1,0), edge hit") {
        // matrix which rotates the plane about the y-axis ( x = 0 is now plane eq)
        const Vec3r<TestType> axis{static_cast<TestType>(1 / std::sqrt(2)), static_cast<TestType>(1 / std::sqrt(2)), 0};
        Mat3r<TestType> rot = arbitraryRotationMatrix(axis, pi<TestType> / 2);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        Vec3r<TestType> org1{-4.f, 6.f, 0.5f};
        Vec3r<TestType> dir1{1.f, -1.f, 0.f};
        RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
            0, 0, 0};
        RTCHit hit1;
        hit1.geomID = RTC_INVALID_GEOMETRY_ID;
        hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
        RTCRayHit rayhit1{ray1, hit1};
        rtcIntersect1(S, &context, &rayhit1);

        // hits from the negative x direction -> normal vector should point towards
        // that direction
        REQUIRE(rayhit1.ray.tfar == Approx(5 * std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_x == Approx(-1 / std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_y == Approx(1 / std::sqrt(2)));
        REQUIRE(rayhit1.hit.Ng_z == Approx(0.f).margin(0.0001));// for rotated planes, you can expect small numerical
        // error of size 1e16
      }
      SECTION("non-rotated, ray origin outside plane boundaries") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        SECTION("outside on positive x-axis") {
          Vec3r<TestType> org1{5.f, 1.f, 5.f};
          Vec3r<TestType> dir1{-1.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(4 * std::sqrt(2.f)));
          REQUIRE(rayhit1.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f));
        }
        SECTION("outside on negative x-axis") {
          Vec3r<TestType> org2{-3.f, 1.f, -3.f};
          Vec3r<TestType> dir2{1.f, 0.f, 1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};
          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(4. * std::sqrt(2.f)));

          REQUIRE(rayhit2.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_z == Approx(-1.f));
        }
      }
      SECTION("non-rotated, edge intersection") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        SECTION("edge: x max") {
          // edge at x_max
          Vec3r<TestType> org1{2.f, 1.f, 4.f};
          Vec3r<TestType> dir1{0.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(3));
          REQUIRE(rayhit1.hit.Ng_x == Approx(1.f / std::sqrt(2)));
          REQUIRE(rayhit1.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: x min") {
          // edge at x_min
          Vec3r<TestType> org2{0.f, 1.f, 4.f};
          Vec3r<TestType> dir2{0.f, 0.f, -1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};

          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(3));
          REQUIRE(rayhit2.hit.Ng_x == Approx(-1.f / std::sqrt(2)));
          REQUIRE(rayhit2.hit.Ng_y == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit2.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: y max") {
          // edge at y_max
          Vec3r<TestType> org3{1.f, 2.f, 4.f};
          Vec3r<TestType> dir3{0.f, 0.f, -1.f};
          RTCRay ray3{org3[0], org3[1], org3[2], 0, dir3[0], dir3[1], dir3[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit3;
          hit3.geomID = RTC_INVALID_GEOMETRY_ID;
          hit3.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit3{ray3, hit3};
          rtcIntersect1(S, &context, &rayhit3);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit3.ray.tfar == Approx(3));
          REQUIRE(rayhit3.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit3.hit.Ng_y == Approx(1.f / std::sqrt(2)));
          REQUIRE(rayhit3.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
        SECTION("edge: y min") {      // edge at y_min
          Vec3r<TestType> org4{1.f, 0.f, 4.f};
          Vec3r<TestType> dir4{0.f, 0.f, -1.f};
          RTCRay ray4{org4[0], org4[1], org4[2], 0, dir4[0], dir4[1], dir4[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit4;
          hit4.geomID = RTC_INVALID_GEOMETRY_ID;
          hit4.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit4{ray4, hit4};
          rtcIntersect1(S, &context, &rayhit4);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit4.ray.tfar == Approx(3));
          REQUIRE(rayhit4.hit.Ng_x == Approx(0.f).margin(0.0001));
          REQUIRE(rayhit4.hit.Ng_y == Approx(-1.f / std::sqrt(2)));
          REQUIRE(rayhit4.hit.Ng_z == Approx(1.f / std::sqrt(2)));
        }
      }
      SECTION("non-rotated, corner intersection") {
        Mat3r<TestType> rot = blaze::IdentityMatrix<TestType>(3UL);

        EmbreePlane plane(D, S, center, d1, d2, rot);
        rtcCommitScene(S);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        SECTION("corner: x max, y max") {
          // corner at x_max, y_max
          Vec3r<TestType> org1{2.f, 2.f, 4.f};
          Vec3r<TestType> dir1{0.f, 0.f, -1.f};
          RTCRay ray1{org1[0], org1[1], org1[2], 0, dir1[0], dir1[1], dir1[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit1;
          hit1.geomID = RTC_INVALID_GEOMETRY_ID;
          hit1.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit1{ray1, hit1};
          rtcIntersect1(S, &context, &rayhit1);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit1.ray.tfar == Approx(3));
          REQUIRE(rayhit1.hit.Ng_x == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit1.hit.Ng_y == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit1.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x min, y max") {
          // corner at x_min, y_max
          Vec3r<TestType> org2{0.f, 2.f, 4.f};
          Vec3r<TestType> dir2{0.f, 0.f, -1.f};
          RTCRay ray2{org2[0], org2[1], org2[2], 0, dir2[0], dir2[1], dir2[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit2;
          hit2.geomID = RTC_INVALID_GEOMETRY_ID;
          hit2.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit2{ray2, hit2};
          rtcIntersect1(S, &context, &rayhit2);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit2.ray.tfar == Approx(3));

          REQUIRE(rayhit2.hit.Ng_x == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit2.hit.Ng_y == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit2.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x max, y min") {
          // corner at x_max, y_min
          Vec3r<TestType> org3{2.f, 0.f, 4.f};
          Vec3r<TestType> dir3{0.f, 0.f, -1.f};
          RTCRay ray3{org3[0], org3[1], org3[2], 0, dir3[0], dir3[1], dir3[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit3;
          hit3.geomID = RTC_INVALID_GEOMETRY_ID;
          hit3.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit3{ray3, hit3};
          rtcIntersect1(S, &context, &rayhit3);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit3.ray.tfar == Approx(3));
          REQUIRE(rayhit3.hit.Ng_x == Approx(1.f / std::sqrt(3)));
          REQUIRE(rayhit3.hit.Ng_y == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit3.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
        SECTION("corner: x min, y min") {
          // corner at x_min, y_min
          Vec3r<TestType> org4{0.f, 0.f, 4.f};
          Vec3r<TestType> dir4{0.f, 0.f, -1.f};
          RTCRay ray4{org4[0], org4[1], org4[2], 0, dir4[0], dir4[1], dir4[2], 0, std::numeric_limits<float>::max(),
              0, 0, 0};
          RTCHit hit4;
          hit4.geomID = RTC_INVALID_GEOMETRY_ID;
          hit4.instID[0] = RTC_INVALID_GEOMETRY_ID;
          RTCRayHit rayhit4{ray4, hit4};
          rtcIntersect1(S, &context, &rayhit4);

          // hits from the negative x direction -> normal vector should point towards
          // that direction
          REQUIRE(rayhit4.ray.tfar == Approx(3));
          REQUIRE(rayhit4.hit.Ng_x == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit4.hit.Ng_y == Approx(-1.f / std::sqrt(3)));
          REQUIRE(rayhit4.hit.Ng_z == Approx(1.f / std::sqrt(3)));
        }
      }
    }
  }
}