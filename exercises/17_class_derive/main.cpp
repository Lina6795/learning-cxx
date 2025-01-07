#include "../exercise.h"

// READ: 派生类 <https://zh.cppreference.com/w/cpp/language/derived_class>

// 静态全局变量i，用于记录构造和析构函数的调用次数
static int i = 0;

struct X {
    int x;

    // 构造函数，接受一个整型参数
    X(int x_) : x(x_) {
        std::cout << ++i << ". " << "X(" << x << ')' << std::endl;
    }
    // 拷贝构造函数
    X(X const &other) : x(other.x) {
        std::cout << ++i << ". " << "X(X const &) : x(" << x << ')' << std::endl;
    }
    // 析构函数
    ~X() {
        std::cout << ++i << ". " << "~X(" << x << ')' << std::endl;
    }
};
struct A {
    int a;

    A(int a_) : a(a_) {
        std::cout << ++i << ". " << "A(" << a << ')' << std::endl;
    }
    A(A const &other) : a(other.a) {
        std::cout << ++i << ". " << "A(A const &) : a(" << a << ')' << std::endl;
    }
    ~A() {
        std::cout << ++i << ". " << "~A(" << a << ')' << std::endl;
    }
};
// 派生类B，继承自A
struct B : public A {
    X x;

    // 先调用基类A的构造函数，再调用成员变量x的构造函数
    B(int b) : A(1), x(b) {
        std::cout << ++i << ". " << "B(" << a << ", X(" << x.x << "))" << std::endl;
    }
    // 先调用基类A的拷贝构造函数，再对自身包含的X类型的成员变量x进行拷贝构造
    B(B const &other) : A(other.a), x(other.x) {
        std::cout << ++i << ". " << "B(B const &) : A(" << a << "), x(X(" << x.x << "))" << std::endl;
    }
    // 释放B类型对象生命周期结束时被调用
    ~B() {
        std::cout << ++i << ". " << "~B(" << a << ", X(" << x.x << "))" << std::endl;
    }
};

int main(int argc, char **argv) {
    X x = X(1);
    A a = A(2);
    B b = B(3);

    // TODO: 补全三个类型的大小
    static_assert(sizeof(X) == sizeof(int), "There is an int in X");
    static_assert(sizeof(A) == sizeof(int), "There is an int in A");
    static_assert(sizeof(B) == sizeof(int) + sizeof(X), "B is an A with an X");

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    // 这是不可能的，A 无法提供 B 增加的成员变量的值
    // B ba = A(4);

    // 这也是不可能的，因为 A 是 B 的一部分，就好像不可以把套娃的外层放进内层里。
    A ab = B(5);// 然而这个代码可以编译和运行！
    // THINK: 观察打印出的信息，推测把大象放进冰箱分几步？
    // THINK: 这样的代码是“安全”的吗？
    // NOTICE: 真实场景中不太可能出现这样的代码

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    return 0;
}
