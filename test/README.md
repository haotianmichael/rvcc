#### LL(1)文法测试样例



| 测试文件                    | 测试文法           | 备注                                                         |
| --------------------------- | ------------------ | ------------------------------------------------------------ |
| **constTestBench.c**        | **<常量说明\>​**    | 以`const`开头的`int`,`char`内置类型;支持连续定义;            |
| **varTestBench.c**          | **<变量说明>**     | 全局的`int`,`char`类型的变量和数组，支持连续定义；支持混合声明； |
| **funcTestBench.c**         | **<函数说明>**     | 参数表支持混合声明`int func(int a, int b[10], char c[]){}`;支持连续定义; |
| **conditionTestBench.c**    | **<条件语句说明>** | 仅仅支持`if(){}else{}`格式；                                 |
| **loopTestBench.c**         | **<循环语句说明>** | 格式`while(){}`；                                            |
| **scanfTestBench.c**        | **<读语句>**       | 格式`scanf(identA, identB, ...);`                            |
| **printfTestBench.c**       | **<写语句>**       | 格式`printf(<字符串常量> | <表达式>);`                       |
| **compoundStmtTestBench.c** | **<语句>**         | 支持多语句的函数定义;`main`完成定义；                        |
|                             |                    |                                                              |
|                             |                    |                                                              |
|                             |                    |                                                              |
|                             |                    |                                                              |
|                             |                    |                                                              |
|                             |                    |                                                              |





