#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//#include <Metodos.h>

struct Calendarios{

    char dia[11];
    char hora[11];
    char esp[9];
    char nom[9];
    char dni[9];
    
} calendar;

//Variables constantes

#define NumCitas 21

//FUNCIONES PRINCIPALES

void crearMes(FILE *calendar, int numerodias, char mes[],int anio);
void crearCalendario(FILE *c, int anio);
void modificarCalendario(FILE *ca, int mes, int anio);
void eliminarCita(FILE *c, int dia, char h[], int mes, char anio[]);
void mostrarCalendarioMes(FILE *calendar, char doctor[], int mes, char anio[]);


// FUNCIONES AUXILIARES A MÉTODOS PRINCIPALES

char* getNombreMes(int num);
int getDiasMes(int num, int anio);

bool esBisiesto(int anio);
bool formatoDNI(char dni[]);
bool formatoHora(char h[]);

int buscarHora(FILE *calendar, char h[],char nFich[]);
void modificar(FILE *c, char nFich[], int idCampo, char campo[], int p, int d);


int main(){

	FILE *fich_calendar = NULL;

	/************** PRUEBAS CALENDARIO ************/

	//crearCalendario(fich_calendar, 2015);
	//crearMes(fich_calendar, 31, "ENERO", 2017);
	
	modificarCalendario(fich_calendar, 1, 2017);
	//printf("\nposhora: %d",buscarHora(fich_calendar, "10:30", "ENERO2017.txt"));
	
	//eliminarCita(fich_calendar, 5, "10:30", 1, "2017");
	mostrarCalendarioMes(fich_calendar, "Boby", 1, "2017");	
    
    return 0;

}


/******************* FUNCIONES CALENDARIO *******************/


void crearMes(FILE *calendar, int numDias, char mes[], int anio){ 
	
	//crea un mes sin citas
	//k = nº de posibles citas a lo largo del dia(jornada laboral)
	//jornada laboral (9:00 a 19:00);
	//duracion por cita 30 min;
	
	int i,j, k,m; 
	int min=0; float hora=9; char mesFich[20]; char ani[4];
		
	sprintf(ani, "%d", anio); 
	memset(mesFich, 0, sizeof(mesFich));
	
	//Generar nombre del fichero
	
	strcat(mesFich, mes);
	strcat(mesFich, ani);
	strcat(mesFich,".txt");	
	
	calendar = fopen(mesFich, "w");
	
	//Método
	
	if(calendar==NULL){
			printf("CALENDARIO NO ENCONTRADO.\n");
	}else{
		
		//Imprime los dias de los meses (11 caracteres/columna)
		
		for(i = 0; i <= numDias; i++){
		
				if(i <= 9){
					if(i==0){
						fprintf(calendar, "       HORA", i);
					}
					else{
						fprintf(calendar, "         0%i", i);
					}
				}else fprintf(calendar, "         %i", i); 
			}
		
		 //Imprime las horas y campos a rellenar para la cita (3 filas/hora)
		 
		for(k = 0; k < NumCitas; k++){

			min = (hora - ((int)hora))*60;
			fprintf(calendar, "\n%0.2d:%0.2d      ", (int)hora, min); 
			hora = hora +0.5;
			
			for(m=0; m<3;m++){
			
				if(m!=0)fprintf(calendar, "           ");
				
				for(j = 1; j<=numDias; j++){
				
					if(m==0){
						fprintf(calendar, "    #esp   ");
					}else if(m==1){
						fprintf(calendar, "    #nom   ");
					}else{
						fprintf(calendar, "    #DNI   ");
					}					
				}
				fprintf(calendar, "\n");	
			}					
		}				
	}
	fclose(calendar);
}

void crearCalendario(FILE *c, int anio){//AÑO COMPLETO

	int i;

	for (i = 1; i <= 12; ++i) {	
		crearMes(c, getDiasMes(i,anio), getNombreMes(i),anio);
	}
}

