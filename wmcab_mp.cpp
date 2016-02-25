#include <iostream>
#include <string>
using namespace std;

const int cycle_len = 12;	//����������������� ����� 12���
const int srt_age = 12; //�������, �� �������� ����� ����������� ������
const int p_quant = 7;	//����� ������� �����
const int y_quant = 48;	//���������� ��� � ��������� (���������� ������� 12-59���)
const int m_quant = 12;	//���������� ������� // month precision


void find_oppoints(int dig, int oppoints[p_quant]);	//���������� ������� ����� ������ GLE �� ����������� ����� WARNING! function incomplete!
string month_translate(int month);

int main(int argc, char * argv[]){
	int hy = 1987; int hm = 11; int hd = 14;
	int wy = 1984; int wm = 11; int wd = 16;

	cout << "Input male bithdate (dd mm yyyy)" << endl;
	cin >> hd >> hm >> hy;
	cout << "Input female bithdate (dd mm yyyy)" << endl;
	cin >> wd >> wm >> wy;

	int maj_b;	//��� �������� �������� �� ����
	int min_b;	//��� �������� �������� �� ����(������ ������� � ����� GLE)

	int digitMin;	//���������� ����� �������� �� ����
	int digitMaj;	//���������� ����� �������� �� ����

	int cycle_p[p_quant];					//������ ������� �����, ��� ������ ������ ��������� ����� �������. 7 ������� ����� ��� ������� �� 0 �� 71 ��� (72 ���� ������ 0�) 12*(n-1)
	float cycle_y[cycle_len*(p_quant - 1)];	//������-����� ��� ������� ������ �������� GLE � ������ ����, ��� ������ ������ �������������� ������� ������� ������� �� ������� 12*(n-1)
	float cycle_m[cycle_len*(p_quant - 1)*m_quant];	// month precision

	float gle[2][y_quant];		//��������� ������ �� ���������� GLE ���������. � ������ ������ ������� ���� � �������� 12-59���(48 ���)

	float gle_mp[2][y_quant*m_quant];	//gle month precision

	if (hy < wy || (hy == wy && hm < wm) || (hy == wy && hm == wm && hd < wd))	{//male is elder
		digitMin = wy*wm*wd;
		maj_b = hy;
		digitMaj = hy*hm*hd;
		min_b = wy;
	}
	else{							//female male is elder or equal 
		digitMin = hy*hm*hd;
		maj_b = wy;
		digitMaj = wy*wm*wd;
		min_b = hy;
	}


	if (digitMin<1000 || digitMin>9999999 || digitMaj<1000 || digitMaj>9999999)	{	//�������� ������ ��������� �� 4 �� 7 ������ (2������ 999-26881 ���� ����� ���)
		cout << "error in digest, check bithdates" << endl; system("pause"); return 0;
	}

	find_oppoints(digitMin, cycle_p);											//������� ������� ����� ������ ��������� ������� �������� �� ����

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];							//��������� �������� GLE � ���� �����
		for (int u = 0; u < cycle_len; u++){
			cycle_y[(cycle_len * i) + u] = cycle_p[i] + (u*(curr_cycle / cycle_len));	//��������� �������� � �����-�������  ��� ������� ���� 
		}
	}

	for (int i = 0; i < y_quant; i++){ gle[0][i] = cycle_y[i + srt_age]; }		//����������� ������ �������� �� ���� � ������� GLE. ���� � 12�� �� 59� �������� ������ ��������� �������� ����������
	/*
	// month precision
	*/

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];						
		for (int u = 0; u < (cycle_len*m_quant); u++){
			cycle_m[(cycle_len * m_quant*i) + u] = cycle_p[i] + (u*(curr_cycle / (cycle_len*m_quant)));
		}
	}

	for (int i = 0; i < (y_quant*m_quant); i++){ gle_mp[0][i] = cycle_m[i + (srt_age*m_quant)];  }

	/*
	// month precision
	*/
	find_oppoints(digitMaj, cycle_p);											//������� ������� ����� ������ ��������� ������� �������� �� ����

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];
		for (int u = 0; u < cycle_len; u++){
			cycle_y[(cycle_len * (i)) + u] = cycle_p[i] + (u*(curr_cycle / cycle_len));
		}
	}

	for (int i = 0; i < y_quant; i++){ gle[1][i] = cycle_y[i + srt_age + (min_b - maj_b)]; }//����������� ������ �������� �� ���� � ������� GLE. ����������� ������ ���������� � ��������� �� ������� �� �������

	if ((min_b - maj_b) >0){						//��� ������� ������� � ��������  
		for (int i = (min_b - maj_b); i > 0; i--){	//i ��� ��������� i ����� ������ �������� � ���� ���������� ��������� -1.
			gle[1][(y_quant - i)] = -1;			//��� �������� ����� ����������� ��������(�������� ������������ �������� ����� ������ GLE (�� ������� ������ 12 ��� �����) 
		}											//� ������������ �� ������ �� ������� ������� ��� ���������� ���������� ����� GLE(���� ������ 72��)
	}

	/*
	mp start
	*/

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];
		for (int u = 0; u < (cycle_len*m_quant); u++){
			cycle_m[(cycle_len * m_quant*i) + u] = cycle_p[i] + (u*(curr_cycle / (cycle_len*m_quant)));
		}
	}

	for (int i = 0; i < (y_quant*m_quant); i++){ gle_mp[1][i] = cycle_m[i + ((srt_age + (min_b - maj_b))*m_quant)]; }


	for (int i = 0; i < y_quant*m_quant; i++){ 

		if (abs(gle_mp[1][i] - gle_mp[0][i])<0.00001){ //grade of precision
			cout << "Month precision: "<<"Most likely, your child was born in " << month_translate(i % 12) << " of " << (i / 12) + min_b + 12 << " year!" << endl;
		}
	}

	/*
	mp finish
	*/

	//for (int i = 0; i < y_quant; i++){ cout << i + min_b + srt_age << ": " << gle[0][i] << " / " << gle[1][i] << endl; }	//���������, ����������� ��������
	for (int i = 0; i < y_quant; i++){ if (abs(gle[1][i] - gle[0][i])<0.5){ cout << "Most likely, your child was born in " << i + min_b + 12 << " year!" << endl; }; }

	system("pause");
	return 0;
}

