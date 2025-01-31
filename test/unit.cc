#include "gtest/gtest.h"
#include "../falloc.h"

// Test fixture for setting up and tearing down tests (if needed)
class StackAllocateTest : public ::testing::Test {
protected:
    void SetUp() override {

    }

    void TearDown() override {

    }
};

// Test case 1: Test the stack creation with size 1024 and alignment b64
TEST_F(StackAllocateTest, CreateStack) {
    stack_allocate_t *alloc = stack_create(1024, b64);
    
    ASSERT_NE(alloc, nullptr); 
    EXPECT_EQ(alloc->stack_size, 1024); 
    EXPECT_EQ(alloc->alignment, b64); 
}

// Test case 2: Test stack allocation with valid size (size 1024 bytes, alignment b64)
TEST_F(StackAllocateTest, StackAllocate) {
    stack_allocate_t *alloc = stack_create(1024, b64);

    memblk allocated_block = stack_alloc(alloc, 512);
    
    ASSERT_NE(allocated_block.memptr, nullptr); 
    EXPECT_EQ(allocated_block.size, 512); 
}

// Test case 3: Test stack allocation fails due to exceeding the stack size
TEST_F(StackAllocateTest, StackAllocateFail) {
    stack_allocate_t *alloc = stack_create(1024, b64);

    memblk allocated_block = stack_alloc(alloc, 2048);
    
    EXPECT_EQ(allocated_block.memptr, nullptr);  // Allocation should fail
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

