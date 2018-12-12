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
        std::vector<std::pair<int, int>> pairsDoubled;
        std::vector<std::pair<int, int>> pairsSquared;

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

            for(int i=0; i < 13; i++) {
                pairsDoubled.push_back({i, i*2});
                pairsSquared.push_back({i, i*i});
            }
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

TEST_F(LinqTest, TestSize){
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

TEST_F(LinqTest, TestAt) {
    auto linqed = from(as);
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(linqed.at(i)->test(), i);
}

TEST_F(LinqTest, TestConstAt) {
    const auto linqed = from(as);
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(linqed.at(i)->test(), i);
}

TEST_F(LinqTest, TestIndexing) {
    auto linqed = from(as);
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(linqed[i]->test(), i);
}

TEST_F(LinqTest, TestConstIndexing) {
    const auto linqed = from(as);
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(linqed[i]->test(), i);
}

TEST_F(LinqTest, TestAtOrDefaultIn) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestAtOrDefaultNotIn) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestEmptyFalse) {
    auto linqed = from(as);
    EXPECT_FALSE(linqed.empty());
}

TEST_F(LinqTest, TestEmptyTrue) {
    std::vector<int> temp;
    auto linqed = from(temp);
    EXPECT_TRUE(linqed.empty());
}

TEST_F(LinqTest, TestAllTrue) {
    auto linqed = from(as);
    EXPECT_TRUE(linqed.all([](const std::shared_ptr<A>& a) { return a->test() >= 0; }));
}

TEST_F(LinqTest, TestAllFalse) {
    auto linqed = from(as);
    EXPECT_FALSE(linqed.all([](const std::shared_ptr<A>& a) { return a->test() < 10; }));
}

TEST_F(LinqTest, TestAnyTrue) {
    auto linqed = from(as);
    EXPECT_TRUE(linqed.any([](const std::shared_ptr<A>& a) { return a->test() == 10; }));
}

TEST_F(LinqTest, TestAnyFalse) {
    auto linqed = from(as);
    EXPECT_FALSE(linqed.any([](const std::shared_ptr<A>& a) { return a->test() < 0; }));
}

TEST_F(LinqTest, TestFirst) {
    auto linqed = from(as);
    EXPECT_EQ(&linqed.first(), &as.front());
}

TEST_F(LinqTest, TestFront) {
    auto linqed = from(as);
    EXPECT_EQ(&linqed.front(), &as.front());
}

TEST_F(LinqTest, TestConstFirst) {
    const auto linqed = from(as);
    EXPECT_EQ(&linqed.first(), &as.front());
}

TEST_F(LinqTest, TestConstFront) {
    const auto linqed = from(as);
    EXPECT_EQ(&linqed.front(), &as.front());
}

TEST_F(LinqTest, TestLast) {
    /* auto linqed = from(as); */
    /* EXPECT_EQ(&linqed.last(), &as.back()); */
}

TEST_F(LinqTest, TestBack) {
    /* auto linqed = from(as); */
    /* EXPECT_EQ(&linqed.back(), &as.back()); */
}

TEST_F(LinqTest, TestConstLast) {
    /* const auto linqed = from(as); */
    /* EXPECT_EQ(&linqed.last(), &as.back()); */
}

TEST_F(LinqTest, TestConstBack) {
    /* const auto linqed = from(as); */
    /* EXPECT_EQ(&linqed.back(), &as.back()); */
}

TEST_F(LinqTest, TestFirstOrDefaultIn) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestFirstOrDefaultEmpty) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestLastOrDefaultIn) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestLastOrDefaultEmpty) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestToContainerSet) {
    const auto linqed = from(as);
    std::set<std::shared_ptr<A>> setVersion = linqed.toContainer<std::set<std::shared_ptr<A>>>();
    for(const std::shared_ptr<A>& a : linqed) {
        EXPECT_NE(setVersion.find(a), setVersion.end());
    }
    EXPECT_EQ(setVersion.size(), as.size());
}

TEST_F(LinqTest, TestToContainerMap) {
    const auto linqed = from(pairsDoubled);
    std::map<int, int> mapVersion = linqed.toContainer<std::map<int, int>>();
    for(const std::pair<int, int>& pair : linqed) {
        ASSERT_NE(mapVersion.find(pair.first), mapVersion.end());
        EXPECT_EQ(mapVersion.at(pair.first), pair.second);
    }
    EXPECT_EQ(mapVersion.size(), pairsDoubled.size());
}

TEST_F(LinqTest, TestToContainerGeneric) {
    const auto linqed = from(as);
    std::set<std::shared_ptr<A>> setVersion = linqed.toContainer<std::set>();
    for(const std::shared_ptr<A>& a : linqed) {
        EXPECT_NE(setVersion.find(a), setVersion.end());
    }
    EXPECT_EQ(setVersion.size(), as.size());
}

TEST_F(LinqTest, TestToVector) {
    // CodeReveiw need operator-(iterator_wrapper other)
    /* const auto linqed = from(as); */
    /* std::vector<std::shared_ptr<A>> vectorVersion = linqed.toVector(); */
    /* ASSERT_EQ(vectorVersion.size(), as.size()); */
    /* for(size_t i = 0; i < as.size(); i++) { */
    /*     EXPECT_EQ(vectorVersion[i], as[i]); */
    /* } */
}

TEST_F(LinqTest, TestFilterEven) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 0; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 0);
        ++count;
    }
    EXPECT_EQ(count, 7);
}

TEST_F(LinqTest, TestFilterOdd) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 1; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 1);
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(LinqTest, TestFilterOddConst) {
    const auto filtered = from(as).filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 1; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 1);
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(LinqTest, TestFilterFalse) {
    auto filtered = from(as).filter([](const std::shared_ptr<A>&) { return false; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& _ : filtered) {
        EXPECT_TRUE(false);
        ++count;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(LinqTest, TestSelect) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSelect) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestOfTypeSharedPtr) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstOfTypeSharedPtr) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestOfTypePtr) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstOfTypePtr) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestOfTypeRef) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstOfTypeRef) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestAppend) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstAppend) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestPrepend) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstPrepend) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestReverse) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstReverse) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestRemoveFirst) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstRemoveFirst) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestRemoveAllContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstRemoveAllContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestRemoveAllIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstRemoveAllIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConcatContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstConcatContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConcatIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstConcatIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestDistinct) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstDistinct) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestSetUnionContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSetUnionContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestSetUnionIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSetUnionIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestOrderByDefault) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstOrderByDefault) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestOrderByFunc) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstOrderByFunc) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestTake) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstTake) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestSkip) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSkip) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestTakeWhile) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstTakeWhile) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestSkipWhile) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSkipWhile) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestFlatten) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstFlatten) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestSelectMany) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstSelectMany) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestGroup) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstGroup) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestJoinContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstJoinContainer) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestJoinIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstJoinIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestDefaultZipContainers) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstDefaultZipContainers) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestDefaultZipIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstDefaultZipIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestZipContainers) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstZipContainers) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestZipIterators) {
    // CodeReview: Implement
}

TEST_F(LinqTest, TestConstZipIterators) {
    // CodeReview: Implement
}