void modificarCalendario(FILE *ca, int mes, int anio){
	
	// variables 
	int correcto=0, cancel = 0, dia, puntero;
	char ani[4], nomFichero[20], hora[5],camp[10];
	
	//Generar nombre del fichero
	memset(nomFichero, 0, sizeof(nomFichero));
	sprintf(ani, "%d", anio); 
	strcat(nomFichero, getNombreMes(mes));
	strcat(nomFichero, ani);
	strcat(nomFichero,".txt");
	
	//Método
	
	ca = fopen(nomFichero, "r+");
	
	if(ca==NULL){
			printf("CALENDARIO NO ENCONTRADO.\n");
	}else{
		
		do{
			correcto=0;
			
			printf("\nIntroduzca el dia: ");
			scanf("%d", &dia);
			
			if(dia>0 && dia<=getDiasMes(mes,anio)){
				
				correcto++;
				
				printf("\nIntroduzca la hora (ej: 09:30): ");
				scanf("%s", hora);
				
				if(!formatoHora(hora)){
					printf("\n** FORMATO HORA INCORRECTO! **\n");
				}else correcto++;
				
			}else{
				printf("\n\t** Dia NO VALIDO! **\n");
			}
					
			// Si formato hora y día son válidos
			
			if(correcto == 2){
				
				puntero = buscarHora(ca, hora,nomFichero);
				
				if(puntero != 0){
					
					printf("\nIntroduzca los siguientes datos: \n");
					
					printf("\n\tEspecialidad: ");
					scanf("%s", camp);
					modificar(ca,nomFichero,1,camp,puntero,dia);
					
					printf("\n\tPrimer nombre del paciente: ");
					scanf("%s", camp);
					modificar(ca,nomFichero,2,camp,puntero,dia);
					
					printf("\n\tDNI: ");
					scanf("%s", camp);
					if(formatoDNI(camp)) modificar(ca,nomFichero,3,camp,puntero,dia);
					else {
						printf("\n\t** DNI NO VALIDO **");
					}
					
				}else{
					printf("\n\t** NO EXISTEN CITAS PARA ESA HORA **\n");
				}
					
				printf("\nSeguir(1=si 0=no): ");
				scanf("%d", &cancel);
				
			}else{
				
				printf("\nPara volver a intentarlo PULSE 1: ");
				scanf("%d", &cancel);
				system("cls");
			}					
		}while(cancel == 1);
	}
	fclose(ca);
}

void modificar(FILE *c, char nFich[], int idCampo, char campo[], int p, int d){
	
	c = fopen(nFich, "r+");
	
	if(c==NULL){
			printf("CALENDARIO mod NO ENCONTRADO.\n");
	}else{
		
		strcat(campo,"----------");
		switch(idCampo){
			case 1:
				p = p + (d*11) + 4;	
				break;
			case 2:
				p = (p+354) + (11*d) + 4;
				break;	
			case 3:
				p = (p+(354*2)) + (11*d) + 4;
				break;		
		}
		
		fseek(c,p,SEEK_SET);
		if(idCampo == 3) fwrite(campo, 9, 1, c);
		else {
			fwrite(campo, 7 , 1, c);
		}
	}
	fclose(c);	
}

void mostrarCalendarioMes(FILE *calendar, char doctor[], int mes, char anio[]){

	//Variables
	char lineFich[200];
	char nomFich[20];
	
	//Generar nombre del fichero
	strcpy(nomFich,getNombreMes(mes));
	strcat(nomFich, anio);
	strcat(nomFich, ".txt");
	
	//Método
	calendar = fopen(nomFich, "r");
	if(calendar==NULL){
			printf("CALENDARIO NO ENCONTRADO.\n");
	}else{
		
		printf("\n\n\t******************** CALENDARIO %s DEL DOCTOR %s ********************\n\n",getNombreMes(mes), doctor);
		while(!feof(calendar)){
				fgets(lineFich, 200,calendar);
				printf("%s", lineFich);
				memset(lineFich, 0, sizeof(lineFich));
			}
	}	
	fclose(calendar);
}

