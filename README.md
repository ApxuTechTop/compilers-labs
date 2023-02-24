

# compilers-1-ApxuTechTop
## Lab1

 1.
 a. S -> **~~T~~ - S** -> **~~F~~** - S -> **a** - ~~S~~ -> a - **~~T~~ + S** -> a - **~~F~~ * T** + S -> a - **b** * ~~T~~ + S -> a - b * **~~F~~** + S -> a - b * **a** + ~~S~~ -> a - b * a + **~~T~~** -> a - b * a + **~~F~~** -> a - b * a + **b**
   
 b. S -> **a *~~S~~* B C** -> a**a ~~S~~ B C** B C -> aa**ab ~~C~~** ~~B~~ C B C -> aaa~~b~~ **~~B~~ C** C B C -> aaa**bb** C ~~C B~~ C -> aaabb ~~C~~ **~~B~~ C** C -> aaab~~b~~ **~~B~~ C** C C -> aaab**b~~b~~** ~~C~~ C C -> aaabb**b~~c~~** ~~C~~ C -> aaabbb**c~~c~~** ~~C~~ -> aaabbbc**cc**
 
 2. 
 
a. 
  
```sh
S : A B C;
A : 'a' | 'a' A;
B : 'b' | 'b' B;
C : 'c' | 'c' C;
```

b.

```sh
S : A B;
A : '0' | '0' A | ;
B : '10' | '10' B | ;
```

c. 
    
```sh
S : '0' S '0' | '1' S '1' | '00' | '11'
```
3.
a. Тип 1 Контекстно-зависимая грамматика, так как в условии `'0' A : '00' A '1'` есть контекст `'0'`
b. Тип 2 контекстно-свободная грамматика

4.
```sh
S : 'a' S | 'b' S | 'a' | 'b';
```

5.
```sh
S : '0' A;
S : '1' A;
A : '0' A;
A : '1' A;
A : '.' B;
B : '0' C;
B : '1' C;
C : '0' C;
C : '1' C;
C : ε;
```

6.
a. 
```sh
1. ([A-Za-z_])([A-Za-z_0-9])*
2. _a123b 134sas a _ a1 _2 _2_
3. regexr.com/790gu
```

b.

```sh
1. [-+]?([0-9]*[.])?[0-9]*([eE][-+]?\d+)?([fFlL])?
2. 1e+71 2.3e7f .33e-1L 0.123 -.123 -0.001 +1 -2 -0 1.e7
3. regexr.com/790it
```

7.

   

 ![Alt text](https://g.gravizo.com/svg?digraph%20G%20{size%20="4,4";main%20[label="S"];underline%20[label="_"];digit%20[label="0-9"];symbol%20[label="a-Z"];main%20->%20underline;main%20->%20symbol;underline%20->%20symbol;underline%20->%20digit;underline%20->%20underline;symbol%20->%20underline;symbol%20->%20digit;symbol%20->%20symbol;digit%20->%20symbol;digit%20->%20underline;digit%20->%20digit;}
 )

     ![Alt text](https://g.gravizo.com/svg?
    	 digraph G {
            size ="4,4";
            main [label="S"];
            sign [label="+-"];
            digit [label="0-9"];
            dotdigit [label="0-9"];
            expdigit [label="0-9"];
            exp [label="eE"];
            expsign [label="+-"];
            fl [label="fFlL"];
            dot [label="."];
            main -> {sign, digit, dot};
            sign -> {digit, dot};
            digit -> {digit, dot};
            dot -> {dotdigit, exp, fl};
            dotdigit -> {dotdigit, exp, fl};
            exp -> {expdigit, expsign};
            expsign -> expdigit;
            expdigit -> {expdigit, fl};
          }
        )

 ![Alt text](https://g.gravizo.com/svg?digraph%20G%20{size%20="4,4";main%20[label="S"];sign%20[label="+-"];digit%20[label="0-9"];dotdigit%20[label="0-9"];expdigit%20[label="0-9"];exp%20[label="eE"];expsign%20[label="+-"];fl%20[label="fFlL"];dot%20[label="."];main%20->%20{sign,%20digit,%20dot};sign%20->%20{digit,%20dot};digit%20->%20{digit,%20dot};dot%20->%20{dotdigit,%20exp,%20fl};dotdigit%20->%20{dotdigit,%20exp,%20fl};exp%20->%20{expdigit,%20expsign};expsign%20->%20expdigit;expdigit%20->%20{expdigit,%20fl};}
    )
