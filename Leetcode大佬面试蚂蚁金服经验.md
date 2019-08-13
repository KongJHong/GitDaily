 GO语言使用和语言特性：协程细节，垃圾回收，channel细节，接口，以及 panic，recover，defer，闭包，反射 等等，以及设计模式基础及简单

的Go代码实现。

索引与存储，B/B+，跳表，红黑树，LSM啥啥啥的。

分布式存储分片方案（普通哈希，一致性哈希等等）与一致性共识机制（2PC，3PC，viewstamp，paxos，raft等等）。

分布式基础知识概念：FLP，CAP，BASE，lamport timestamp（logic clock）与vector clock以及version vector等等。

分布式KV相关的实现细节，raft共识层和KV层，以及boltDB和自写跳表的一些细节，并发编程的共享数据/操作的原子性保持，有锁编程和无锁编程以及锁的粒度等处理细节，宕机恢复和快照持久化的实现。

raft和paxos的基本原理，raft的实现细节及实现过程中的坑，raft和paxos的对比，例如raft和paxos的日志处理方式的优劣（例如乱序提交），multi paxos和multi raft的基本概念。

拜占庭网络和非拜占庭网络下的分布式系统问题的区别，拜占庭容错的共识算法（如POW POS PBFT）和非拜占庭容错的共识算法（如RAFT PAXOS），区块链问题和普通分布式系统问题在这两种情况下的举例，相应的分布式系统安全问题。

REDIS，ETCD，HDFS，CEPH等等数据存储相关的工具的底层基础知识及使用，SQL基础知识，数据库基础概念如ACID，四大隔离级别，脏读 不可重复读 幻读，基础SQL语句。

布隆过滤器，位图，mapreduce，虚拟内存和页面置换算法基础知识，操作系统死锁/活锁概念和条件，TCP/UDP基础知识，HTTP基础知识，topK问题，惊群效应，银行家算法等等。

中间件基本概念，微服务基本概念，DOCKER与K8S基本概念，RPC基础概念和实现，主从复制，读写分离，服务注册与发现，消息中间件，分布式缓存，分布式锁，分布式ID，分布式幂等性，熔断，限流，降级等等，

区块链系统实现细节，区块链理论知识和一些paper，区块链相关的分片，状态通道，plasma，环签名，同态，零知识证明等等，自己对区块链的看法，觉得区块链有哪些问题需要解决。

一些开放性系统设计题，算法题，智力题

![](https://kongjhong-image.oss-cn-beijing.aliyuncs.com/img/20190513164516.png)