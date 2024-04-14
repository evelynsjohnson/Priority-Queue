#include "prqueue.h"

#include "gtest/gtest.h"
#include <queue>

using namespace std;
TEST(ConstructorTest, DefaultConstructor) {
    prqueue<int> pq;
    EXPECT_EQ(pq.size(), 0);
}

TEST(EnqueueTest, MultipleElements) {
    prqueue<int> pq;

    pq.enqueue(20, 2);
    pq.enqueue(10, 1);
    pq.enqueue(30, 3);

    EXPECT_EQ(pq.size(), 3);
}

TEST(EnqueueTest, DuplicatePriorities) {
    prqueue<int> pq;
    
    pq.enqueue(10, 2);
    pq.enqueue(20, 1);
    pq.enqueue(30, 2);
    pq.enqueue(40, 3);
    pq.enqueue(50, 1);

    EXPECT_EQ(pq.size(), 5);
    pq.begin();
    int value;
    int priority;
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(pq.next(value, priority));
        //cout << value << ", " << priority << endl;
    }

    EXPECT_FALSE(pq.next(value, priority));
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

TEST(DequeueTest, DefaultQueue) {
    prqueue<int> pq;
    pq.enqueue(1, 0);
    pq.enqueue(2, -3);
    pq.enqueue(3, -4);
    pq.enqueue(4, -2);
    pq.enqueue(5, -1);
    pq.enqueue(6, 3);
    pq.enqueue(7, 0);
    pq.enqueue(8, -3);
    pq.enqueue(9, -4);
    pq.enqueue(10, -2);
    pq.enqueue(11, -1);
    pq.enqueue(12, 3);
    pq.enqueue(13, 0);
    pq.enqueue(14, -3);
    pq.enqueue(15, -4);
    pq.enqueue(16, -2);
    pq.enqueue(17, -1);
    pq.enqueue(18, 3);

    pq.begin();
    EXPECT_EQ(pq.dequeue(), 3);
}

TEST(DequeueTest, PeekDequeueSmall) {
    prqueue<int> pq;
    int value;
    int priority;

    pq.enqueue(1, 0);
    pq.enqueue(2, -3);      // 4
    pq.enqueue(3, -4);      // 1
    pq.enqueue(4, -2);      // 7
    pq.enqueue(5, -1);
    pq.enqueue(6, 3);
    pq.enqueue(7, 0);
    pq.enqueue(8, -3);      // 5
    pq.enqueue(9, -4);      // 2
    pq.enqueue(10, -2);     // 8            Current node with priority -2 has right child with priority -1
                            //              The right child's parent is not the current node.
    pq.enqueue(11, -1);
    pq.enqueue(12, 3);
    pq.enqueue(13, 0);
    pq.enqueue(14, -3);     // 6
    pq.enqueue(15, -4);     // 3
    pq.enqueue(16, -2);     // 9
    pq.enqueue(17, -1);
    pq.enqueue(18, 3);
    
    pq.begin();
    for (int i = 0; i < pq.size(); i++) {
        EXPECT_TRUE(pq.next(value, priority));
        cout << value << " " << priority << "\t" << i << endl;
    }

    EXPECT_EQ(pq.dequeue(), 3);
    EXPECT_EQ(pq.dequeue(), 9);
    EXPECT_EQ(pq.dequeue(), 15);
    EXPECT_EQ(pq.dequeue(), 2);
    EXPECT_EQ(pq.dequeue(), 8);
    EXPECT_EQ(pq.dequeue(), 14);
    EXPECT_EQ(pq.dequeue(), 4);

    EXPECT_EQ(pq.peek(), 10);

    pq.begin();
    cout << endl << "AFTER DQ: " << endl;
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(pq.next(value, priority));
        cout << value << " " << priority << "\t" << i << endl;
    }
}

TEST(AsStringTest, EmptyQueue) {
    prqueue<string> names;
    ASSERT_EQ(names.as_string(), "");
}

