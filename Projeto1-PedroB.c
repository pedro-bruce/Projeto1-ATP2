#include <stdio.h>
#include <stdlib.h>

void openFiles(FILE **f1, FILE **f2, FILE **f3, FILE **f4);  //abre os arquivos
void closeFiles(FILE *f1, FILE *f2, FILE *f3, FILE *f4); //fecha os arquivos
void threshold(FILE *f1, FILE *f2); //trabalha com o arquivo de limiarização
void negative(FILE *f1, FILE *f3);  //trabalha com o arquivo de negativação
void histogram(FILE *f1, FILE *f4); //trabalha com o arquivo do histograma
void firstLines(FILE *in, FILE*out);//percorre as primeiras linhas do arquivo 

int main(){
	FILE *f1, *f2, *f3, *f4;
	
	openFiles(&f1, &f2, &f3, &f4);
	threshold(f1, f2);
	negative(f1, f3);
	histogram(f1, f4);
	closeFiles(f1, f2, f3, f4);
	
	return 0;
}

void openFiles(FILE **f1, FILE **f2, FILE **f3, FILE **f4){
	*f1 = fopen("exemplo.pgm", "r");      //abre a imagem que será trabalhada
	*f2 = fopen("exemplo_lim.pgm", "w");  //cria um arquivo de limiarização
	*f3 = fopen("exemplo_neg.pgm", "w");  //cria um arquivo de negativação
	*f4 = fopen("exemplo_hist.txt", "w"); //cria o histograma
	
	if(*f1 == NULL || *f2 == NULL || *f3 == NULL || *f4 == NULL){
		printf("Erro! Os arquivos nao foram abertos.");
		return;
	}
}

void threshold(FILE *f1, FILE *f2){
	firstLines(f1, f2);
	
	int val;
	
	do{
		fscanf(f1, "%d", &val); //lê o valor do tom
		
		if(val<150)
			val = 0;
			
		else val = 255;
		
		fprintf(f2, "%d ", val); //printa o novo valor do tom
	}while(!feof(f1));
	
	rewind(f1);
}

void negative(FILE *f1, FILE *f3){
	firstLines(f1, f3);
	
	int val;
	
	do{
		fscanf(f1, "%d", &val); //lê o valor do tom
		fprintf(f3, "%d ", 255-val); //printa 255 - o valor do tom
	}while(!feof(f1));
	
	rewind(f1);
}

void histogram(FILE *f1, FILE *f4){
	int i, val, hist[256] = {0};
	int count = 0;
	char c;
	
	while(count != 3){ //percorre as 3 primeiras linhas do arquivo (tipo, linhas e colunas, tom de cinza)
		fscanf(f1, "%c", &c);
		
		if(c == '\n')
			count++;
	}
	
	do{
		fscanf(f1, "%d", &val); //lê o tom de cinza
		
		hist[val]++; //incrementa na respectiva posição do tom
	}while(!feof(f1));
	
	for(i=0;i<256;i++)
		fprintf(f4, "Tom: [%d] = %d\n", i, hist[i]); //printa a quantidade do respectivo tom no arquivo
		
	rewind(f1);
}

void firstLines(FILE *in, FILE *out){
	char c[3]; //dois caracteres e '\0'
	
	fscanf(in, "%s", c); //lê o tipo do arquivo
	fprintf(out, "%s\n", c); //printa o tipo do arquivo
	
	int col, lin, max;
	
	fscanf(in, "%d %d", &col, &lin); //lê o número de colunas e linhas da imagem
	fprintf(out, "%d %d\n", col, lin);//printa o número de colunas e linhas da imagem
	
	fscanf(in, "%d", &max); //lê o número máximo de tons de cinza
	fprintf(out, "%d\n", max);//printa o número máximo de tons de cinza
}

void closeFiles(FILE *f1, FILE *f2, FILE *f3, FILE *f4){
	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
}
