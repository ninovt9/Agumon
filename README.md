# Agumon
	类C语言编译器



	expression:
			符号		名称				parser进度
		0:
			integer		整型				OK
			
		1:
			[]			数组下标			OK
			()			圆括号				
			.			对象
			->			指针
		2:
			*			取值（指针)	
			&			取地址
			!			逻辑非
		3:
			/			除
			*			乘
			%			余数
		4:
			+			加
			-			减
		5:
			>			大于
			>=			大于等于
			<			小于
			<=			小于等于
		6:
			==			等于
			!=			不等于
		7:
			&&			逻辑与
		8:
			||			逻辑或
		9:
			?:			条件运算符
		10:
			=			赋值
			/=			运算后赋值
			*=
			%=
			+=
			-=
		11:
			,

	