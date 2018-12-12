#include <iostream>
#include <memory>

#include "gtest/gtest.h"

#include "../linq.h"

using namespace linq;

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"
#define GTEST_WARN std::cerr << "[          ] [ WARNING ]"

int testCount = 0;

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
        decltype(from(std::declval<std::vector<std::shared_ptr<A>>&>())) as_linqed;
        decltype(from(std::declval<const std::vector<std::shared_ptr<A>>&>())) as_const_linqed;
        std::vector<std::pair<int, int>> pairsDoubled;
        decltype(from(std::declval<std::vector<std::pair<int, int>>&>())) pairsDoubled_linqed;
        decltype(from(std::declval<const std::vector<std::pair<int,int>>&>())) pairsDoubled_const_linqed;
        std::vector<std::pair<int, int>> pairsSquared;
        decltype(from(std::declval<std::vector<std::pair<int, int>>&>())) pairsSquared_linqed;
        decltype(from(std::declval<const std::vector<std::pair<int,int>>&>())) pairsSquared_const_linqed;

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

            as_linqed = from(this->as);
            as_const_linqed = from(((const LinqTest*)this)->as);
            pairsDoubled_linqed = from(this->pairsDoubled);
            pairsDoubled_const_linqed = from(((const LinqTest*)this)->pairsDoubled);
            pairsSquared_linqed = from(this->pairsSquared);
            pairsSquared_const_linqed = from(((const LinqTest*)this)->pairsSquared);
		}
	};
}

TEST_F(LinqTest, TestFrom) {
    auto current = as_linqed.begin();
    for(size_t i=0; i < as.size(); i++) {
        EXPECT_EQ(*current, as[i]);
        ++current;
    }
    EXPECT_EQ(current, as_linqed.end());
}

TEST_F(LinqTest, TestLooping) {
    int count = 0;
    for(std::shared_ptr<A>& a : as_linqed) {
        EXPECT_EQ(a->test(), as[count]->test());
        count++;
    }
    EXPECT_EQ(count, as.size());
}

TEST_F(LinqTest, TestConstLooping) {
    int count = 0;
    for(const std::shared_ptr<A>& a : as_linqed) {
        EXPECT_EQ(a->test(), as[count]->test());
        count++;
    }
    EXPECT_EQ(count, as.size());
}

TEST_F(LinqTest, TestCount){
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
    // CodeReview: Implement

}

TEST_F(LinqTest, TestSize){
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
    // CodeReview: Implement
}

TEST_F(LinqTest, TestContains) {
    EXPECT_TRUE(as_linqed.contains(as[7]));
}

TEST_F(LinqTest, TestForEach) {
    int count = 0;
    as_linqed.forEach([&count](std::shared_ptr<A>& a) -> void{ count += a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestConstForEach) {
    int count = 0;
    as_linqed.forEach([&count](const std::shared_ptr<A>& a) -> void{ count += a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestAggregate) {
    int count = as_const_linqed.aggregate([](int count, const std::shared_ptr<A>& a) -> int { return count + a->test(); });
    EXPECT_EQ(count, 78);
}

TEST_F(LinqTest, TestAggregateStartingValue) {
    int count = as_const_linqed.aggregate(22, [](int count, const std::shared_ptr<A>& a) -> int { return count + a->test(); });
    EXPECT_EQ(count, 100);
}

TEST_F(LinqTest, TestAt) {
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(as_linqed.at(i)->test(), i);
}

TEST_F(LinqTest, TestConstAt) {
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(as_const_linqed.at(i)->test(), i);
}

TEST_F(LinqTest, TestIndexing) {
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(as_linqed[i]->test(), i);
}

TEST_F(LinqTest, TestConstIndexing) {
    const auto linqed = from(as);
    for(size_t i=0; i < as.size(); i++) EXPECT_EQ(as_const_linqed[i]->test(), i);
}

TEST_F(LinqTest, TestAtOrDefaultIn) {
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
    // CodeReview: Implement
}

TEST_F(LinqTest, TestAtOrDefaultNotIn) {
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
    // CodeReview: Implement
}

TEST_F(LinqTest, TestEmptyFalse) {
    EXPECT_FALSE(as_const_linqed.empty());
}

TEST_F(LinqTest, TestEmptyTrue) {
    std::vector<int> temp;
    const auto linqed = from(temp);
    EXPECT_TRUE(linqed.empty());
}

TEST_F(LinqTest, TestAllTrue) {
    EXPECT_TRUE(as_const_linqed.all([](const std::shared_ptr<A>& a) { return a->test() >= 0; }));
}

TEST_F(LinqTest, TestAllFalse) {
    EXPECT_FALSE(as_const_linqed.all([](const std::shared_ptr<A>& a) { return a->test() < 10; }));
}

TEST_F(LinqTest, TestAnyTrue) {
    EXPECT_TRUE(as_const_linqed.any([](const std::shared_ptr<A>& a) { return a->test() == 10; }));
}

TEST_F(LinqTest, TestAnyFalse) {
    EXPECT_FALSE(as_const_linqed.any([](const std::shared_ptr<A>& a) { return a->test() < 0; }));
}

TEST_F(LinqTest, TestFirst) {
    EXPECT_EQ(&as_linqed.first(), &as.front());
}

TEST_F(LinqTest, TestFront) {
    EXPECT_EQ(&as_linqed.front(), &as.front());
}

TEST_F(LinqTest, TestConstFirst) {
    EXPECT_EQ(&as_const_linqed.first(), &as.front());
}

TEST_F(LinqTest, TestConstFront) {
    EXPECT_EQ(&as_const_linqed.front(), &as.front());
}

TEST_F(LinqTest, TestLast) {
    /* EXPECT_EQ(&as_linqed.last(), &as.back()); */
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestBack) {
    /* EXPECT_EQ(&as_linqed.back(), &as.back()); */
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstLast) {
    /* EXPECT_EQ(&as_const_linqed.last(), &as.back()); */
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstBack) {
    /* EXPECT_EQ(&as_const_linqed.back(), &as.back()); */
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestFirstOrDefaultIn) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestFirstOrDefaultEmpty) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestLastOrDefaultIn) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestLastOrDefaultEmpty) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestToContainerSet) {
    std::set<std::shared_ptr<A>> setVersion = as_const_linqed.toContainer<std::set<std::shared_ptr<A>>>();
    for(const std::shared_ptr<A>& a : as_linqed) {
        EXPECT_NE(setVersion.find(a), setVersion.end());
    }
    EXPECT_EQ(setVersion.size(), as.size());
}

