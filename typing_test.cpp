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


void init_db();//��ʼ�����ݿ�ĺ���
void add_text();//������ݵĺ���
void tip_cmd_err();//��ʾ�������ĺ���
void tip_db_error(char* msg);//��ʾ���ݿ����ĺ���
void tip_db_empty();//��ʾ��ϰ����Ϊ�յĺ���
void tip_success();//��ʾ�����ɹ��ĺ���
void data_manage();//�������ݵĺ���
void select_text();//�鿴ȫ����ϰ�ı��ĺ���
void testing(string txt);//��ʽִ�в��Եĺ���
void history();//�鿴��ʷ��¼�ĺ���
void about();

void speed_test();//���ֲ��Եĺ���


void BackGround(unsigned int ForeColor = 7, unsigned int BackGroundColor = 0);//����������ɫ�ĺ���

int* tmp_int;
string tmp_str;



int main()
{
	//�������ݿ�
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
			<< "*             �����ٶȲ���ϵͳ               *\n"
			<< "*                  V1.0.0                    *\n"
			<< "*                                            *\n"
			<< "*            �����ߣ������Ϣ����            *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*                   �� ��                    *\n"
			<< "*                                            *\n"
			<< "*               1�����ֲ���                  *\n"
			<< "*               2����ʷ��¼                  *\n"
			<< "*               3����ϰ���ݹ���              *\n"
			<< "*               4��������Ŀ                  *\n"
			<< "*               5���˳�����                  *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";
		
		cout << endl;
		cout << "��������Ҫ���еĲ����ı��: ";
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
			cout << endl << "��л���ʹ�ã��ټ���" << endl;
			sqlite3_close(db);
			Sleep(500);
			exit(0);
		}
		else
		{
			tip_cmd_err();//��ʾ�������
		}



	}

}

void init_db()//��ʼ�����ݿ�
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
		fprintf(stdout, "�ɹ���ʼ�����ݿ⣡\n");
	}

}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	
	cout << "==========================================================" << endl;
	
	for (int i = 0; i < argc; i++) {
		string name;
		if (i == 0) name = "�ı�id��";
		else name = "�ı����ݣ�\n";

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
			name = "����id��" + ttmp;
			break;
		case 1:
			ttmp = argv[i];
			name = "ƽ����ȷ�ٶȣ�\t " + ttmp +"\t��/����";
			break;
		case 2:
			ttmp = argv[i];
			name = "���˲ʱ�ٶȣ�\t " + ttmp + "\t��/����";
			break;
		case 3:
			ttmp = argv[i];
			name = "����˲ʱ�ٶȣ�\t " + ttmp + "\t��/����";
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
	cout << "�����������ı�:" << endl;
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
		
		
		if (ipt.size()==txt.length())//������������ַ�
		{
			_putch('\n');
			end_time = clock();
			break;
		}
		ch = _getch();
		if (ch == 8)//�˸�
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
		else if (ch == -32 || ch == 75 || ch == 77 || ch == 72||ch==80)//�÷������������
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
		 << "*                 ���Խ�����                 *\n"
		 << "*                                            *\n"
		 << "**********************************************\n";
	cout << endl << endl;
	cout << "\t\t�ַ�����\t" << txt.length() << endl << endl;
	cout << "\t\t����ʱ��\t" << total_time <<"\t��"<< endl<<endl;
	cout << "\t\t�˸������\t" << js_bk << "\t��" << endl << endl;
	cout << "\t\t��ȷ�ʣ�\t" << 100.0*double(correct)/double(txt.length())<< "%" << endl << endl;
	cout << "\t\tƽ���ٶȣ�\t" <<  60.0*double(txt.length())/double(total_time) << "\t�ַ�/����" << endl << endl;
	cout << "\t\tƽ����ȷ�ٶȣ�\t" << 60.0*double(correct) / double(total_time) << "\t�ַ�/����" << endl << endl;
	cout << "\t\t���˲ʱ�ٶȣ�\t" << double(60.0 * 2.0) / double(min_delta) << "\t�ַ�/����" << endl << endl;
	cout << "\t\t��С˲ʱ�ٶȣ�\t" << double(60.0 * 2.0) / double(max_delta) << "\t�ַ�/����" << endl << endl;

	
	 

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



//���ֲ���
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

	
	if (end == 0)//�����ݿ���û�����ݵ�ʱ��
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
			tip_db_error(ErrMsg);//������ݿ������Ϣ
			sqlite3_free(ErrMsg);
		}
		else is_ok = true;
		if (tmp_str == "") is_ok = false;
		
	}
	
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*         ���ֲ��Խ��������Ӻ�ʼ...        *\n"
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
		<< "*               ���ֲ���ģʽ                 *\n"
		<< "*                                            *\n"
		<< "* ========================================== *\n"
		<< "*                                            *\n"
		<< "*                 �� ��                      *\n"
		<< "*                                            *\n"
		<< "*            1����ʼ����                     *\n"
		<< "*            2�������ϼ��˵�                 *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";

	cout << endl;
	cout << "��������Ҫ���еĲ����ı��: ";
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
		tip_cmd_err();//��ʾ�������
		return;
	}
}



