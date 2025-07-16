// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

module;

#include "../ScopedTimer/ScopedTimer.h"

module modern_cpp:folding;

namespace Folding_Seminar {

    //template <typename ... TArgs>
    //[[ nodiscard ]] int addierer(TArgs ... args)
    //{
    //    auto sum = ( ...  + args );

    //    return sum;
    //}

    // C++ 20

    int addierer(auto ... args)
    {
        auto sum = ( ...  + args );

        return sum;
    }

    template <typename ... TArgs>
    int subtrahierer (TArgs ... args)
    {
        // auto result = (... - args);   // (... op pack)

        auto result = (args - ...); // (pack op ...)

        return result;
    }

    template <typename ... TArgs>
    void printer(TArgs ... args)
    {
        // (((init op pack1) op pack2) op ...) op packN
        // (((std::cout << pack1) << pack2) <<...) << packN
        // (std::cout << args1) << args2 << args3 << args4 .......

        (std::cout << ... << args);
    }

    void test_folding () {

        auto result = addierer(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        // Klammerung von links nach rechts oder von rechts nach links
        // (1 - 2) - 3 = -4
        // 1 - (2 - 3) = +2 

        result = subtrahierer(1, 2, 3);

        printer(1, 2, 3, 4, 5, 6, 7, 8);
    }

}

namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (... , (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft (1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        
        auto list = { values ... };   // { 1, 2, 3, 4 }

        /*volatile*/ auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

#ifdef _DEBUG
    constexpr size_t MaxIterations = 10'000'000;     // debug
#else
    constexpr size_t MaxIterations = 100'000'000;    // release
#endif

    static void test_06_benchmark_folding() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void test_06_benchmark_iterating() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static size_t test_06_benchmark_folding_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }

    static size_t test_06_benchmark_iterating_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }
}

void main_folding()
{
    //Folding_Seminar::test_folding();
    //return;

    using namespace Folding;
    //test_01();
    //test_02();
    //test_03a();
    //test_03b();
    //test_03c();
    //test_03d();
    //test_04();
    //test_05();
    
    // Benchmarks: need to switch to nano seconds! 
    // Optimizer in Release mode is very aggressive
    test_06_benchmark_folding();
    test_06_benchmark_iterating();
    size_t result1{ test_06_benchmark_folding_02() };
    size_t result2{ test_06_benchmark_iterating_02() };
    std::println("Result1: {}", result1);
    std::println("Result2: {}", result2);
}

// =====================================================================================
// End-of-File
// =====================================================================================