void eliminarCita(FILE *c, int dia, char h[], int mes, char anio[]){
	
	//Variables
	
	int  pun,i;
	char cam1[11];
	
	//Generar nombre del fichero
	char nomF[21];
	memset(nomF, 0, sizeof(nomF));
	strcat(nomF, getNombreMes(mes));
	strcat(nomF, anio);
	strcat(nomF,".txt");	
	
	//Método
	c = fopen(nomF, "r+");
	
	if(c==NULL){
			printf("CALENDARIO elmi NO ENCONTRADO.\n");
	}else{
			
		for(i=1;i<4;i++){
			pun = buscarHora(c,h,nomF);
			modificar(c,nomF,i,cam1,pun,dia);
		}
	}

	fclose(c);	
}



/******************* FUNCIONES AUXILIARES *******************/

//GETTERS

char* getNombreMes(int num){
	char *mes;
	switch(num){
	case 1: mes = "ENERO"; break;
	case 2: mes = "FEBRERO";break;
	case 3: mes = "MARZO"; break;
	case 4: mes = "ABRIL";  break;
	case 5: mes = "MAYO"; break;
	case 6: mes = "JUNIO"; break;
	case 7: mes = "JULIO"; break;
	case 8: mes = "AGOSTO"; break;
	case 9: mes = "SEPTIEMBRE";  break;
	case 10: mes = "OCTUBRE"; break;
	case 11: mes = "NOVIEMBRE"; break;
	case 12: mes = "DICIEMBRE";break;
	}
	return mes;
}

int getDiasMes(int num, int anio){
	int n;
	switch(num){
	case 1: n = 31 ; break;
	case 2: 
		if(esBisiesto(anio))n = 29;
		else { n=28; }break;
	case 3: n = 31; break;
	case 4: n = 30;  break;
	case 5: n = 31; break;
	case 6: n = 30; break;
	case 7: n = 31; break;
	case 8: n = 31; break;
	case 9: n = 30;  break;
	case 10: n = 31; break;
	case 11: n = 30; break;
	case 12: n = 31;break;
	}
	return n;
}

//OTRAS

bool esBisiesto(int anio){
	return (anio%4==0 && (anio%100!=0) || anio%400==0);
}

bool formatoHora(char h[]){
	
	//formato hora: 00:00 
	
	bool f = true;
	int i=0;
	int len=strlen(h);
	if(len==5){
		while(i<len&& f){
			if(i==2){
				if(h[i]!=':') f = false;
			}else if(h[i] >= '0' && h[i] <= '9') {
			}else f = false;
					
			i++;
		}
	}else f = false;
	
	return f;
}

bool formatoDNI(char dni[]){
	
	//formato DNI: <8 números><letra>
	
	bool f = true;
	int i=0;
	int len = strlen(dni);
	
	if(len==9){
		while(i<len-1 && f){
			if(dni[i] >= '0' && dni[i] <= '9') {
			}else f = false;
			i++;
		}
	}else f = false;

	return (f && (dni[len-1] >= 'a' && dni[len-1] <='z'));
}

int buscarHora(FILE *calendar, char h[], char nFich[]){
	
	bool enc = false;
	int posHora = 354;
	char hFich[6];
	
	
	calendar = fopen("ENERO2017.txt", "r");
	if(calendar==NULL){
			printf("CALENDARIO NO ENCONTRADO.\n");
	}else{
		while(!enc && !feof(calendar)){
			memset(hFich, 0, sizeof(hFich));
			fseek(calendar,posHora,SEEK_SET);
			fgets(hFich, 6,calendar);
			if(strcmp(hFich,h)==0){
				enc = true;
			}else {
				posHora = posHora + 1064;
			}
		}	
	}
	
	if(!enc) posHora=0;
	return posHora;
}


