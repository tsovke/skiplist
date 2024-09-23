#include <iostream>
#include "node.h"

// 测试用例
void testNodeCreation() {
    using Node = SkipListKV::Node<int, std::string>;
    
    // 创建一个具有特定键值对和层级的节点
    auto node = std::make_shared<Node>(10, "value", 3);

    // 检查键值对和层级是否正确设置
    if (node->key == 10 && node->value == "value" && node->level == 3) {
        std::cout << "节点创建成功." << std::endl;
    } else {
        std::cout << "节点创建失败." << std::endl;
    }

    // 检查forward向量是否正确初始化
    bool isForwardInitializedCorrectly = true;
    for (auto& next : node->forward) {
        if (next != nullptr) {
            isForwardInitializedCorrectly = false;
            break;
        }
    }
    if (isForwardInitializedCorrectly) {
        std::cout << "forward向量正确初始化为nullptr." << std::endl;
    } else {
        std::cout << "forward向量初始化错误." << std::endl;
    }
}

int main() {
    testNodeCreation();
    return 0;
}
