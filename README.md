# Agumon
	类C语言编译器

#进度
	assignment statement:
		int i = 0;
		double d = 0.0;
		int i = 1 + 2 * 3;
		int i = (1 + 2) * 3;
		int i = +1;
		int i = -1;
		bool b = true / false;

明天加一组测试：生成树的class type 测试。

fork
fork




##以下请无视

	

类型：
	int
	double
	char
	bool

表达式：
	双目：lhs op(+ - * /) rhs
	单目：+a = a, -a = -a
	赋值：a = b, a = b + c
	相等：a == b, a != b
	比较：a( > < >= <= )b
	条件：a ? b : c
	逻辑：a || b, a && b
	结尾：;

语句：
	if：if( exp ) { stat }
		if( exp ) { stat_1 } else { stat_2 } 

	


todoList:
	scanner 错误检查
	int, double 换成 intSign doubleSign或者其他容易辨识的名字