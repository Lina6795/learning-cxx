#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // TODO: 实现移动构造器
    // 实现移动构造器（浅拷贝）,将源对象的资源窃取过来，避免不必要的内存分配和数据复制
    // 1. 通过初始化列表将传入的右值引用对象 other 的 cache 指针和 cached 值直接 “拿” 过来赋值给当前正在构造的对象的对应成员变量，实现了资源的快速转移，避免了重新分配内存和复制数据的开销。
    // 2. 将源对象的 cache 指针和 cached 值置空，避免了源对象析构时重复释放资源。
    DynFibonacci(DynFibonacci && others) noexcept : cache(others.cache), cached(others.cached) {
        others.cache = nullptr;
        others.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci && others) noexcept {
        if (this != &others) {
            delete[] cache;  // 先释放当前对象的cache数组内存（防止内存泄漏，因为要接管新的资源）
            cache = others.cache;  // 然后将传入的右值引用对象 other 的 cache 指针和 cached 值直接 “拿” 过来赋值给当前对象的对应成员变量，实现了资源的快速转移，避免了重新分配内存和复制数据的开销。
            cached = others.cached;
            others.cache = nullptr; // 将源对象的 cache 指针和 cached 值置空，避免了源对象析构时重复释放资源。
            others.cached = 0;
        }
        return *this;  // 首先通过 if (this!= &other) 判断是否是将对象移动赋值给自身，如果是自身则不做任何操作直接返回当前对象引用（避免错误释放内存等问题）。
    };

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci(){
        delete[] cache;
    };

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        for (; cached<=i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
