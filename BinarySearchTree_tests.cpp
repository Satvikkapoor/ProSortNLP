// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(tree_empty) {
    BinarySearchTree<int> val;

    ASSERT_TRUE(val.begin() == val.end());

    //empty
    ASSERT_TRUE(val.empty());

    //insert
    val.insert(3);
    ASSERT_TRUE(!val.empty());

}

TEST(tree_size){
    BinarySearchTree<int> val;
    ASSERT_TRUE(val.size() == 0);

    val.insert(2);
    ASSERT_TRUE(val.size() == 1);
    val.insert(5);
    ASSERT_TRUE(val.size() == 2);
    val.insert(4);
    ASSERT_TRUE(val.size() == 3);
    val.insert(18);
    ASSERT_TRUE(val.size() == 4);

}

TEST(tree_height){
    BinarySearchTree<int> val;
    ASSERT_TRUE(val.height() == 0);

    val.insert(6);
    ASSERT_TRUE(val.height() == 1);
    val.insert(4);
    ASSERT_TRUE(val.height() == 2);
    val.insert(3);
    ASSERT_TRUE(val.height() == 3);
    val.insert(2);
    ASSERT_TRUE(val.height() == 4);
    val.insert(18);
    ASSERT_TRUE(val.height() == 4);

}

TEST(tree_sorting){
    BinarySearchTree<int> val;
    ASSERT_TRUE(val.check_sorting_invariant());

    val.insert(5);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(9);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(6);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(4);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(20);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(14);
    ASSERT_TRUE(val.check_sorting_invariant());
    val.insert(0);
    ASSERT_TRUE(val.check_sorting_invariant());
   *val.begin() = 8;
   ASSERT_FALSE(val.check_sorting_invariant());
}

TEST(tree_ref){
    BinarySearchTree<int> val;
    //max min
    ASSERT_TRUE(val.max_element() == val.end());
    ASSERT_TRUE(val.min_element() == val.end());
    ASSERT_TRUE(val.min_greater_than(0) == val.end());

    val.insert(5);
    ASSERT_TRUE(val.min_greater_than(0) == val.begin());
    val.insert(9);
    val.insert(6);
    val.insert(4);
    val.insert(20);
    val.insert(14);
    val.insert(0);
    std::cout << "cout << tree.to_string()" << std::endl;
    std::cout << val.to_string() << std::endl << std::endl;
    ASSERT_TRUE(val.height() == 4);

    auto ref = val.begin();
    //045691420
    ASSERT_TRUE(*ref == 0);
    ASSERT_TRUE(val.min_element() == ref);
    ref++;
    ASSERT_TRUE(*ref == 4);
    ref++;
    ASSERT_TRUE(*ref == 5);
    ASSERT_TRUE(val.find(5) == ref);
    ref++;
    ASSERT_TRUE(*ref == 6);
    ASSERT_TRUE(val.min_greater_than(5) == ref);
    ref++;
    ASSERT_TRUE(*ref == 9);
    ASSERT_TRUE(val.min_greater_than(8) == ref);
    ref++;
    ASSERT_TRUE(*ref == 14);
    ASSERT_TRUE(val.find(14) == ref);
    ref++;
    ASSERT_TRUE(*ref == 20);
    ASSERT_TRUE(val.max_element() == ref);
    ASSERT_TRUE(val.find(20) == ref);

    ASSERT_TRUE(val.find(0) == val.begin());

    ostringstream oss_preorder;
    val.traverse_preorder(oss_preorder);
    std::cout << "preorder" << std::endl;
    std::cout << oss_preorder.str() << std::endl << std::endl;
    ASSERT_TRUE(oss_preorder.str() == "5 0 4 6 9 14 20 ");

    ostringstream oss_inorder;
    val.traverse_inorder(oss_inorder);
    std::cout << "inorder" << std::endl;
    std::cout << oss_inorder.str() << std::endl << std::endl;
    ASSERT_TRUE(oss_inorder.str() == "0 4 5 6 9 14 20 ");
}

TEST(tree_rand){
    BinarySearchTree<int> val;
    ASSERT_TRUE(val.begin() == val.end());
    val.insert(9);
    ASSERT_TRUE(*val.begin() == 9);

}

TEST(tree_copy){
    BinarySearchTree<int> val;
    BinarySearchTree<int> val1;

    val.insert(9);
    val.insert(6);
    val.insert(4);
    val.insert(20);
    val.insert(14);
    val.insert(0);

    BinarySearchTree<int> val2(val);
    val1 = val;

    auto ref = val.begin();
    auto ref1 = val1.begin();
    auto ref2 = val2.begin();

    for(int i = 0; i < 6; i++){
        ASSERT_EQUAL(*ref, *ref1);
        ASSERT_EQUAL(*ref1, *ref2);
        ref++;
        ref1++;
        ref2++;
    }
}


TEST_MAIN()