TEST(AsStringTest, OneElement) {
    prqueue<string> names;
    names.enqueue("Zade", 3);
    ASSERT_EQ(names.as_string(), "3 value: Zade\n");
}

TEST(AsStringTest, MultipleElements) {
    prqueue<string> names;
    names.enqueue("Jade", 3);
    names.enqueue("Wade", 2);
    names.enqueue("Zade", 1);
    names.enqueue("Sade", 2);

    stringstream expected;
    expected << "1 value: Zade\n"
             << "2 value: Wade\n"
             << "2 value: Sade\n"
             << "3 value: Jade\n";

    ASSERT_EQ(names.as_string(), expected.str());
}

TEST(ClearTest, OneElement) {
    prqueue<int> pq;
    pq.enqueue(10, 1);

    pq.clear();
    EXPECT_EQ(pq.peek(), 0);
}

TEST(AsStringTest, MultipleDuplicateElements) {
    prqueue<string> names;
    names.enqueue("Zack", 3);
    names.enqueue("Mack", 2);
    names.enqueue("Jack", 1);
    names.enqueue("Isack", 2);
    names.enqueue("Tack", 1);

    stringstream expected;
    expected << "1 value: Jack\n"
             << "1 value: Tack\n"
             << "2 value: Mack\n"
             << "2 value: Isack\n"
             << "3 value: Zack\n";

    EXPECT_EQ(names.as_string(), expected.str());

    names.clear();
    EXPECT_EQ(names.size(), 0);
}

TEST(CopyOpTest, DefaultCopy) {
    prqueue<int> pq;

    pq.enqueue(10, 2);
    pq.enqueue(20, 1);
    pq.enqueue(30, 2);
    pq.enqueue(40, 3);
    pq.enqueue(50, 1);

    prqueue<int> pqCopy(pq);

    EXPECT_EQ(pqCopy.size(), pq.size());
    pqCopy.begin();
    int value;
    int priority;
    
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(pqCopy.next(value, priority));
        //cout << value << ", " << priority << endl;
    }
    EXPECT_FALSE(pqCopy.next(value, priority));
}

TEST(AssignOpTest, DefaultAssignment) {
    // Test assignment operator
    prqueue<string> pq1;
    pq1.enqueue("apple", 3);
    pq1.enqueue("banana", 2);
    pq1.enqueue("orange", 1);

    prqueue<string> pq2;
    pq2.enqueue("grape", 5);
    pq2.enqueue("kiwi", 4);

    pq2 = pq1;
    
    EXPECT_EQ(pq2.size(), pq1.size());
}

TEST(EqualOpTest, EmptyEqualPq) {
    // Test equality operator
    prqueue<string> pq1;
    prqueue<string> pq2;

    EXPECT_TRUE(pq1 == pq2);
}

TEST(EqualOpTest, EqualPq) {
    // Test equality operator
    prqueue<string> pq1;
    pq1.enqueue("apple", 3);
    pq1.enqueue("banana", 2);
    pq1.enqueue("orange", 1);

    prqueue<string> pq2;
    pq2.enqueue("apple", 3);
    pq2.enqueue("banana", 2);
    pq2.enqueue("orange", 1);

    EXPECT_TRUE(pq1 == pq2);
}

TEST(EqualOpTest, NonEqualPq) {
    // Test equality operator
    prqueue<string> pq1;
    pq1.enqueue("apple", 3);
    pq1.enqueue("banana", 2);
    pq1.enqueue("orange", 1);

    prqueue<string> pq2;
    pq2.enqueue("apple", 3);
    pq2.enqueue("banana", 2);

    EXPECT_FALSE(pq1 == pq2);
}