void find_oppoints(int dig, int oppoints[p_quant]) {
	oppoints[0] = (dig / 1000000);
	if (oppoints[0] == 0){
		oppoints[0] = (dig / 100000);			//6digits
		if (oppoints[0] == 0){
			oppoints[0] = (dig / 10000);		//5	
			if (oppoints[0] == 0){
				oppoints[0] = (dig / 1000);		//4
				oppoints[1] = (dig / 100) % 10;
				oppoints[2] = (dig / 10) % 10;
				oppoints[3] = (dig / 1) % 10;
				oppoints[4] = oppoints[0];
				oppoints[5] = oppoints[1];
				oppoints[6] = oppoints[2];
			}
			else{
				oppoints[1] = (dig / 1000) % 10;
				oppoints[2] = (dig / 100) % 10;
				oppoints[3] = (dig / 10) % 10;
				oppoints[4] = (dig / 1) % 10;
				oppoints[5] = oppoints[0];
				oppoints[6] = oppoints[1];
			}
		}
		else{
			oppoints[1] = (dig / 10000) % 10;
			oppoints[2] = (dig / 1000) % 10;
			oppoints[3] = (dig / 100) % 10;
			oppoints[4] = (dig / 10) % 10;
			oppoints[5] = (dig / 1) % 10;
			oppoints[6] = oppoints[0];
		}
	}
	else{
		oppoints[1] = (dig / 100000) % 10;
		oppoints[2] = (dig / 10000) % 10;
		oppoints[3] = (dig / 1000) % 10;
		oppoints[4] = (dig / 100) % 10;
		oppoints[5] = (dig / 10) % 10;
		oppoints[6] = (dig / 1) % 10;
	}
}

string month_translate(int month){
	string output;
	switch (month){
	case 0: output = "Jan"; break;
	case 1: output = "Feb"; break;
	case 2: output = "Mar"; break;
	case 3: output = "Apr"; break;
	case 4: output = "May"; break;
	case 5: output = "Jun"; break;
	case 6: output = "Jul"; break;
	case 7: output = "Aug"; break;
	case 8: output = "Sep"; break;
	case 9: output = "Oct"; break;
	case 10: output = "Nov"; break;
	case 11: output = "Dec"; break;
	default: output="ERR";
	}
	return output;
}