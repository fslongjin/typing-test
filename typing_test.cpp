//@author-fslongjin
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
#include "sqlite3.h"
#pragma comment(lib,"sqlite3.lib")
#include<Windows.h>
#include<time.h>
#include<sstream>
#include<random>
#include<cstdlib>
using namespace std;

sqlite3* db;



static int callback(void* NotUsed, int argc, char** argv, char** azColName);


void init_db();//初始化数据库的函数
void add_text();//添加数据的函数
void tip_cmd_err();//提示命令错误的函数
void tip_db_error(char* msg);//提示数据库错误的函数
void tip_db_empty();//提示练习数据为空的函数
void tip_success();//提示操作成功的函数
void data_manage();//管理数据的函数
void select_text();//查看全部练习文本的函数
void testing(string txt);//正式执行测试的函数
void history();//查看历史记录的函数
void about();

void speed_test();//打字测试的函数


void BackGround(unsigned int ForeColor = 7, unsigned int BackGroundColor = 0);//设置字体颜色的函数

int* tmp_int;
string tmp_str;



int main()
{
	//连接数据库
	int rc;
	char* zErrMsg = 0;
	rc = sqlite3_open("typing_test.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	init_db();
	
	while (true)
	{
		system("cls");
		cout << "**********************************************\n"
			<< "*                                            *\n"
			<< "*             打字速度测试系统               *\n"
			<< "*                  V1.0.0                    *\n"
			<< "*                                            *\n"
			<< "*            开发者：灵高信息技术            *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*                   菜 单                    *\n"
			<< "*                                            *\n"
			<< "*               1、打字测试                  *\n"
			<< "*               2、历史记录                  *\n"
			<< "*               3、练习数据管理              *\n"
			<< "*               4、关于项目                  *\n"
			<< "*               5、退出程序                  *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";
		
		cout << endl;
		cout << "请输入你要进行的操作的编号: ";
		string choose;
		cin >> choose;
		if (choose == "1")
		{
			speed_test();
		}
		else if (choose == "2")
		{
			history();
		}
		else if (choose == "3")
		{
			data_manage();
		}
		else if (choose == "4")
		{
			about();
		}
		else if (choose == "5")
		{
			cout << endl << "感谢你的使用，再见！" << endl;
			sqlite3_close(db);
			Sleep(500);
			exit(0);
		}
		else
		{
			tip_cmd_err();//提示命令错误
		}



	}

}

void init_db()//初始化数据库
{

	string sql_1;
	sql_1 = "CREATE TABLE WORDS("  \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT," \
		"WD TEXT NOT NULL);" ;

	const char* cmd_1 = { sql_1.c_str() };
	int rc1;
	char* zErrMsg1 = 0;
	
	rc1 = sqlite3_exec(db, cmd_1, callback, 0, &zErrMsg1);


	string sql_2;
	sql_2 = "CREATE TABLE Record("  \
		"ID INTEGER PRIMARY KEY AUTOINCREMENT," \
		"AVERAGE_SPEED REAL NOT NULL,"\
		"MAX_SPEED REAL NOT NULL,"\
		"MIN_SPEED REAL NOT NULL);";

	const char* cmd_2 = { sql_2.c_str() };
	int rc2;
	char* zErrMsg2 = 0;
	rc2 = sqlite3_exec(db, cmd_2, callback, 0, &zErrMsg2);

	
	if (rc1 != SQLITE_OK||rc2!=SQLITE_OK) {
		sqlite3_free(zErrMsg1);
		sqlite3_free(zErrMsg2);
		
	}
	else {
		fprintf(stdout, "成功初始化数据库！\n");
	}

}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	
	cout << "==========================================================" << endl;
	
	for (int i = 0; i < argc; i++) {
		string name;
		if (i == 0) name = "文本id：";
		else name = "文本内容：\n";

		cout << endl;
		printf("%s %s\n", name.c_str(), argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int callback_history(void* NotUsed, int argc, char** argv, char** azColName) {

	cout << "==========================================================" << endl;

	for (int i = 0; i < argc; i++) {
		string name;
		string ttmp;
		switch (i)
		{
		case 0:
			ttmp = argv[i];
			name = "测试id：" + ttmp;
			break;
		case 1:
			ttmp = argv[i];
			name = "平均正确速度：\t " + ttmp +"\t字/分钟";
			break;
		case 2:
			ttmp = argv[i];
			name = "最大瞬时速度：\t " + ttmp + "\t字/分钟";
			break;
		case 3:
			ttmp = argv[i];
			name = "最慢瞬时速度：\t " + ttmp + "\t字/分钟";
			break;
		}
		cout << name;
		cout << endl;
		
	}
	printf("\n");
	return 0;
}

static int callback_txt(void* NotUsed, int argc, char** argv, char** azColName) {
	int js = 0;
	//*tmp_str = 
	tmp_str = argv[0];
	
	return 0;
}

static int callback_count(void* para, int argc, char** argv, char** data) 
{
	*tmp_int = atoi(data[1]);
	return 0;
}

static int callback_first(void* para, int argc, char** argv, char**data)
{
	*tmp_int = atoi(argv[0]);
	return  0;
}

static int callback_speed_test(void* NotUsed, int argc, char** argv, char** data)
{

	return atoi(data[1]);
}



void testing(string txt)
{
	cout << "\n\n\n\n\n\n\n\n";
	cout << "请输入下列文本:" << endl;
	cout << endl << txt;
	cout << endl << endl;

	
	char ch;
	vector<char> ipt;

	double min_delta = 9999999999;
	double max_delta = 0;
	double totle_time = 0;
	int js_bk = 0;

	time_t start_time = clock();
	time_t end_time;
	time_t last_time = clock();

	for (;;)
	{
		
		
		if (ipt.size()==txt.length())//输入完成整段字符
		{
			_putch('\n');
			end_time = clock();
			break;
		}
		ch = _getch();
		if (ch == 8)//退格
		{
			last_time = clock();
			if (!ipt.empty())
			{
				js_bk++;
				_putch('\b');
				_putch(' ');
				_putch('\b');
				ipt.pop_back();
			}

		}
		else if (ch == -32 || ch == 75 || ch == 77 || ch == 72||ch==80)//让方向键不起作用
		{
			
		}
		else
		{
			
			time_t tmp = clock();
			max_delta = max(max_delta, (double(tmp - last_time) / 1000.0));
			min_delta = min(min_delta, (double(tmp - last_time) / 1000.0));
			last_time = tmp;
			putchar(ch);
			ipt.push_back(ch);
		}
	}

	int correct = 0;
	for (int i = 0; i < txt.length(); i++)
	{
		if (txt[i] == ipt[i])
			correct++;
	}

	double total_time = double(end_time-start_time) / 1000.0;
	system("cls");
	cout << "**********************************************\n"
		 << "*                                            *\n"
		 << "*                 测试结束！                 *\n"
		 << "*                                            *\n"
		 << "**********************************************\n";
	cout << endl << endl;
	cout << "\t\t字符数：\t" << txt.length() << endl << endl;
	cout << "\t\t总用时：\t" << total_time <<"\t秒"<< endl<<endl;
	cout << "\t\t退格次数：\t" << js_bk << "\t次" << endl << endl;
	cout << "\t\t正确率：\t" << 100.0*double(correct)/double(txt.length())<< "%" << endl << endl;
	cout << "\t\t平均速度：\t" <<  60.0*double(txt.length())/double(total_time) << "\t字符/分钟" << endl << endl;
	cout << "\t\t平均正确速度：\t" << 60.0*double(correct) / double(total_time) << "\t字符/分钟" << endl << endl;
	cout << "\t\t最大瞬时速度：\t" << double(60.0 * 2.0) / double(min_delta) << "\t字符/分钟" << endl << endl;
	cout << "\t\t最小瞬时速度：\t" << double(60.0 * 2.0) / double(max_delta) << "\t字符/分钟" << endl << endl;

	
	 

	string sql;
	sql = "INSERT INTO Record (AVERAGE_SPEED,MAX_SPEED, MIN_SPEED ) "  \
		"VALUES (" + to_string(60.0 * double(correct) / double(total_time)) + "," + to_string(double(60 * 2) / double(min_delta)) + "," + to_string(double(60 * 2) / double(max_delta)) + "); ";


	
	char* zErrMsg = 0;
	int rc = rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);

		sqlite3_free(zErrMsg);
	}

}