TEST_F(LinqTest, TestToContainerMap) {
    std::map<int, int> mapVersion = pairsDoubled_const_linqed.toContainer<std::map<int, int>>();
    for(const std::pair<int, int>& pair : pairsDoubled_const_linqed) {
        ASSERT_NE(mapVersion.find(pair.first), mapVersion.end());
        EXPECT_EQ(mapVersion.at(pair.first), pair.second);
    }
    EXPECT_EQ(mapVersion.size(), pairsDoubled.size());
}

TEST_F(LinqTest, TestToContainerGeneric) {
    std::set<std::shared_ptr<A>> setVersion = as_const_linqed.toContainer<std::set>();
    for(const std::shared_ptr<A>& a : as_const_linqed) {
        EXPECT_NE(setVersion.find(a), setVersion.end());
    }
    EXPECT_EQ(setVersion.size(), as.size());
}

TEST_F(LinqTest, TestToVector) {
    // CodeReveiw need operator-(iterator_wrapper other)
    /* std::vector<std::shared_ptr<A>> vectorVersion = as_linqed.toVector(); */
    /* ASSERT_EQ(vectorVersion.size(), as.size()); */
    /* for(size_t i = 0; i < as.size(); i++) { */
    /*     EXPECT_EQ(vectorVersion[i], as[i]); */
    /* } */
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestFilterEven) {
    auto filtered = as_linqed.filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 0; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 0);
        ++count;
    }
    EXPECT_EQ(count, 7);
}

TEST_F(LinqTest, TestFilterOdd) {
    auto filtered = as_linqed.filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 1; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 1);
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(LinqTest, TestFilterOddConst) {
    const auto filtered = as_linqed.filter([](const std::shared_ptr<A>& a) { return a->test() % 2 == 1; });
    // EXPECT_EQ(filtered.count(), 7);
    size_t count = 0;
    for (const std::shared_ptr<A>& a : filtered) {
        EXPECT_EQ(a->test() % 2, 1);
        ++count;
    }
    EXPECT_EQ(count, 6);
}

TEST_F(LinqTest, TestFilterFalse) {
    auto filtered = as_linqed.filter([](const std::shared_ptr<A>&) { return false; });
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
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSelect) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestOfTypeSharedPtr) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstOfTypeSharedPtr) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestOfTypePtr) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstOfTypePtr) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestOfTypeRef) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstOfTypeRef) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestAppend) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstAppend) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestPrepend) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstPrepend) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestReverse) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstReverse) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestRemoveFirst) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstRemoveFirst) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestRemoveAllContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstRemoveAllContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestRemoveAllIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstRemoveAllIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConcatContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstConcatContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConcatIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstConcatIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestDistinct) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstDistinct) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestSetUnionContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSetUnionContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestSetUnionIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSetUnionIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestOrderByDefault) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstOrderByDefault) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestOrderByFunc) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstOrderByFunc) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestTake) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstTake) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestSkip) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSkip) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestTakeWhile) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstTakeWhile) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestSkipWhile) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSkipWhile) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestFlatten) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstFlatten) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestSelectMany) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstSelectMany) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestGroup) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstGroup) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestJoinContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstJoinContainer) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestJoinIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstJoinIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestDefaultZipContainers) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstDefaultZipContainers) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestDefaultZipIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstDefaultZipIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestZipContainers) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstZipContainers) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestZipIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}

TEST_F(LinqTest, TestConstZipIterators) {
    // CodeReview: Implement
    GTEST_WARN << "Test not implemented. Number " << testCount++ << "\n";
}
