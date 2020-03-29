## SDT属性翻译文法  

  

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





##### 2.6 赋值语句处理

<assignstat> $\rightarrow$  **@setL$\downarrow$L**  <variable> $\downarrow$L$\uparrow$t

​								  **@resetL$\uparrow$L** <expr>$\uparrow$ **@stroin$\downarrow$t, s**









