//打字测试
void start_test()
{
	default_random_engine engine(static_cast<unsigned int>(time(0)));
	
	int rc;
	char* zErrMsg = 0;
	const char* data = "ttt";

	
	
	string sql = "SELECT * from WORDS limit 1";
	tmp_int = (int*)malloc(sizeof(int));
	rc = sqlite3_exec(db, sql.c_str(), callback_first, (void*)data, &zErrMsg);

	int start = *tmp_int;
	
	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);
		sqlite3_free(zErrMsg);

	}
	
	sql = "SELECT count(*) from WORDS";

	rc = sqlite3_exec(db, sql.c_str(), callback_count, (void*)data, &zErrMsg);

	int end = *tmp_int;

	
	if (end == 0)//当数据库中没有数据的时候
	{
		tip_db_empty();
		return;
	}
	free(tmp_int);
	

	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);
		sqlite3_free(zErrMsg);

	}
	sqlite3_free(zErrMsg);
	uniform_int_distribution<unsigned int> randomInt(start,start+end);

	bool is_ok = false;
	while (!is_ok)
	{
		int id = randomInt(engine);
		stringstream fmt;
		fmt << "SELECT WD from WORDS WHERE ID = " << to_string(id);
		string s = fmt.str();
		
		
		
		char* ErrMsg = 0;
		int r;
		r = sqlite3_exec(db, s.c_str(), callback_txt, (void*)data, &ErrMsg);
		if (r != SQLITE_OK) {
			tip_db_error(ErrMsg);//输出数据库错误信息
			sqlite3_free(ErrMsg);
		}
		else is_ok = true;
		if (tmp_str == "") is_ok = false;
		
	}
	
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*         打字测试将在三秒钟后开始...        *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";

	Sleep(1000);
	for (int i = 3; i >=1; i--)
	{
		stringstream f;
		f << "*                     " << to_string(i) << "                      *\n";
		system("cls");
		cout << "\n\n\n\n\n\n\n\n";
		cout << "**********************************************\n"
			<< "*                                            *\n"
			<< f.str()
			<< "*                                            *\n"
			<< "**********************************************\n";
		Sleep(1000);
	}

	system("cls");
	testing(tmp_str);
	Sleep(5000);
	system("pause");


}

