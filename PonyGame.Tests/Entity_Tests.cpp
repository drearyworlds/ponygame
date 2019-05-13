#include "pch.h"
#include <Pony.h>

using namespace ParticleHomeEntertainment;

struct Location {
    float x, y;
    friend std::ostream& operator<<(std::ostream& os, const Location& obj) {
        return os << "(" << obj.x << ", " << obj.y <<")";
    }
};

struct EntityCollisionTest : testing::Test, testing::WithParamInterface<Location> {
    EntityCollisionTest() { }
    EntityCollisionTest(const EntityCollisionTest&) = delete;
    EntityCollisionTest& operator=(const EntityCollisionTest&) = delete;
};

INSTANTIATE_TEST_CASE_P(Default, EntityCollisionTest,
    testing::Values(
        Location { -1, -1 },
        Location { 123,456 }
));

TEST_P(EntityCollisionTest, ShouldReturnTrueForCollision) {
    auto expectedLocation = GetParam();
    Pony p(expectedLocation.x, expectedLocation.y);

    EXPECT_EQ(expectedLocation.x, p.GetLocation().x);
    EXPECT_EQ(expectedLocation.y, p.GetLocation().y);
}