引用自：[官方文档math/rand](https://golang.org/pkg/math/rand/)

生成随机数首先要初始化随机种子，不然每次生成都是（指每次重新开始）相同的数，例如：

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190723230255.png)

**初始化随机种子函数**

```go
func Seed(seed int64)
```

> Seed uses the provided seed value to initialize the default Source to a deterministic state. If Seed is not called, the generator behaves as if seeded by Seed(1). Seed values that have the same remainder when divided by 2^31-1 generate the same pseudo-random sequence. Seed, unlike the Rand.Seed method, is safe for concurrent use.

大概的意思是：系统每次都会先用Seed函数初始化系统资源，如果用户不提供seed参数，则默认用seed=1来初始化，这就是为什么每次都输出一样的值的原因

正确的应该是用一个不确定数来初始化随机种子，拉达到随机数随机分布的效果，unix时间就是一个很好的助手

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190724094140.png)

**Intn**函数

```go
func Intn(n int) int
```

> Intn returns, as an int, a non-negative pseudo-random number in [0,n) from the default Source. It panics if n <= 0.

**更多函数请参考官方文档**

