; assignment
.class public assignment
.super java/lang//Object
.field public static _sc Ljava/util/Scanner;
.field public static intvalue I
1:int intvalue;
.field public static floatvalue F
2:float floatvalue;
3:
4:int main() {
5:	int integer_a, integer_b, integer_c;
6:	bool bool_d;
7:	float float_e, float_f;
8:	
9:	// read 3 integer and 2 float
new java/util/Scanner
dup
getstatic java/lang/System/in Ljava/io/InputStream;
invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V
putstatic assignment/_sc Ljava/util/Scanner;

getstatic assignment/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 1
10:	read integer_a;
getstatic assignment/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 2
11:	read integer_b;
getstatic assignment/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextInt()I
istore 3
12:	read integer_c;
getstatic assignment/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 5
13:	read float_e;
getstatic assignment/_sc Ljava/util/Scanner;
invokevirtual java/util/Scanner/nextFloat()F
fstore 6
14:	read float_f;
15:	
16:	print "--------------------\n;
17:	
18:	print integer_c;
19:	print " ;
20:	print integer_a;
21:	print " ;
22:	print integer_b;
23:	print " ;
24:	print float_e;
25:	print " ;
26:	print float_f;
27:	print "\n;
28:	
29:	// swap integer_b and integer_c, take intvalue as temp storage
30:	intvalue = integer_b;
31:	integer_b = integer_c;
32:	integer_c = intvalue;
33:	print integer_b;
34:	print " ;
35:	print integer_c;
36:	print "\n;
37:	
38:	// swap float_e and float_f, take gr as temp storage
39:	floatvalue = float_f;
40:	float_f = float_e;
41:	float_e = floatvalue;
42:	print float_e;
43:	print " ;
44:	print float_f;
45:	print "\n;
46:	
47:	bool_d = true;
48:	print bool_d;
49:	print "\n;
50:
51:	return 0;
52:}
53:
54:
55:/*
56:Input: 
57:6 28 496 1.4142 3.1415
58:
59:Output: 
60:--------------------
61:496 6 28 1.4142 3.1415
62:496 28
63:3.1415 1.4142
64:true
65:*/
