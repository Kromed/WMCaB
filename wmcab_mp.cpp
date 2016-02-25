#include <iostream>
#include <string>
using namespace std;

const int cycle_len = 12;	//продолжительность цикла 12лет
const int srt_age = 12; //возраст, от которого будет происходить расчет
const int p_quant = 7;	//число опорных точек
const int y_quant = 48;	//количество лет в обработке (фертильный возраст 12-59лет)
const int m_quant = 12;	//количество месяцев // month precision


void find_oppoints(int dig, int oppoints[p_quant]);	//нахождение опорных точек кривой GLE из магического числа WARNING! function incomplete!
string month_translate(int month);

int main(int argc, char * argv[]){
	int hy = 1987; int hm = 11; int hd = 14;
	int wy = 1984; int wm = 11; int wd = 16;

	cout << "Input male bithdate (dd mm yyyy)" << endl;
	cin >> hd >> hm >> hy;
	cout << "Input female bithdate (dd mm yyyy)" << endl;
	cin >> wd >> wm >> wy;

	int maj_b;	//год рождения старшего из пары
	int min_b;	//год рождения младшего из пары(начало отсчета в цикле GLE)

	int digitMin;	//магическое число младшего из пары
	int digitMaj;	//магическое число старшего из пары

	int cycle_p[p_quant];					//массив опорных точек, его размер задает временные рамки расчета. 7 опорных точек это возраст от 0 до 71 лет (72 года считая 0й) 12*(n-1)
	float cycle_y[cycle_len*(p_quant - 1)];	//массив-буфер для расчета точных значений GLE в каждом году, его размер должен соотвествовать размеру массива опточек по формуле 12*(n-1)
	float cycle_m[cycle_len*(p_quant - 1)*m_quant];	// month precision

	float gle[2][y_quant];		//двумерный массив со значениями GLE партнеров. в данном случае рассчет идет в возрасте 12-59лет(48 лет)

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


	if (digitMin<1000 || digitMin>9999999 || digitMaj<1000 || digitMaj>9999999)	{	//магчисла должны содержать от 4 до 7 знаков (2января 999-26881 годы нашей эры)
		cout << "error in digest, check bithdates" << endl; system("pause"); return 0;
	}

	find_oppoints(digitMin, cycle_p);											//находим опорные точки кривой жизненной энергии старшего из пары

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];							//изменение значения GLE в этом цикле
		for (int u = 0; u < cycle_len; u++){
			cycle_y[(cycle_len * i) + u] = cycle_p[i] + (u*(curr_cycle / cycle_len));	//заполняем значения в буфер-массиве  для каждого года 
		}
	}

	for (int i = 0; i < y_quant; i++){ gle[0][i] = cycle_y[i + srt_age]; }		//заполняется строка младшего из пары в массиве GLE. годы с 12го по 59й младшего задают временный интервал построения
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
	find_oppoints(digitMaj, cycle_p);											//находим опорные точки кривой жизненной энергии старшего из пары

	for (int i = 0; i < p_quant - 1; i++){
		float curr_cycle = cycle_p[i + 1] - cycle_p[i];
		for (int u = 0; u < cycle_len; u++){
			cycle_y[(cycle_len * (i)) + u] = cycle_p[i] + (u*(curr_cycle / cycle_len));
		}
	}

	for (int i = 0; i < y_quant; i++){ gle[1][i] = cycle_y[i + srt_age + (min_b - maj_b)]; }//заполняется строка старшего из пары в массиве GLE. копирование данных происходит с поправкой на разницу во времени

	if ((min_b - maj_b) >0){						//при наличии разницы в возрасте  
		for (int i = (min_b - maj_b); i > 0; i--){	//i лет последние i ячеек строки старшего в паре забиваются значением -1.
			gle[1][(y_quant - i)] = -1;			//это отражает порог фертильного возраста(задается максимальным размером длины строки GLE (за вычетом первых 12 лет жизни) 
		}											//и предохраняет от выхода за пределы массива при отсутствии заполнения ячеек GLE(годы старше 72го)
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

	//for (int i = 0; i < y_quant; i++){ cout << i + min_b + srt_age << ": " << gle[0][i] << " / " << gle[1][i] << endl; }	//служебная, дляпроверки значений
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