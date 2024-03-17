#include "prqueue.h"

#include "gtest/gtest.h"
#include <queue>

using namespace std;

/*
Part One Tests
*/
TEST(Constructor, DefaultConstructor) {
    prqueue<int> pq;
    EXPECT_EQ(pq.size(), 0);
}

TEST(Enqueue, MultipleElements) {
    prqueue<int> pq;

    pq.enqueue(20, 2);
    pq.enqueue(10, 1);
    pq.enqueue(30, 3);

    EXPECT_EQ(pq.size(), 3);

    // Verify the order of elements after enqueue
    pq.begin();
    int value;
    int priority;

    // Test first element
    pq.next(value, priority);
    EXPECT_EQ(value, 10);
    EXPECT_EQ(priority, 1);
 
    // Test second element
    pq.next(value, priority);
    EXPECT_EQ(value, 20);
    EXPECT_EQ(priority, 2);

    // Test third element
    pq.next(value, priority);
    EXPECT_EQ(value, 30);
    EXPECT_EQ(priority, 3);

    // Verify that there are no more elements in the queue
    EXPECT_FALSE(pq.next(value, priority));

    // Clear the priority queue
    pq.clear();
    EXPECT_EQ(pq.size(), 0);
}

TEST(PeekTest, EmptyQueue) {
    prqueue<int> pq;
    EXPECT_EQ(pq.peek(), 0);
}

// Test case for a non-empty priority queue
TEST(PeekTest, NonEmptyQueue) {
    prqueue<int> pq;

    pq.enqueue(10, 2);
    pq.enqueue(20, 1);
    pq.enqueue(15, 3);

    // The element with the lowest priority should be 1 with a value of 20
    EXPECT_EQ(pq.peek(), 20);
}

TEST(DequeueTest, EmptyQueue) {
    prqueue<int> pq;
    EXPECT_EQ(pq.dequeue(), 0);
}

TEST(DequeueTest, NonEmptyQueue) {
    prqueue<int> pq;

    pq.enqueue(10, 2);
    pq.enqueue(20, 1);
    pq.enqueue(15, 3);


    EXPECT_EQ(pq.size(), 3);

    // Dequeue the element with the lowest priority, which is 1
    EXPECT_EQ(pq.dequeue(), 20);
    EXPECT_EQ(pq.size(), 2);
    EXPECT_EQ(pq.peek(), 10);
}



TEST(AsStringTest, EmptyQueue) {
    prqueue<string> names;
    ASSERT_EQ(names.as_string(), "");
}

TEST(AsStringTest, OneElement) {
    prqueue<string> names;
    names.enqueue("Gwen", 3);
    ASSERT_EQ(names.as_string(), "3 value: Gwen\n");
}

TEST(AsStringTest, MultipleElements) {
    prqueue<string> names;
    names.enqueue("Gwen", 3);
    names.enqueue("Jen", 2);
    names.enqueue("Ben", 1);
    names.enqueue("Sven", 2);

    stringstream expected;
    expected << "1 value: Ben\n"
             << "2 value: Jen\n"
             << "2 value: Sven\n"
             << "3 value: Gwen\n";

    ASSERT_EQ(names.as_string(), expected.str());
}

TEST(ClearTest, OneElement) {
    prqueue<int> pq;
    pq.enqueue(10, 1);

    pq.clear();
    EXPECT_EQ(pq.peek(), 0);
}



/*
Part Two Tests (DUPES)
*/
TEST(EnqueueTest, DuplicatePriorities) {
    prqueue<int> pq;

    pq.enqueue(10, 2);
    pq.enqueue(20, 1);
    pq.enqueue(30, 2);
    pq.enqueue(40, 3);
    pq.enqueue(50, 1);

    EXPECT_EQ(pq.size(), 5);
}

TEST(AsStringTest, MultipleDuplicateElements) {
    prqueue<string> names;
    names.enqueue("Gwen", 3);
    names.enqueue("Jen", 2);
    names.enqueue("Ben", 1);
    names.enqueue("Sven", 2);
    names.enqueue("Ken", 1);

    stringstream expected;
    expected << "1 value: Ben\n"
             << "1 value: Ken\n"
             << "2 value: Jen\n"
             << "2 value: Sven\n"
             << "3 value: Gwen\n";

    EXPECT_EQ(names.as_string(), expected.str());
}

TEST(ClearTest, MultipleDuplicateElements) {
    prqueue<string> names;
    names.enqueue("Gwen", 3);
    names.enqueue("Jen", 2);
    names.enqueue("Ben", 1);
    names.enqueue("Sven", 2);
    names.enqueue("Ken", 1);
    
    names.clear();
    EXPECT_EQ(names.size(), 0);
}