TEST(EqualOpTest, NonEqualPq2) {
    // Test equality operator
    prqueue<int> pq1;
    pq1.enqueue(10, 2);
    pq1.enqueue(20, 1);
    pq1.enqueue(30, 0);


    prqueue<int> pq2;
    pq1.enqueue(20, 1);
    pq1.enqueue(10, 2);
    pq1.enqueue(30, 0);

    EXPECT_FALSE(pq1 == pq2);
}

// Test case for checking next function with empty prqueue
TEST(NextTest, EmptyQueue) {
    prqueue<int> pq;
    int value;
    int priority;
    pq.begin();

    EXPECT_FALSE(pq.next(value, priority));
}

TEST(NextTest, PeekTop) {
    prqueue<int> pq;
    int value;
    int priority;
    pq.enqueue(0, 6);
    pq.enqueue(1, 1);
    pq.enqueue(2, 15);
    pq.begin();

    pq.next(value, priority);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(priority, 1);
}

TEST(NextTest, SmallSomeDupes) {
    prqueue<int> pq;
    int value;
    int priority;
    pq.enqueue(0, 6);       // #4
    pq.enqueue(1, 1);       // #1a
    pq.enqueue(2, 15);      // #6a
    pq.enqueue(3, 2);       // #2
    pq.enqueue(4, 4);       // #3
    pq.enqueue(5, 10);      // #5
    pq.enqueue(6, 20);      // #7
    pq.enqueue(7, 1);       // #1b
    pq.enqueue(8, 15);      // #6b
    pq.enqueue(9, 15);      // #6c
    pq.begin();

    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 1);

    EXPECT_TRUE(pq.next(value, priority));
    EXPECT_EQ(value, 7);
}

TEST(NextTest, NextFunctionality) {
    prqueue<int> pq;

    pq.enqueue(0, 6);
    pq.enqueue(1, 1);
    pq.enqueue(3, 2);
    pq.enqueue(4, 4);
    pq.enqueue(7, 1);

    int value;
    int priority;
    pq.begin();

    // Attempt to get the next value and priority multiple times
    // Should print
    // 1, 1
    // 7, 1
    // 3, 2
    // 4, 4
    // 0, 6
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(pq.next(value, priority));
        //cout << value << ", " << priority << endl;
    }

    EXPECT_FALSE(pq.next(value, priority));
}

TEST(NextTest, SmallNoDupes) {
    prqueue<int> pq;

    pq.enqueue(0, 6);
    pq.enqueue(1, 1);
    pq.enqueue(2, 15);
    pq.enqueue(3, 2);
    pq.enqueue(4, 4);
    pq.enqueue(5, 10);
    pq.enqueue(6, 20);

    int value;
    int priority;
    pq.begin();
    for (int i = 0; i < 7; i++) {
        EXPECT_TRUE(pq.next(value, priority));
        //cout << value << ", " << priority << endl;
    }

    EXPECT_FALSE(pq.next(value, priority));
}

TEST(NextTest, SmallManyDupes) {
    prqueue<int> pq;

    // enqueue(value, priority)
    pq.enqueue(0,    6);
    pq.enqueue(1,    1);
    pq.enqueue(2,   15);
    pq.enqueue(3,    2);
    pq.enqueue(4,    4);
    pq.enqueue(5,   10);
    pq.enqueue(6,   20);
    pq.enqueue(7,    6);
    pq.enqueue(8,    1);
    pq.enqueue(9,   15);
    pq.enqueue(10,   2);
    pq.enqueue(11,   4);
    pq.enqueue(12,  10);
    pq.enqueue(13,  20);
    pq.enqueue(14,   6);
    pq.enqueue(15,   1);
    pq.enqueue(16,  15);
    pq.enqueue(17,   2);
    pq.enqueue(18,   4);
    pq.enqueue(19,  10);
    pq.enqueue(20,  20);

    int value;
    int priority;
    pq.begin();

    for (int i = 0; i < 21; i++) {
        EXPECT_TRUE(pq.next(value, priority));
        //cout << value << " " << priority << "\t" << i << endl;
    }

    EXPECT_FALSE(pq.next(value, priority));

}

