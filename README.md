## 1. PL/0扩展输入文法

```
        /*Lexer*/
<字符串> ::= <字符>{<字符>}
<字符> ::= <加减运算符> | <乘除运算符> | <字母> | <数字>
<字母> ::= a|b|...X|Y|Z
<整数> ::= [+ | -]<无符号整数>
<无符号整数> ::= <非零数字>{<数字>} | 0
<数字> ::= <非零数字> | 0
<非零数字> :: = 1|2|...|9
<加减运算符> ::= + | -
<乘除运算符> ::= * | /
<关系运算符> ::= > | >= | < | <= | == | !=



        /*Parser*/
<程序> ::= [<常量声明>][<变量声明>][<函数定义>]<主函数>

<常量声明> ::= const<常量定义>;{<常量定义>;}
<常量定义> ::= int<标识符>=<整数>{, <标识符>=<整数>} | char<标识符>=<字符>{, <标识符>=<字符>}
<变量声明> ::= <变量定义>;{变量定义;}
<变量定义> ::= int(<标识符> | <标识符>'['<无符号整数>']'){, (<标识符> | <标识符>'['<无符号整数>']')}
| char(<标识符> | <标识符>'['<无符号整数>'>'){, (<标识符> | <标识符>'['<无符号整数>']')}
<标识符> ::= <字母>{<字母> | <数字>}
<类型说明符> ::= int | char 
<函数定义> ::= {<有返回值函数定义> | <无返回值函数定义>}
<有返回值函数定义> ::= <声明头部>'('<参数表>')''{'<复合语句>'}'
<无返回值函数定义> ::= void<标识符>'('<参数表>')''{'<复合语句>'}'
<声明头部> ::= int<标识符> | char <标识符>
<参数表> ::= <参数>{, <参数>} | <空>
<参数> ::= <类型标识符><标识符>

<主函数> ::= int main'('')''{'<复合语句>'}'
<复合语句> ::= [<常量说明>][<变量说明>]{<语句>}
<语句> ::= <条件语句> | <循环语句> | <有返回值函数调用语句>; | <无返回值函数调用语句>; | <赋值语句>; | <读语句>; | <写语句>; | <空>; | <返回语句>;
<条件语句> ::= if'('<条件>')'<语句>
<条件> ::= <表达式><关系运算符><表达式> | <表达式>
<循环语句> ::= while'('<条件>')'<语句>
<赋值语句> ::= <标识符>=<表达式> | <标识符>'['<表达式>']'=<表达式>
<有返回值函数调用语句> ::= <标识符>'('<值参数表>')'
<无返回值函数调用语句> ::= <标识符>'('<值参数表>')'
<值参数表> ::= <表达式>{, <表达式>} | <空>

<表达式> ::= [+ | -]<项>{<加减运算符><项>} 
<项> ::= <因子>{<乘除运算符><因子>}
<因子> ::= <标识符>['('<值参数表>')'] | <标识符> '['<表达式>']' | <整数> | <字符>
<读语句> ::= scanf'('<标识符>{, <标识符>}')'
<写语句> ::= printf'('<字符串>')' | printf '('<表达式>')'
<返回语句> ::= return['('<表达式>')']
```
##### 文法现限制细则

* 文法字符表: {字母，数字，+ - \* \/}的克林闭包  
* 数据类型: 32位有符号整数   数组元素下标为无符号整数>0  
* 内置类型: int char int[], char[]
* 函数需要有参数名，参数类型支持混合声明  <错误样例>: int func(int [], char, ) {}
* 语句: if, while, =, func, 
* 补待充: 丰富数据类型， for, switch
* 变量声明时均不可直接赋值，全局变量由系统初始化为0，局部变量由赋值语句完成  <错误样例>: int a = 9;

  

​    



## 2. SDT属性翻译文法  

  

**note: gitpages can't parse *Latex* completely, please download to local.**

##### 2.1 常量声明语句

<const del>  $\rightarrow$  const  <type>$\uparrow$t   <entity>$\uparrow$n   =  <const expr>$\uparrow$c, s   **@insert $\downarrow$t, n, c, s;**

<type> $\uparrow$ t    $\rightarrow$   int$\uparrow$t  |   char$\uparrow$t   

<const expr>$\uparrow$c, s   $\rightarrow$  <integer const>$\uparrow$c,s   | <char const>$\uparrow$c,s

 

##### 2.2 变量声明语句

<svar del> $\rightarrow$   <type> $\uparrow$t, i  <entity>$\uparrow$n  **@svardef $\downarrow$t,i,n   @allocsv$\downarrow$i ;**

<type> $\rightarrow$  int$\uparrow$t,i | char$\uparrow$t,i.   



##### 2.3 数组变量声明语句

<array del> $\rightarrow$  <type>$\uparrow$t  <entity>$\uparrow$n  [<dem>$\uparrow$j,k] **@symbinsert$\downarrow$j, k, n, t;**

<type>  $\rightarrow$   int$\uparrow$t | char$\uparrow$t

<dem> $\rightarrow$ int$\uparrow$j,k.

  

##### 2.4 表达式语句处理

<expression> $\rightarrow$ <expr>

<expr> $\uparrow$t  $\rightarrow$ <term>$\uparrow$s  <terms>$\downarrow$s $\uparrow$t  

<terms> $\downarrow$s $\uparrow$u $\rightarrow$  **@echo$\downarrow$s$\uparrow$u**

​				 |  + <term>$\uparrow$t **@add$\downarrow$t,s  $\uparrow$v**   <terms>$\downarrow$v $\uparrow$u

​    			 |  -  <term>$\uparrow$t **@sub$\downarrow$t,s  $\uparrow$v **   <terms>$\downarrow$v $\uparrow$u

<term>$\uparrow$u  $\rightarrow$ <factor>$\uparrow$s  <factors>$\downarrow$s $\uparrow$u

<factors> $\downarrow$s $\uparrow$u $\rightarrow$  **@echo$\downarrow$s$\uparrow$u**

​				 |  * <factor>$\uparrow$t **@mul$\downarrow$t,s  $\uparrow$v**   <factors>$\downarrow$v $\uparrow$u

​    			 |  / <factor>$\uparrow$t  **@div$\downarrow$t,s  $\uparrow$v **   <factors>$\downarrow$v $\uparrow$u

<factor> $\uparrow$t   $\rightarrow$  <avriable>$\uparrow$i **@type$\downarrow$i$\uparrow$t **

​				  |  <integer>$\uparrow$i **@pushi$\downarrow$i$\uparrow$t**

​				   |  <char>$\uparrow$c  **@push$\downarrow$c$\uparrow$t**

 

##### 2.5 条件语句处理

<if_stat> $\rightarrow$  <if_head>$\uparrow$y <if_tail>$\downarrow$y

<if_head> $\rightarrow$  IF <log_expr>**@brf$\uparrow$y** THEN <stat_list>

<if_tail> $\downarrow$y  $\rightarrow$  **@labprod$\downarrow$y**

​							| ELSE **@br$\uparrow$z** **@labprod$\downarrow$y**  <stat_list> **@labprod$\downarrow$z**











































