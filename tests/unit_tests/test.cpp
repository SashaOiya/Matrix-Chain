//#include <gtest/gtest.h>
//
//#include <vector>
//
//#include "hwt.hpp"
//
//TEST(AVL_tree, copy_ctor) {
//    avl_tree::SearchTree<int> tree = {1, 2, 3, 4, 6, 7, 8, 12, 1};
//    avl_tree::SearchTree<int> lhs = tree;
//
//    EXPECT_TRUE(tree.size() == lhs.size());
//    for (auto itt = tree.begin(), itt_lhs = lhs.begin(), end = tree.end(); itt != end;
//         ++itt, ++itt_lhs) {
//        EXPECT_EQ(*itt, *itt_lhs);
//    }
//}