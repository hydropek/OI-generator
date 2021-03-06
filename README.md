# OI-generator
A useful and convient generator for OI

## demo
```cpp
#include "generator.h"
int main() {
    using namespace gen;
    // 生成一个长度为 10，值域为 [-20, 20] 的数组
    auto a = generate_list(10, {-20, 20});//此时 a 的类型是vector<long long>
    // 生成一个长度为 10，值域为 [-20, 20] 的数组，互不重复
    auto b = generate_list(10, {-20, 20}, limits::unique);
    // 生成一个长度为 10，值域为 [-20, 20] 的数组，单调不升
    auto c = generate_list(10, {-20, 20}, limits::decrease);
    // 生成一个长度为 10，值域为 [-20, 20] 的数组，单调递增
    auto d = generate_list(10, {-20, 20}, limits::increase | limits::unique);
    // 在一行输出一个数组
    print(a);
    // 输出多个数组
    print(b, c, d);

    // 一行输出多个数
    int x = 10, y = -1, z = 5;
    print(x, y, '\n');

    // 生成一颗节点大小为 10 的树，两种方法等价
    tree t1(10);
    auto t2 = generate_random_tree(10);
    // 生成一颗节点大小为 10 的树，边权值域为 [-20, 20]，两种方法等价
    tree t3(10, {-20, 20});
    auto t4 = generate_random_tree(10, {-20, 20});
    // 生成一颗节点大小为 10 的树，边权值域为 [-20, 20]，权值互不重复，两种方法等价
    tree t5(10, {-20, 20}, limits::unique);
    auto t6 = generate_random_tree(10, {-20, 20}, limits::unique);
    // 输出
    print(t1, t2, t3, t4, t5, t6);

    using ll = long long;
    // 随机数，值域 [-20, 20]
    ll v1 = randint(-20, 20);
    // 随机数，值域 [0, 10]
    ll v2 = randint(10);
    // 随机数，值域 [-1e12, 1e12]
    ll v3 = randint(-1e12, 1e12);
    print(v1, v2, v3, '\n');
}
```

## demo(abbreviation)
```cpp
#include "generator.h"
int main() {

    // abbreviation of namespace gen
    using namespace gen_abbr;

    auto a = gl(10, {-20, 20});
    auto b = gl(10, {-20, 20}, lmt::unq);
    auto c = gl(10, {-20, 20}, lmt::dec);
    auto d = gl(10, {-20, 20}, lmt::inc | lmt::unq);

    pr(a), pr(b, c, d);

    int x = 10, y = -1, z = 5;
    pr(x, y, '\n');

    tree t1(10);
    auto t2 = grt(10);
    tree t3(10, {-20, 20});
    auto t4 = grt(10, {-20, 20});
    tree t5(10, {-20, 20}, lmt::unq);
    auto t6 = grt(10, {-20, 20}, lmt::unq);
    pr(t1, t2, t3, t4, t5, t6);

    using ll = long long;
    ll v1 = ri(-20, 20);
    ll v2 = ri(10);
    ll v3 = ri(-1e12, 1e12);
    print(v1, v2, v3, '\n');

    return 0;
}
```