TEST(NextTest, LargeRandom) {
    prqueue<int> pq;

    // enqueue(value, priority)
    pq.enqueue(0, -4);
    pq.enqueue(1, 10);
    pq.enqueue(2, -1);
    pq.enqueue(3, 6);
    pq.enqueue(4, 1);
    pq.enqueue(5, -2);
    pq.enqueue(6, 3);
    pq.enqueue(7, 3);
    pq.enqueue(8, -6);
    pq.enqueue(9, 1);
    pq.enqueue(10, 0);
    pq.enqueue(11, -2);
    pq.enqueue(12, 2);
    pq.enqueue(13, -10);
    pq.enqueue(14, -2);
    pq.enqueue(15, 8);
    pq.enqueue(16, 2);
    pq.enqueue(17, -1);
    pq.enqueue(18, 1);
    pq.enqueue(19, -10);
    pq.enqueue(20, 0);
    pq.enqueue(21, -9);
    pq.enqueue(22, -10);
    pq.enqueue(23, 9);
    pq.enqueue(24, -10);
    pq.enqueue(25, -6);
    pq.enqueue(26, 9);
    pq.enqueue(27, 4);
    pq.enqueue(28, 8);
    pq.enqueue(29, 6);
    pq.enqueue(30, 5);
    pq.enqueue(31, -7);
    pq.enqueue(32, 5);
    pq.enqueue(33, -7);
    pq.enqueue(34, 10);
    pq.enqueue(35, -4);
    pq.enqueue(36, 2);
    pq.enqueue(37, 2);
    pq.enqueue(38, 9);
    pq.enqueue(39, 6);
    pq.enqueue(40, -7);
    pq.enqueue(41, -1);
    pq.enqueue(42, -6);
    pq.enqueue(43, 6);
    pq.enqueue(44, 0);
    pq.enqueue(45, 3);
    pq.enqueue(46, 4);
    pq.enqueue(47, -8);
    pq.enqueue(48, -8);
    pq.enqueue(49, -6);
    pq.enqueue(50, -8);
    pq.enqueue(0, -4);
    pq.enqueue(1, 10);
    pq.enqueue(2, -1);
    pq.enqueue(3, 6);
    pq.enqueue(4, 1);
    pq.enqueue(5, -2);
    pq.enqueue(6, 3);
    pq.enqueue(7, 3);
    pq.enqueue(8, -6);
    pq.enqueue(9, 1);
    pq.enqueue(10, 0);
    pq.enqueue(11, -2);
    pq.enqueue(12, 2);
    pq.enqueue(13, -10);
    pq.enqueue(14, -2);
    pq.enqueue(15, 8);
    pq.enqueue(16, 2);
    pq.enqueue(17, -1);
    pq.enqueue(18, 1);
    pq.enqueue(19, -10);
    pq.enqueue(20, 0);
    pq.enqueue(21, -9);
    pq.enqueue(22, -10);
    pq.enqueue(23, 9);
    pq.enqueue(24, -10);
    pq.enqueue(25, -6);
    pq.enqueue(26, 9);
    pq.enqueue(27, 4);
    pq.enqueue(28, 8);
    pq.enqueue(29, 6);
    pq.enqueue(30, 5);/*
    pq.enqueue(31, -7);
    pq.enqueue(32, 5);
    pq.enqueue(33, -7);
    pq.enqueue(34, 10);
    pq.enqueue(35, -4);
    pq.enqueue(36, 2);
    pq.enqueue(37, 2);
    pq.enqueue(38, 9);
    pq.enqueue(39, 6);
    pq.enqueue(40, -7);
    pq.enqueue(41, -1);
    pq.enqueue(42, -6);
    pq.enqueue(43, 6);
    pq.enqueue(44, 0);
    pq.enqueue(45, 3);
    pq.enqueue(46, 4);
    pq.enqueue(47, -8);
    pq.enqueue(48, -8);
    pq.enqueue(49, -6);
    pq.enqueue(50, -8);
    pq.enqueue(51, 2);
    pq.enqueue(52, -5);
    pq.enqueue(53, -7);
    pq.enqueue(54, 0);
    pq.enqueue(55, -4);
    pq.enqueue(56, -2);
    pq.enqueue(57, -1);
    pq.enqueue(58, 10);
    pq.enqueue(59, -5);
    pq.enqueue(60, -6);
    pq.enqueue(61, 5);
    pq.enqueue(62, -1);
    pq.enqueue(63, 10);
    pq.enqueue(64, 8);
    pq.enqueue(65, -2);
    pq.enqueue(66, -5);
    pq.enqueue(67, 0);
    pq.enqueue(68, -10);
    pq.enqueue(69, -6);
    pq.enqueue(70, -5);
    pq.enqueue(71, -6);
    pq.enqueue(72, 3);
    pq.enqueue(73, -1);
    pq.enqueue(74, 10);
    pq.enqueue(75, -7);
    pq.enqueue(76, -9);
    pq.enqueue(77, 3);
    pq.enqueue(78, -5);
    pq.enqueue(79, -6);
    pq.enqueue(80, 8);
    pq.enqueue(81, -2);
    pq.enqueue(82, 6);
    pq.enqueue(83, -7);
    pq.enqueue(84, 1);
    pq.enqueue(85, -4);
    pq.enqueue(86, -1);
    pq.enqueue(87, 9);
    pq.enqueue(88, 5);
    pq.enqueue(89, -1);
    pq.enqueue(90, -6);
    pq.enqueue(91, 10);
    pq.enqueue(92, -7);
    pq.enqueue(93, 3);
    pq.enqueue(94, 10);
    pq.enqueue(95, -9);
    pq.enqueue(96, -9);
    pq.enqueue(97, -6);
    pq.enqueue(98, 1);
    pq.enqueue(99, -8);*/
    int value;
    int priority;
    pq.begin();

    for (int i = 0; i < pq.size(); i++) {
        EXPECT_TRUE(pq.next(value, priority));
        //cout << value << " " << priority << "\t"<< i << endl;
    }

    EXPECT_FALSE(pq.next(value, priority));

}

