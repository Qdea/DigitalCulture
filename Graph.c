#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

FILE* out;
int inf = 10000; // бесконечность
int g[1000][20]; // граф
int exists[1000]; // присутствует ли вершина в графе
int power[1000]; // степени вершин
int maxp = -1; // максимальная степень
int maxv = -1; // вершина, для которой степень максимальна
int maxd = 0; // диаметр компоненты связности
int n = 1000; // граф из 1000 вершин
int m = 0; // количество рёбер
int e[10000][2]; // массив рёбер

int vA = 658;
int vB = 23;
int vC = 378;
int vD = 811;
int vE = 541;
int vF = 988;

int queue[1024];
int bq = 1023;
int eq = 1023;

// операция добавления элемента в очередь
void push(int v)
{
	if (((eq + 1) & 1023) == bq) return;
	eq = (eq + 1) & 1023;
	queue[eq] = v;
}

// операция изъятия элемента из очереди
int pop(int* v)
{
	if (eq == bq) return 0;
	bq = (bq + 1) & 1023;
	*v = queue[bq];
	return 1;
}

void calc_comp()
{
	maxd = 0; // диаметр компоненты связности
	
	int d[1000]; // хранит кратчайшее растояние от стартовой вершины до каждой вершины 
	int v; // вершина
	int u; // смежная вершина

	for (int s = 0; s < n; s++) {
		// инициализируем массив кратчайших расстояний бесконечностью
		for (int v = 0; v < n; v++) {
			d[v] = inf;
		}
		push(s); // стартовая вершина
		d[s] = 0; // кратчайшее расстояние до стартовой вершины равно нулю
		while (pop(&v)) {
			// перебираем все смежные вершины для вершины v в графе
			// их количество равно степени вершины
			for (int i = 0; i < power[v]; i++) {
				u = g[v][i]; // вершина u - это i-ая смежная вершина для вершины v
				// если кратчайший путь до вершины u ещё не найден 
				if (d[u] == inf) {
					// сохраняем кратчайшее расстояние, как расстояние до вершины v плюс 1
					d[u] = d[v] + 1; 
					// диаметр компоненты связности равен максимальному кратчайшему расстоянию
					maxd = max(d[u], maxd);
					// в очередь кладём новую вершину, до которой найден путь
					push(u);
				}
			}
		}
	}
}

void calc_path(int s, int t)
{
	int next[1000]; // хранит номер вершины, из которой попали в данную вершину
	int v;
	int u;

	// заполняем массив номеров вершин несуществующими номерами 
	// бесконечность больше максимального номера вершины
	for (int v = 0; v < n; v++) {
		next[v] = inf;
	}
	// кладём в очередь последнюю вершину,
	// чтобы узнать, из какой вершины можно попасть в вершину t
	push(t);
	next[t] = t;
	while (pop(&v)) {
		// пока путь до вершины s из вершины t не найден
		if (next[s] == inf) {
			for (int i = 0; i < power[v]; i++) {
				u = g[v][i];
				if (next[u] == inf) {
					// запоминаем номер вершины, из которой попали в вершину u
					next[u] = v;
					push(u);
				}
			}
		}
	}

	int len = 0; // для подсчёта длины пути
	v = s; // начинаем отображать путь с вершины s
	fprintf(out, "кратчайший путь из вершины %d в вершину %d: %d", s, t, s);
	while (v != t) {
		v = next[v]; // следующая вершина в искомом пути
		fprintf(out, " %d", v);
		len++;
	}
	fprintf(out, "; длина пути: %d\n", len);
}

void calc_power()
{
	maxp = -1;
	maxv = -1;

	/* заполняем массив нулями, который будем использовать для подсчёта степеней каждой вершины */
	for (int v = 0; v < 1000; v++) {
		power[v] = 0;
	}
	/* проходим по всему массиву с ребрами */
	for (int i = 0; i < m; i++) {
		int v = e[i][0];
		int u = e[i][1];
		/* заполняем граф */
		g[v][power[v]] = u;
		g[u][power[u]] = v;
		/* для каждого ребра <v, u> увеличиваем степень для вершин v и u */
		power[v]++;
		power[u]++;
	}

	for (int v = 0; v < 1000; v++) {
		if (power[v] > maxp) {
			maxp = power[v];
			maxv = v;
		}
	}
}

int valid(int v)
{
	if (v % 17 == 0) return 0;
	if (v == 434) return 0;
	if (v == 180) return 0;
	if (v == 342) return 0;
	if (v == 311) return 0;
	if (v == 604) return 0;
	if (v == 349) return 0;
	if (v == 0) return 0;
	return 1;
}

void delete()
{
	int oldm = m; // количество ребер до удаления
	m = 0; // для подсчёта ребер после удаления
	for (int v = 0; v < 1000; v++) {
		// помечаем удаленные вершины, как не существующие
		if (!valid(v)) {
			exists[v] = 0;
		}
	}

	for (int i = 0; i < oldm; i++) {
		int v = e[i][0];
		int u = e[i][1];
		// записываем ребро в массив ребер, 
		// только если обе вершины существуют
		if (exists[v] && exists[u]) {
			// временный массив не нужен, так как i всегда больше m
			// добавляем ребро <v, u>
			e[m][0] = v;
			e[m][1] = u;
			m++; // увеличиваем число ребер
		}
	}
	fprintf(out, "\n=== после удаления ===\n\n");
}

void ans1()
{
	fprintf(out, "количество рёбер: %d\n", m);
}

void ans2()
{
	calc_power();

	int i = 0;
	for (int v = 0; v < n; v++)
		// если степень вершины равна 0 и она существует, то это изолят
		if (power[v] == 0 && exists[v])
			i++;

	fprintf(out, "количество изолятов: %d\n", i);
	
	fprintf(out, "список изолятов:");
	for (int v = 0; v < n; v++)
		if (power[v] == 0 && exists[v])
			fprintf(out, " %d", v);
	fprintf(out, "\n");
}

void ans3()
{
	fprintf(out, "вершина с максимальной степенью: %d, её степень равна: %d\n", maxv, maxp);
}

void ans4()
{
	calc_comp();
	fprintf(out, "диаметр компоненты связности графа: %d\n", maxd);
}

void ans5()
{
	calc_path(vA, vB);
}

void ans6()
{
	calc_path(vC, vD);
}

void ans7()
{
	calc_path(vE, vF);
}

void answers()
{
	ans1();
	ans2();
	ans3();
	ans4();
	ans5();
	ans6();
	ans7();
}

void init()
{
	for (int v = 0; v < 1000; v++) {
		exists[v] = 1;
	}
}

void solve()
{
	init();
	answers();
	delete();
	answers();
}

void load()
{
	/* читаем все рёбра из файла */
	FILE* f = fopen("1.txt", "r");
	while (!feof(f)) {
		int i = fscanf(f, "%d %d", &e[m][0], &e[m][1]);
		m++;
	}
	fclose(f);
}

void save()
{
	out = fopen("report1.txt", "w");
	solve();
	fclose(out);
}

void main()
{
	load();
	save();
}
