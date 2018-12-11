#include <memory>

#include "gtest/gtest.h"

#include "linq.h"

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
};
TEST_F(LinqTest, TestWhereEven) {
	{
		auto filtered = filter(as.begin(), as.end(), [](const std::shared_ptr<A>& a) { return a->test() % 2 == 0; });
		// EXPECT_EQ(filtered.count(), 7);
		size_t count = 0;
		for (const std::shared_ptr<A>& a : filtered) {
			EXPECT_EQ(a->test() % 2, 0);
			++count;
		}
		EXPECT_EQ(count, 7);
	}
}