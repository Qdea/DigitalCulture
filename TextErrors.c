#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dict_item {
	char word[50];
	int freq;
	int used;
};

struct dict_list {
	struct dict_item a[5000];
	int count;
};

struct dict_list d;

int find_in_dict(char* s)
{
	int left = 0;
	int right = d.count - 1;
	do {
		int p = (left + right) / 2;
		if (strcmp(d.a[p].word, s) < 0) {
			left = p;
		}
		else {
			right = p;
		}
	} while (right - left > 1);

	if (strcmp(d.a[left].word, s) == 0) {
		return left;
	}
	if (strcmp(d.a[right].word, s) == 0) {
		return right;
	}
	return -1;
}

int distance(char* s1, char* s2, int diff)
{
	if (diff > 2) {
		return diff;
	}//если текущая разница между словами больше 2 символов - допустимой границы в задании, то выходим, возвращая эту разницу
	int d = 3;//устанавливаем худший случай для редакторского расстояния
	if (*s1 == '\0' && *s2 == '\0') {
		d = diff;
	}//если слова пустые или символы в нем закончились, то редакторскому расстоянию присваиваем текущую разницу
	if (*s1 != '\0' && *s2 != '\0') {
		d = distance(s1 + 1, s2 + 1, (*s1 == *s2) ? diff : diff + 1);
	}//если слова не пустые, то пропустить сивол в обоих словах; 
	//если символы до пропуска равны, то оставляем разницу без измений, 
	//иначе увеличиваем разницу на 1
	if (d > diff + 1 && *s1 != '\0') {
		d = min(distance(s1 + 1, s2, diff + 1), d);
	}//если текущее редакторское расстояние больше текущей разницы, увеличенной на единицу и в первом слове
	//есть символы, то взять минимум от расстояния между первым словом, смещенным на один символ, и вторым словом и текущим
	//редакторским расстоянием и записать его в новое редакторское расстояние
	if (d > diff + 1 && *s2 != '\0') {
		d = min(distance(s1, s2 + 1, diff + 1), d);
	}//если текущее редакторское расстояние больше текущей разницы, увеличенной на единицу и во втором слове
	//есть символы, то взять минимум от расстояния между вторым словом, смещенным на один символ, и первым словом и текущим
	//редакторским расстоянием и записать его в новое редакторское расстояние
	return d;//возвращаем текущее редакторское расстояние
}

void load_dict() {
	FILE* f;
	f = fopen("dict1.txt", "r");
	int n = 0;
	while (!feof(f)) {
		fscanf(f, "%s %d", d.a[n].word, &d.a[n].freq);
		d.a[n].used = 0;
		n++;
	}
	d.count = n;
	fclose(f);
}

char to_lower(char c)
{
	if (c >= 'А' && c <= 'Я') {
		return c - 'А' + 'а';
	}
	if (c == 'Ё') {
		return c - 'Ё' + 'ё';
	}
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 'a';
	}
	return c;
}

int is_separator(char c)
{
	return c == ' ' || c == '.' || c == ',' || c == ':' || c == '!' || c == '?'
		|| c == ';' || c == '(' || c == ')' || c == '\r' || c == '\n'
		|| c == '«' || c == '»';
}

void load_text()
{
	FILE* f;
	FILE* out;
	f = fopen("brain019.txt", "r");
	out = fopen("words1.txt", "w");
	int n = 0;
	int m = 0;
	char w[50];

	char c = 0;
	while (!feof(f)) {
		c = fgetc(f);
		if (is_separator(c)) {
			if (m > 0) {
				w[m] = '\0';
				int pos = find_in_dict(w);
				if (pos == -1) {
					int min_diff = 3;
					int j = -1;
					int freq = 0;
					for (int i = 0; i < d.count; i++) {
						int diff = distance(w, d.a[i].word, 0);
						if (diff < min_diff || diff == min_diff && d.a[i].freq > freq) {
							j = i;
							min_diff = diff;
							freq = d.a[i].freq;
						}
						if (diff == 0) {
							break;
						}
					}
					if (min_diff > 0) {
						fprintf(out, "%s %s %d\n", w, d.a[j].word, min_diff);
					}
					else {
						d.a[j].used++;
					}
				}
				else {
					d.a[pos].used++;
				}
			}
			m = 0;
		}
		else {
			w[m] = to_lower(c);
			m++;
		}
	}
	fclose(f);

	int count = 0;
	int unique_count = 0;
	for (int i = 0; i < d.count; i++) {
		if (d.a[i].used > 0) {
			unique_count++;
		}
		count += d.a[i].used;
	}
	fprintf(out, "%d %d\n", unique_count, count);
	fclose(out);
}

void main()
{
	load_dict();
	load_text();
}
