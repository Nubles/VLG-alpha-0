#include "Tests/Game/GameTestSuite.h"

#include "Game/Source/Data/DataTextParser.h"

#include <cassert>
#include <string>
#include <vector>

void TestDataTextParser()
{
    const std::vector<std::string> parts = rw::game::data::Split("wood:1, stone:2 ,fiber:3", ',');
    assert(parts.size() == 3);
    assert(parts[0] == "wood:1");
    assert(parts[1] == "stone:2");
    assert(parts[2] == "fiber:3");

    assert(rw::game::data::IsCommentOrBlankLine(""));
    assert(rw::game::data::IsCommentOrBlankLine("   "));
    assert(rw::game::data::IsCommentOrBlankLine("# comment"));
    assert(rw::game::data::IsCommentOrBlankLine("   # comment"));
    assert(!rw::game::data::IsCommentOrBlankLine("wood|Wood"));

    int intValue = 0;
    assert(rw::game::data::ParsePositiveInt("42", intValue));
    assert(intValue == 42);
    assert(!rw::game::data::ParsePositiveInt("0", intValue));
    assert(!rw::game::data::ParsePositiveInt("-1", intValue));
    assert(!rw::game::data::ParsePositiveInt("not-a-number", intValue));

    float floatValue = 0.0F;
    assert(rw::game::data::ParsePositiveFloat("3.5", floatValue));
    assert(floatValue == 3.5F);
    assert(!rw::game::data::ParsePositiveFloat("0", floatValue));
    assert(!rw::game::data::ParsePositiveFloat("-1.0", floatValue));
    assert(!rw::game::data::ParsePositiveFloat("nope", floatValue));

    rw::math::Vec3 vec;
    assert(rw::game::data::ParseVec3Triple("1, 2.5,3", vec));
    assert(vec.x == 1.0F);
    assert(vec.y == 2.5F);
    assert(vec.z == 3.0F);
    assert(!rw::game::data::ParseVec3Triple("1,2", vec));
    assert(!rw::game::data::ParseVec3Triple("1,two,3", vec));
    assert(!rw::game::data::ParseVec3Triple("1,0,3", vec));

    assert(rw::game::data::MakeLineError("Item", 7, "bad value") == "Item data line 7: bad value");
}
