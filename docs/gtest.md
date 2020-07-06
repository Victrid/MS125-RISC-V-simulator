# Gtest

Gtest, short for Google test, is a xUnit style test toolset.

Gtest have a ease learning curve. To run your first gtest-powered unit test, just install gtest, and include gtest/gtest.h in your test file.

Add test unit using the macro `TEST(test_case_name, test_name){...}`, and run tests in the main function:

```cpp
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```