void speed_test()
{
	system("cls");
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*               打字测试模式                 *\n"
		<< "*                                            *\n"
		<< "* ========================================== *\n"
		<< "*                                            *\n"
		<< "*                 菜 单                      *\n"
		<< "*                                            *\n"
		<< "*            1、开始测试                     *\n"
		<< "*            2、返回上级菜单                 *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";

	cout << endl;
	cout << "请输入你要进行的操作的编号: ";
	string choose;
	cin >> choose;

	if (choose == "1")
	{
		start_test();
	}
	
	else if (choose == "2")
	{
		return;
	}
	else
	{
		tip_cmd_err();//提示命令错误
		return;
	}
}



void history()//查看历史记录
{

	system("cls");
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*                  历史记录                  *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	int rc;
	char* zErrMsg = 0;
	const char* data = "Callback function called";

	string sql = "SELECT * from Record";


	rc = sqlite3_exec(db, sql.c_str(), callback_history, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);
		sqlite3_free(zErrMsg);
	}
	Sleep(500);
	//cout << "按任意键返回上一级...\n";
	system("pause");
}

void data_manage()
{
	while (true)
	{
		system("cls");
		cout << "**********************************************\n"
			<< "*                                            *\n"
			<< "*                数据管理                    *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*                 菜 单                      *\n"
			<< "*                                            *\n"
			<< "*            1、添加练习数据                 *\n"
			<< "*            2、查看所有练习数据             *\n"
			<< "*            3、返回上级菜单                 *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";

		cout << endl;
		cout << "请输入你要进行的操作的编号: ";
		string choose;
		cin >> choose;
		if (choose == "1")
		{
			add_text();
		}
		else if (choose == "2")
		{
			select_text();
		}
		else if (choose == "3")
		{
			return;
		}
		else
		{
			tip_cmd_err();//提示命令错误
		}
	}
	
}




