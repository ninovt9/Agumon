# Agumon

#进度
	assignment statement:
		int i = 0;





##以下请无视

	类C语言编译器

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
	peekChar 优化
	dict.find(char / string)
	parser tree assignment statement 生成时的类型检查
	更改Node(Token(TokenType::VARIABLE, "i"), nullptr, nullptr), nullptr 改成 std::shared默认构造