#include "stable.h"

struct vehicle cars[NUM_V];//��������
struct unit units[NUM_U];//·�ߵ�Ԫ����

int ncycle;//��ǰ����

int flow[C];//���������
double flowDelay[C];//ʱ������
double ratio[C];//������Ӧ������
double defi_ratio[C];//ͨ����ȱʧ�����飺�ɱ�������³����
int tran[C];//���ݴ��ݴ�������

int maxFlow;//�������ת������ʵ�������·����
double totalDelay;//ʱ��
double _ratio;//������Ӧ��
double _defi_ratio;//ͨ����ȱʧ��
int _tran;//���δ��ݴ���

void cycle_startup()
{
	int nreq = 0,//������������
		nrec = 0,//����������������
		l = ncycle * T,//��ǰ�����Ŀ�ʼ����
		r = l + T - 1,//��ǰ������ĩβ����
		i;

	if(ncycle < C) {
		if (REAL_MAP)
			for (int i = 0; i < NUM_V; i++) 
				for (int j = 0; j < T; j++) {
					cars[i].route[j] = cars[i].route[j + l];//���ǳ����켣
				}
		else{
			//�ڳ�ʼ��·�ߣ�������ʼ��˳��:���maxFlow�Ľ�������
			for (i = 0; i < NUM_V; i++) {
				get_passing_units(&cars[i], l, r);//��ȡ��ͨ�ŵ�·�ߵ�Ԫ
				for (int j = 0; j < T; j++)
					cars[i].route[j] = cars[i].route[j + l];//���ǳ����켣
				V2V(&cars[i]);//��ȡ��ͨ�ŵĳ���
			}
		}
		
		max_flow(cars);//��������
		flow[ncycle] = maxFlow;//�����
		flowDelay[ncycle] = totalDelay;//ʱ��
		ratio[ncycle] = _ratio;//��Ӧ��
		defi_ratio[ncycle] = _defi_ratio;//ͨ����ȱʧ��
		tran[ncycle] = _tran;//���ݴ���

		cout << "Cycle " << ncycle + 1 << "/" << C << endl;
	}
}

void run()
{
	int nreq, nrec;
	int i, t;
	
	set_seed();//��ʼ���������

	if (REAL_MAP) {
		readfile();//��ȡ����&·�ߵ�Ԫ����
		getmeet();//��ȡ���������·�ߵ�Ԫ&����
		/*for (int i = 0; i < NUM_V; i++) {//�鿴�������
			for (int j = 0; j < C*T; j++) {
				cout << i << "-->" << j << endl;
				cout << "  ������Ԫ��" << cars[i].route[j].uid << "����Ϊ��" << cars[i].route[j].u2v_distance << endl;
				for (int t = 0; t < cars[i].route[j].num_v; t++) {
					cout << "   ����������" << cars[i].route[j].vid[t] << "����Ϊ��" << cars[i].route[j].v2v_distance[t] << endl;
				}
			}
		}*/
		//system("pause");
	}
	else {
		for (i = 0; i < NUM_V; i++) init_vehicle(&cars[i]);//���� ·�ߵ�Ԫ��ʼ����������켣
	}
	
	cout << "Initializing done." << endl;

	memset(flow, 0, sizeof(flow));//��ʼ��
	memset(flowDelay, 0, sizeof(flowDelay));
	memset(ratio, 0, sizeof(ratio));
	memset(defi_ratio, 0, sizeof(defi_ratio));
	memset(tran, 0, sizeof(tran));
	for(ncycle = 0; ncycle < C; ncycle++) {
		cycle_startup();//�������ں���
	}
}

int main()
{
	
	run();
	int i;
	double tr;
	int total;
	char filename[85] = "C:\\Users\\JiangHejun\\Desktop\\~\\��������\\max_flow\\VANET7_maxflowing _degree\\result.txt"; 
	FILE *out;
	out = fopen(filename, "at");//at:д��ʽ���ļ�����������ݣ�ָ�붨λ��ĩβ��w:д��ʽ���ļ����������
	
	//��ӡʵ����Ϣ
	if(REAL_MAP)
		fprintf(out, "������ʵ�����켣����--����\n");
	else
		fprintf(out, "�����������������\n");

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