void select_text()
{
	system("cls");
	cout << "\n\n";
	cout << "\t当前处于查看练习文本模式！" << endl;
	cout << "\n\n";

	int rc;
	char* zErrMsg = 0;
	const char* data = "Callback function called";
	

	string sql = "SELECT count(*) from WORDS";
	tmp_int = (int*)malloc(sizeof(int));

	rc = sqlite3_exec(db, sql.c_str(), callback_count, (void*)data, &zErrMsg);
	
	cout << "当前数据库中有" << *tmp_int << "条练习数据！" << endl;
	free(tmp_int);
	sql = "SELECT * from WORDS";


	rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);
		sqlite3_free(zErrMsg);
	}
	

	cout << "==========================================================" << endl;
	while (true)
	{
		cout << "**********************************************\n"
			<< "*                                            *\n"
			<< "*                 菜 单                      *\n"
			<< "*                                            *\n"
			<< "*            1、删除练习数据                 *\n"
			<< "*            2、返回上级菜单                 *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";

		cout << endl;
		cout << "请输入你要进行的操作的编号: ";
		string choose;
		cin >> choose;

		if (choose == "1")
		{
			cout << endl;
			cout << "请输入你要删除的数据的id:";
			string ipt;
			cin >> ipt;
			int id;
			try {
				id = stoi(ipt);
			}
			catch(exception e)
			{
				tip_cmd_err();
				return;
			}
			stringstream fmt;
			fmt << "DELETE from WORDS WHERE ID = " << ipt;
			string s = fmt.str();
			
			rc = sqlite3_exec(db, s.c_str(), callback, (void*)data, &zErrMsg);

			if (rc != SQLITE_OK) {
				tip_db_error(zErrMsg);//输出数据库错误信息
				sqlite3_free(zErrMsg);
			}
			else {
				tip_success();
				return;
			}
		}
		
		else
		{
			return;
		}
	}
}


void add_text()
{
	system("cls");
	cout << "\n\n";
	cout << "\t当前是导入练习数据的模式！" << endl;
	cout << endl << "\n\t请在下方粘贴文本！\n\t(输入esc即可返回上一级！)" << endl << endl;;
	cout << "\t==========文本粘贴区==========" << endl<<endl;
	string txt;
	getchar();
	getline(cin,txt);
	
	if (txt == "esc") return;
	if (txt == "") return;
	//将文本插入数据库
	 /* Create SQL statement */
	string sql;
	sql = "INSERT INTO WORDS (WD) "  \
		"VALUES ('" + txt + "'); " ;
	

	
	char* zErrMsg = 0;
	int rc = rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		tip_db_error(zErrMsg);
		
		sqlite3_free(zErrMsg);
	}
	else {
		
		system("cls");
		
		cout << "\n\n\n\n\n\n\n\n";
		cout << "**********************************************\n"
			<< "*                                            *\n";
		cout << "*";
		BackGround(2, 0);
		cout << "            成功添加一条练习数据！          ";

		BackGround(7, 0);
		cout << "*\n";
		cout<< "*                                            *\n"
			<< "**********************************************\n";
		Sleep(1000);
		sqlite3_free(zErrMsg);
	}

}



void about()
{
	
		system("cls");
		cout<< "**********************************************\n"
			<< "*                                            *\n"
			<< "*                关于项目                    *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*      项目首页：typing-test.ringotek.cn     *\n"
			<< "*                                            *\n"
			<< "*          项目作者：灵高信息技术            *\n"
			<< "*                                            *\n"
			<< "*        本项目采用apache-2.0协议开源        *\n"
			<< "*                                            *\n"
			<< "*      联系我们：ringotek@ringotek.cn        *\n"
			<< "*                                            *\n"
			<< "*    如果您修复了本项目的bug，或者增加了     *\n"
			<< "*  新功能，请务必在github上提交PullRequest   *\n"
			<< "*          我们将第一时间采纳！              *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";

		cout << endl;
		Sleep(1000);
		system("Pause");
	

}


void tip_cmd_err()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*             请输入正确的命令！             *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	Sleep(1000);
}

void tip_db_error(char* msg)
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*                数据库错误！                *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	cout <<"错误信息： "<<msg<< endl;
	Sleep(1000);
}

void tip_db_empty()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*           你还没有添加练习数据！           *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	Sleep(1000);
}


void tip_success()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*                 操作成功！                 *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	
	Sleep(1000);
}

//背景颜色： 
//0 黑色  
//1 蓝色 
//2 绿色 
//3 湖蓝色 
//4 红色 
//5 紫色 
//6 黄色
//7 白色
//8 灰色
//9 淡蓝色
void BackGround(unsigned int ForeColor, unsigned int BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);
}