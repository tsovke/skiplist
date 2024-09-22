#include <gtest/gtest.h>
#include "../src/skiplist.h" // 假设这是包含SkipList定义的头文件路径

// 定义测试用例
TEST(SkipListTest, InsertAndSearch) {
    SkipList<int, std::string> list;
    ASSERT_TRUE(list.insert(1, "one"));
    ASSERT_TRUE(list.insert(2, "two"));

    auto *node1 = list.search(1);
    auto *node2 = list.search(2);
    auto *node3 = list.search(3);

    ASSERT_NE(node1, nullptr);
    ASSERT_EQ(node1->GetValue(), "one");

    ASSERT_NE(node2, nullptr);
    ASSERT_EQ(node2->GetValue(), "two");

    ASSERT_EQ(node3, nullptr);
}

TEST(SkipListTest, Remove) {
    SkipList<int, std::string> list;
    ASSERT_TRUE(list.insert(1, "one"));
    ASSERT_TRUE(list.insert(2, "two"));

    ASSERT_TRUE(list.remove(1));
    ASSERT_TRUE(list.remove(2));

    auto *node1 = list.search(1);
    auto *node2 = list.search(2);

    ASSERT_EQ(node1, nullptr);
    ASSERT_EQ(node2, nullptr);

    // 尝试删除不存在的键
    ASSERT_FALSE(list.remove(3));
}

TEST(SkipListTest, DisplayList) {
    SkipList<int, std::string> list;
    list.insert(1, "one");
    list.insert(2, "two");

    // 这里只是调用displayList函数，实际测试可能需要捕获输出或手动检查控制台输出
    list.displayList();

    // 对于显示列表的功能，通常需要通过捕获stdout来验证输出内容
    // 但在这个简单的例子中，我们假设函数没有崩溃即视为通过
    SUCCEED(); // 表示这个测试成功
}

TEST(SkipListTest, DumpFileAndLoadFile) {
    SkipList<int, std::string> list;
    ASSERT_TRUE(list.insert(1, "one"));
    ASSERT_TRUE(list.insert(2, "two"));

    list.dumpFile();
    list.loadFile();

    auto *node1 = list.search(1);
    auto *node2 = list.search(2);

    ASSERT_NE(node1, nullptr);
    ASSERT_EQ(node1->GetValue(), "one");

    ASSERT_NE(node2, nullptr);
    ASSERT_EQ(node2->GetValue(), "two");
}

// 主函数，运行所有测试
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
