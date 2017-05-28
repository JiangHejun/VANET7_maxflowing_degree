#include "stable.h"

struct vehicle cars[NUM_V];//车辆数组
struct unit units[NUM_U];//路边单元数组

int ncycle;//当前轮数

int flow[C];//最大流数组
double flowDelay[C];//时延数组
double ratio[C];//数据响应率数组
double defi_ratio[C];//通信流缺失率数组：可表明网络鲁棒性
int tran[C];//数据传递次数数组

int maxFlow;//最大流，转化：其实是最大传输路径数
double totalDelay;//时延
double _ratio;//数据响应率
double _defi_ratio;//通信流缺失率
int _tran;//单次传递次数

void cycle_startup()
{
	int nreq = 0,//请求数据数量
		nrec = 0,//满足请求数据数量
		l = ncycle * T,//当前轮数的开始跳数
		r = l + T - 1,//当前轮数的末尾跳数
		i;

	if(ncycle < C) {
		if (REAL_MAP)
			for (int i = 0; i < NUM_V; i++) 
				for (int j = 0; j < T; j++) {
					cars[i].route[j] = cars[i].route[j + l];//覆盖车辆轨迹
				}
		else{
			//在初始化路线，调整初始化顺序:解决maxFlow的降序问题
			for (i = 0; i < NUM_V; i++) {
				get_passing_units(&cars[i], l, r);//获取可通信的路边单元
				for (int j = 0; j < T; j++)
					cars[i].route[j] = cars[i].route[j + l];//覆盖车辆轨迹
				V2V(&cars[i]);//获取可通信的车辆
			}
		}
		
		max_flow(cars);//求解最大流
		flow[ncycle] = maxFlow;//最大流
		flowDelay[ncycle] = totalDelay;//时延
		ratio[ncycle] = _ratio;//响应率
		defi_ratio[ncycle] = _defi_ratio;//通信流缺失率
		tran[ncycle] = _tran;//传递次数

		cout << "Cycle " << ncycle + 1 << "/" << C << endl;
	}
}

void run()
{
	int nreq, nrec;
	int i, t;
	
	set_seed();//初始化随机种子

	if (REAL_MAP) {
		readfile();//获取车辆&路边单元坐标
		getmeet();//获取相遇情况：路边单元&车辆
		/*for (int i = 0; i < NUM_V; i++) {//查看相遇情况
			for (int j = 0; j < C*T; j++) {
				cout << i << "-->" << j << endl;
				cout << "  相遇单元：" << cars[i].route[j].uid << "距离为：" << cars[i].route[j].u2v_distance << endl;
				for (int t = 0; t < cars[i].route[j].num_v; t++) {
					cout << "   相遇车辆：" << cars[i].route[j].vid[t] << "距离为：" << cars[i].route[j].v2v_distance[t] << endl;
				}
			}
		}*/
		//system("pause");
	}
	else {
		for (i = 0; i < NUM_V; i++) init_vehicle(&cars[i]);//车辆 路边单元初始化，随机化轨迹
	}
	
	cout << "Initializing done." << endl;

	memset(flow, 0, sizeof(flow));//初始化
	memset(flowDelay, 0, sizeof(flowDelay));
	memset(ratio, 0, sizeof(ratio));
	memset(defi_ratio, 0, sizeof(defi_ratio));
	memset(tran, 0, sizeof(tran));
	for(ncycle = 0; ncycle < C; ncycle++) {
		cycle_startup();//核心周期函数
	}
}

int main()
{
	
	run();
	int i;
	double tr;
	int total;
	char filename[85] = "C:\\Users\\JiangHejun\\Desktop\\~\\车载网络\\max_flow\\VANET7_maxflowing _degree\\result.txt"; 
	FILE *out;
	out = fopen(filename, "at");//at:写方式打开文件，不清楚内容，指针定位到末尾。w:写方式打开文件，清楚内容
	
	//打印实验信息
	if(REAL_MAP)
		fprintf(out, "采用真实车辆轨迹数据--三亚\n");
	else
		fprintf(out, "采用随机化仿真数据\n");

	//cycle
	fprintf(out, "cycle     ");
	for(i = 1; i <= C; i++)
		fprintf(out, "%8d\t", i);
	fprintf(out, "\tavg\n");

	//flow 
	fprintf(out, "flow:     ");
	total = 0;
	for(i = 0; i < C; i++) {
		fprintf(out, "%8d\t", flow[i]);
		total += flow[i];
	}
	fprintf(out, "\t%.1f\n", (float)total / C);

	//delay
	fprintf(out, "delay:    ");
	tr = 0;
	for(i = 0; i < C; i++) {
		fprintf(out, "%.6f\t", flowDelay[i]);
		tr += flowDelay[i];
	}
	fprintf(out, "\t%.6f\n", tr / (float)C);

	//ratio
	fprintf(out, "ratio:    ");
	tr = 0;
	for (i = 0; i < C; i++) {
		fprintf(out, "%.6f\t", ratio[i]);
		tr += ratio[i];
	}
	fprintf(out, "\t%.6f\n", tr / (float)C);

	//defi_ratio
	fprintf(out, "defratio: ");
	tr = 0;
	for (i = 0; i < C; i++) {
		fprintf(out, "%.6f\t", defi_ratio[i]);
		tr += defi_ratio[i];
	}
	fprintf(out, "\t%.6f\n", tr / (float)C);

	//tran 
	fprintf(out, "tran:     ");
	total = 0;
	for (i = 0; i < C; i++) {
		fprintf(out, "%8d\t", tran[i]);
		total += tran[i];
	}
	fprintf(out, "\t%.1f\n\n", (float)total / C);
	
	fclose(out);
	return 0;
}