// Random Equal OP== tests
TEST(EqualOPTest, EqualEmptyQueues) {
    prqueue<string> queue1, queue2;
    ASSERT_TRUE(queue1 == queue2);
}

TEST(EqualOPTest, EqualNonEmptyQueues) {
    prqueue<string> queue1, queue2;
    queue1.enqueue("apple", 5);
    queue1.enqueue("banana", 3);
    queue2.enqueue("apple", 5);
    queue2.enqueue("banana", 3);
    ASSERT_TRUE(queue1 == queue2);
}

TEST(EqualOPTest, NotEqualDifferentPriorities) {
    prqueue<string> queue1, queue2;
    queue1.enqueue("apple", 5);
    queue2.enqueue("apple", 3);
    ASSERT_FALSE(queue1 == queue2);
}

TEST(EqualOPTest, NotEqualDifferentValues) {
    prqueue<string> queue1, queue2;
    queue1.enqueue("apple", 5);
    queue2.enqueue("banana", 5);
    ASSERT_FALSE(queue1 == queue2);
}

TEST(EqualOPTest, NotEqualDifferentSizes) {
    prqueue<string> queue1, queue2;
    queue1.enqueue("apple", 5);
    ASSERT_FALSE(queue1 == queue2);
}

TEST(EqualOPTest, NotEqualDifferentStructure) {
    prqueue<string> queue1, queue2;
    queue1.enqueue("apple", 5);
    queue1.enqueue("banana", 3);
    queue2.enqueue("apple", 3);
    queue2.enqueue("banana", 5);
    ASSERT_FALSE(queue1 == queue2);
}
