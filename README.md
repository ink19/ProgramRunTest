### 说明
基于libuv的一个多进程程序测试工具，可以快速便捷的批量对一个程序进行不同参数的

### 功能
1. 限制并记录程序运行时间。可以通过添加参数限制程序的运行时间，并且将每一次的运行时间都记录在runtime.data文件；
2. 记录程序的stdout和stderr输出。通过重定向，将程序的输出重定向到了err和out文件夹。所以在一开始就要建立这两个文件来保存数据；
3. 多进程运行程序。可以自定义设置进程数，减短测试时间；

### 编译方法
#### 依赖
1. libuv 1.34
2. ncurses 5.9

#### 编译步骤
1. make


### 使用方法
比如
```
./TaskRunLimit -s 20 -p 2 -t 10000 ./TestProgram.bin loop_arg
```
`-s` 测试样例数

`-p` 进程数

`-t` 限制时间（毫秒）

接下来就是测试程序和测试程序的参数，需要循环变量的参数设置为`loop_arg`