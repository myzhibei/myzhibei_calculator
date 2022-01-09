#include <iomanip>
#include <iostream>
#include <cctype>
#include <cstring>
#include <cmath>

using namespace std;
const int  MAX(100); //定义最大的表达式长度

void eatspaces(char*);//声明删除空格的函数
double calc(char* str);//计算表达式
double getnumber(char* str, int& index);//用于获得数字
double getterm(char* str, int& index);//获取项值 类似于1*96  6/6的形式
char* getkuohao(char* str, int& index);//用于解析字符串的括号 该函数将会删除首尾括号并计算表达式的值
double calcoperator(char* dooperator, double value);//用于计算各种运算符的函数
const double degToRad = 180.0 / 3.14159265358979323846; // 定义角度值转为弧度制的单位值

int main() {
	char expression[MAX] = { 0 };
	//输出信息
	cout <<
		"算术表达式求值\n"
		"操作数可以是负的非整数(支持负数运算,支持小数点\n)"
		"加(+)减(-)乘(*)除(/) 指数(^)\n"
		"支持三角函数sin 、cos 、tan（弧度制）sind、cosd、tand（角度值） 以及平方根sqrt 例如sind(30) sqrt(2+2)\n"
		"pi 表示 π"
		<< endl<<endl;
	for (;;) {
		cin.getline(expression, sizeof expression);   // 读取输入
		eatspaces(expression);                    // 删除空格
		if (!expression[0])                        // 判断输入是否为空
			return 0;
		cout <<endl<< "= " << calc(expression)  // 输出计算值
			<< endl << endl;
	}
	system("pause");//输出按任意键
	return 0;
}
void eatspaces(char* exp) {
	int i(0);//设置第一个元素的索引
	int j(0);
	while (*(exp + i) != '\0') { //判断是否到达最后一位
		*(exp + i) = *(exp + j++); //复制字符
		if (*(exp + i) != ' ') {
			i++;//使i 递增
		}
	}
	return;
}
double calc(char* str) {
	double value(0.0);
	int index = 0;
	value = getterm(str, index);//获得当前的项
	for (;;) { //无限循环
		switch (*(str + index++)) {
		case '+':
			value += getterm(str, index);//
			break;
		case '-':
			value -= getterm(str, index);
			break;
		case '\0':
			return value;//返回值
		default:
			exit(1);//出错了
			break;
		}
	}
}
double getnumber(char* str, int& index) {
	double value(0.0);
	char opname[6];//定义操作符的大小 数学的函数一般最多为5个字符
	int ip = 0; //定义一个计数器
	while (isalpha(*(str + index))) {
		opname[ip++] = *(str + index++);
	}
	opname[ip] = '\0';//添加结尾

	if (opname[0] != '\0') {
		if (!_stricmp(opname, "pi")) //pi获取π值
			return 3.14159265358979323846;
		char* numberexp = getkuohao(str, ++index);//获取括号内容
		value = calc(numberexp);//计算括号内容
		value = calcoperator(opname, value);//计算函数值
		delete[] numberexp;//释放指针
		return value;//返回值
	}
	if (*(str + index) == '(') { //计算括号内容
		char* psubstr(nullptr);            // 设置指针
		psubstr = getkuohao(str, ++index);   // 获取括号内的内容
		value = calc(psubstr);             // 获取括号内的表达式
		delete[]psubstr;                   // 释放指针
		return value;                      // 返回值
	}


	if (isdigit(*(str + index)) == false) {
		exit(1);
	}
	//判断是否为数字
	while (isdigit(*(str + index))) {
		value = value * 10 + (*(str + index++) - '0');//从左往右 计算数字
	}
	if (*(str + index) != '.') {
		return value;    //检查数字后是否包含小数点
	}
	double fact = 1.0;//定义位数
	while (isdigit(*(str + (++index)))) {
		fact = fact * 0.1;
		value = value + (*(str + index) - '0') * fact; //自乘
	}
	return value;
}
double getterm(char* str, int& index) {
	double value(0.0);
	if (*(str + index) == '-') {//负数
		index++;
		value = 0.0 - getnumber(str, index);//获取数组 并递增到下个运算符的位置
	}
	else {
		value = getnumber(str, index);//获取数组 并递增到下个运算符的位置
	}

	while (true) {
		if (*(str + index) == '*') {
			value *= getnumber(str, ++index);//是否为乘法运算
		}
		else {
			if (*(str + index) == '/') {
				value /= getnumber(str, ++index);//是否为除法运算符
			}
			else {
				if (*(str + index) == '^') {
					value = pow(value, getnumber(str, ++index)); //是否为幂运算符
				}
				else {
					break;
				}
			}
		}
	}
	return value;
}
char* getkuohao(char* str, int& index) {
	char buffer[MAX];//声明一个局部数组
	char* pstr(nullptr);//设置一个 指针
	int numcount(0);//设置计数器 原理是 当发现一个左括号时+1 而当发现一个右括号时减去1 当再次检测到有括号时 如果值为0表示是最后一个括号 那么删除它 如果是1则表示他只是一个子括号
	int bufindex(index);//定义str的起始位置
	do {
		buffer[index - bufindex] = *(str + index);//局部数组第一个字符为第一个左括号的笑一个字符(调用前index自增)
		switch (buffer[index - bufindex]) { //判断该字符
		case ')':
			if (0 == numcount) { //到达最后一个右括号
				buffer[index - bufindex] = '\0';//把这个字符用结尾\0替换
				++index;//递增index
				pstr = new char[index - bufindex];//在堆中创建一个字符串
				if (!pstr) {
					cout << "内存分配故障" << endl;
					exit(1);
				}
				strcpy_s(pstr, index - bufindex, buffer);//复制字符串
				return pstr;//将创建的字符串以其地址返回
			}
			else {
				numcount--;//子右括号
				break;
			}
		case '(':
			numcount++;//左括号
			break;
		}
	} while (*(str + index++) != '\0'); //直到最后一个字符
}
double calcoperator(char* dooperator, double value) {
	if (!_stricmp(dooperator, "sin")) //RAD SIN
		return sin(value);
	else if (!_stricmp(dooperator, "sind")) //DEG SIN
		return sin(value / degToRad);
	else if (!_stricmp(dooperator, "cos")) //RAD COS
		return cos(value);
	else if (!_stricmp(dooperator, "cosd")) //DEG COS
		return cos(value / degToRad);
	else if (!_stricmp(dooperator, "tan")) //RAD TAN
		return tan(value);
	else if (!_stricmp(dooperator, "tand")) //DEG TAN
		return tan(value / degToRad);
	else if (!_stricmp(dooperator, "sqrt")) //平方根
		return sqrt(value);
	else {
		//未知的函数
	}
	return 0.0;
}
