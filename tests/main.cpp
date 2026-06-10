#include <vector>
#include <chrono>

#include "Test.h"

int main()
{
    Test test;

    test.create();

    test.start();

    test.destroy();
}