void history()//�鿴��ʷ��¼
{

	system("cls");
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*                  ��ʷ��¼                  *\n"
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
	//cout << "�������������һ��...\n";
	system("pause");
}

void data_manage()
{
	while (true)
	{
		system("cls");
		cout << "**********************************************\n"
			<< "*                                            *\n"
			<< "*                ���ݹ���                    *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*                 �� ��                      *\n"
			<< "*                                            *\n"
			<< "*            1�������ϰ����                 *\n"
			<< "*            2���鿴������ϰ����             *\n"
			<< "*            3�������ϼ��˵�                 *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";

		cout << endl;
		cout << "��������Ҫ���еĲ����ı��: ";
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
			tip_cmd_err();//��ʾ�������
		}
	}
	
}




void select_text()
{
	system("cls");
	cout << "\n\n";
	cout << "\t��ǰ���ڲ鿴��ϰ�ı�ģʽ��" << endl;
	cout << "\n\n";

	int rc;
	char* zErrMsg = 0;
	const char* data = "Callback function called";
	

	string sql = "SELECT count(*) from WORDS";
	tmp_int = (int*)malloc(sizeof(int));

	rc = sqlite3_exec(db, sql.c_str(), callback_count, (void*)data, &zErrMsg);
	
	cout << "��ǰ���ݿ�����" << *tmp_int << "����ϰ���ݣ�" << endl;
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
			<< "*                 �� ��                      *\n"
			<< "*                                            *\n"
			<< "*            1��ɾ����ϰ����                 *\n"
			<< "*            2�������ϼ��˵�                 *\n"
			<< "*                                            *\n"
			<< "**********************************************\n";

		cout << endl;
		cout << "��������Ҫ���еĲ����ı��: ";
		string choose;
		cin >> choose;

		if (choose == "1")
		{
			cout << endl;
			cout << "��������Ҫɾ�������ݵ�id:";
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
				tip_db_error(zErrMsg);//������ݿ������Ϣ
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
	cout << "\t��ǰ�ǵ�����ϰ���ݵ�ģʽ��" << endl;
	cout << endl << "\n\t�����·�ճ���ı���\n\t(����esc���ɷ�����һ����)" << endl << endl;;
	cout << "\t==========�ı�ճ����==========" << endl<<endl;
	string txt;
	getchar();
	getline(cin,txt);
	
	if (txt == "esc") return;
	if (txt == "") return;
	//���ı��������ݿ�
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
		cout << "            �ɹ����һ����ϰ���ݣ�          ";

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
			<< "*                ������Ŀ                    *\n"
			<< "*                                            *\n"
			<< "* ========================================== *\n"
			<< "*                                            *\n"
			<< "*      ��Ŀ��ҳ��typing-test.ringotek.cn     *\n"
			<< "*                                            *\n"
			<< "*          ��Ŀ���ߣ������Ϣ����            *\n"
			<< "*                                            *\n"
			<< "*        ����Ŀ����apache-2.0Э�鿪Դ        *\n"
			<< "*                                            *\n"
			<< "*      ��ϵ���ǣ�ringotek@ringotek.cn        *\n"
			<< "*                                            *\n"
			<< "*    ������޸��˱���Ŀ��bug������������     *\n"
			<< "*  �¹��ܣ��������github���ύPullRequest   *\n"
			<< "*          ���ǽ���һʱ����ɣ�              *\n"
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
		<< "*             ��������ȷ�����             *\n"
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
		<< "*                ���ݿ����                *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	cout <<"������Ϣ�� "<<msg<< endl;
	Sleep(1000);
}

void tip_db_empty()
{
	system("cls");
	cout << "\n\n\n\n\n\n\n\n";
	cout << "**********************************************\n"
		<< "*                                            *\n"
		<< "*           �㻹û�������ϰ���ݣ�           *\n"
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
		<< "*                 �����ɹ���                 *\n"
		<< "*                                            *\n"
		<< "**********************************************\n";
	
	Sleep(1000);
}

//������ɫ�� 
//0 ��ɫ  
//1 ��ɫ 
//2 ��ɫ 
//3 ����ɫ 
//4 ��ɫ 
//5 ��ɫ 
//6 ��ɫ
//7 ��ɫ
//8 ��ɫ
//9 ����ɫ
void BackGround(unsigned int ForeColor, unsigned int BackGroundColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);
}