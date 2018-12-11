#include <memory>

#include "gtest/gtest.h"

#include "../linq.h"

using namespace linq;

struct A {
	virtual int test() const = 0;
	virtual ~A() {};
};

template<int number>
struct B : public A {
	int test() const override {
		return number;
	}
};

namespace {
	class LinqTest : public ::testing::Test {
	protected:
		std::vector<std::shared_ptr<A>> as;

		LinqTest() {
			as.push_back(std::make_shared<B<0>>());
			as.push_back(std::make_shared<B<1>>());
			as.push_back(std::make_shared<B<2>>());
			as.push_back(std::make_shared<B<3>>());
			as.push_back(std::make_shared<B<4>>());
			as.push_back(std::make_shared<B<5>>());
			as.push_back(std::make_shared<B<6>>());
			as.push_back(std::make_shared<B<7>>());
			as.push_back(std::make_shared<B<8>>());
			as.push_back(std::make_shared<B<9>>());
			as.push_back(std::make_shared<B<10>>());
			as.push_back(std::make_shared<B<11>>());
			as.push_back(std::make_shared<B<12>>());
		}
	};
}

TEST_F(LinqTest, TestFrom) {
    auto linqed = from(as);

    auto current = linqed.begin();
    for(size_t i=0; i < as.size(); i++) {
        EXPECT_EQ(*current, as[i]);
        ++current;
    }
    EXPECT_EQ(current, linqed.end());
}

TEST_F(LinqTest, TestLooping) {
    auto linqed = from(as);

    int count = 0;
    for(std::shared_ptr<A>& a : linqed) {
        EXPECT_EQ(a->test(), count);
        count++;
    }
    EXPECT_EQ(count, 13);
}

TEST_F(LinqTest, TestConstLooping) {
    const auto linqed = from(as);

    int count = 0;
    for(const std::shared_ptr<A>& a : linqed) {
        EXPECT_EQ(a->test(), count);
        count++;
    }
    EXPECT_EQ(count, 13);
}

TEST_F(LinqTest, TestCount){
    // CodeReview: Implement
}

TEST_F(LinqTest, TestCountSizeEqual){
    // CodeReview: Implement
}

TEST_F(LinqTest, TestContains) {
    auto linqed = from(as);
    EXPECT_TRUE(linqed.contains(as[7]));
}

TEST_F(LinqTest, TestForEach) {
    auto linqed = from(as);
    int count = 0;
    linqed.forEach([&count](std::shared_ptr<A>& a) -> void{ count += a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestConstForEach) {
    const auto linqed = from(as);
    int count = 0;
    linqed.forEach([&count](const std::shared_ptr<A>& a) -> void{ count += a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestAggregate) {
    const auto linqed = from(as);
    int count = linqed.aggregate([](int count, const std::shared_ptr<A>& a) -> int { return count + a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestAggregateStartingValue) {
    const auto linqed = from(as);
    int count = linqed.aggregate(22, [](int count, const std::shared_ptr<A>& a) -> int { return count + a->test(); });
    EXPECT_EQ(count, 100);
}

TEST_F(LinqTest, TestWhereEven) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 0; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 0);
        ++count;
    }
    EXPECT_EQ(count, 7);
}

TEST_F(LinqTest, TestWhereOdd) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 1; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 1);
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(LinqTest, TestWhereFalse) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>&) { return false; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& _ : filtered) {
        EXPECT_TRUE(false);
        ++count;
    }
    EXPECT_EQ(count, 0);
}

