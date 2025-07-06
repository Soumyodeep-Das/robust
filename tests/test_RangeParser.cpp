#include "gtest/gtest.h"
#include "RangeParser.hpp"
#include <vector>
#include <string>

using namespace drogon;

TEST(RangeParserTest, InvalidRangeHeader)
{
    std::vector<FileRange> ranges;
    EXPECT_EQ(parseRangeHeader("bytes", 100, ranges), InvalidRange);
    EXPECT_EQ(parseRangeHeader("byte=0-10", 100, ranges), InvalidRange);
    EXPECT_EQ(parseRangeHeader("bytes=abc", 100, ranges), InvalidRange);
    EXPECT_EQ(parseRangeHeader("bytes=10-5", 100, ranges), NotSatisfiable);
    EXPECT_EQ(parseRangeHeader("bytes=10-5, 20-15", 100, ranges), InvalidRange);
}

TEST(RangeParserTest, SingleRangeValid)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes=0-9", 100, ranges);
    EXPECT_EQ(result, SinglePart);
    ASSERT_EQ(ranges.size(), 1u);
    EXPECT_EQ(ranges[0].start, 0u);
    EXPECT_EQ(ranges[0].end, 10u);
}

TEST(RangeParserTest, SingleRangePrefix)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes=5-", 100, ranges);
    EXPECT_EQ(result, SinglePart);
    ASSERT_EQ(ranges.size(), 1u);
    EXPECT_EQ(ranges[0].start, 5u);
    EXPECT_EQ(ranges[0].end, 100u);
}

TEST(RangeParserTest, SingleRangeSuffix)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes=-10", 100, ranges);
    EXPECT_EQ(result, SinglePart);
    ASSERT_EQ(ranges.size(), 1u);
    EXPECT_EQ(ranges[0].start, 90u);
    EXPECT_EQ(ranges[0].end, 100u);
}

TEST(RangeParserTest, MultipleRangesValid)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes=0-9, 20-29, 40-49", 100, ranges);
    EXPECT_EQ(result, MultiPart);
    ASSERT_EQ(ranges.size(), 3u);
    EXPECT_EQ(ranges[0].start, 0u);
    EXPECT_EQ(ranges[0].end, 10u);
    EXPECT_EQ(ranges[1].start, 20u);
    EXPECT_EQ(ranges[1].end, 30u);
    EXPECT_EQ(ranges[2].start, 40u);
    EXPECT_EQ(ranges[2].end, 50u);
}

TEST(RangeParserTest, RangeExceedingContentLength)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes=90-110", 100, ranges);
    EXPECT_EQ(result, SinglePart);
    ASSERT_EQ(ranges.size(), 1u);
    EXPECT_EQ(ranges[0].start, 90u);
    EXPECT_EQ(ranges[0].end, 100u);
}

TEST(RangeParserTest, TooManyRanges)
{
    std::vector<FileRange> ranges;
    std::string rangeHeader = "bytes=";
    for (int i = 0; i < 101; ++i)
    {
        if (i > 0)
            rangeHeader += ",";
        rangeHeader += std::to_string(i * 10) + "-" + std::to_string(i * 10 + 9);
    }
    auto result = parseRangeHeader(rangeHeader, 2000, ranges);
    EXPECT_EQ(result, InvalidRange);
}

TEST(RangeParserTest, OverflowRange)
{
    std::vector<FileRange> ranges;
    // Use a very large number to cause overflow
    std::string rangeHeader = "bytes=18446744073709551615-18446744073709551616";
    auto result = parseRangeHeader(rangeHeader, 2000, ranges);
    EXPECT_EQ(result, NotSatisfiable);
}

TEST(RangeParserTest, EmptyRanges)
{
    std::vector<FileRange> ranges;
    auto result = parseRangeHeader("bytes= ", 100, ranges);
    EXPECT_EQ(result, InvalidRange);
}

TEST(RangeParserTest, TotalSizeExceedsContentLength)
{
    std::vector<FileRange> ranges;
    // Two ranges that together exceed content length
    auto result = parseRangeHeader("bytes=0-90, 91-200", 100, ranges);
    EXPECT_EQ(result, NotSatisfiable);
}