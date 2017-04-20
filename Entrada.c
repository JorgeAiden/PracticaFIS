/*
 * Entrada.c
 *
 *  Created on: 12 abr. 2017
 *      Author: Raul
 */
#include <stdio.h>
typedef struct Ficha{
    char* Comentarios; // SE CREAR UN FICHERO NUEVO
};
typedef struct Especialidad{
    char* tipo;             // lista??
   // struct Calendarios cal;
    struct Ficha ficha;
};
typedef struct Medicos{
    char* Nombre_completo;
    int Num_colegiado;
    struct Especialidad especialista[5]; //posibilidad con Maps

};
void entrada(FILE *fichero_medicos,struct Medicos listaMedicos[50]){
	int num,n=50,encontrado=0;
	char continuar;
	int *numero_colegiado;//numero de colegiado del medico
	char *usuario;
	char *dni;
	printf("1.Acceso como Medico\n");
	printf("2.Acceso como Secretaria\n");
	printf("3.Acceso como Paciente\n");
	printf("Introduce un numero segun el modo en que quieras acceder.\n");
	do{
	scanf ("%d",&num);
	fflush(stdin);
	switch(num){
		case 1: do{
				printf("Introduce numero de colegiado \n");
				scanf("%s",*numero_colegiado);
				fflush(stdin);
				for(int i=0;i<50;i++){
					if(*numero_colegiado==listaMedicos[i].Num_colegiado){
						printf("Numero de colegiado correcto\n");
						encontrado=1;
						entradaMedico(listaMedicos[i]);
					}
					else {
						printf("Numero de colegiado incorrecto\n");
						printf("¿Desea introducir otro numero de colegiado? S/N\n");
						scanf("%c",&continuar);
						fflush(stdin);
				}}}while(encontrado==0||continuar=='s'||continuar=='S');
			break;
		case 2:
			break;
		case 3:
			break;
	}
	}while(num<1 || num>3);
}

void entradaMedico(struct Medicos listaMedicos,FILE *fichaMedico){
	char *especialidad[20];
	char modificarFicha;
	int entrar,seg,seguir;
	printf("Acceso concedido a numero de colegiado: %d",listaMedicos.Num_colegiado);
	printf("¿A que especialidad desea acceder?\n");
	scanf ("%s",*especialidad);
	for(int i=0;i<5;i++){
		if(*especialidad==listaMedicos.especialista.tipo){
			printf("¿Desea modificar ficha de esta especialidad?\n");
			scanf ("%c",modificarFicha);
			do{
				fichaMedico=fopen("fichaMedicoEspecialidad.txt", "a");//Abre fichero
				if(fichaMedico==NULL)
				printf("No se accede al fichero\n");
				else{

					printf("1.Añadir informacion al fichero\n");
					printf("2.Leer el fichero\n");
					printf("Introduce numero segun accion a realizar\n");
					do{
					scanf("%d",&entrar);
					seguir=1;
					fflush(stdin);
					switch(entrar){
						case 1: do{
									seg=1;
									aniadirInfo(fichaMedico);
									printf("¿Desea salir de 'añadir informacion'? De ser asi pulse 0");
									scanf("%d",&seg);
									fflush(stdin);
								}while(seg!=0);
								break;
						case 2:	do{
									seg=1;
									leerInfo(fichaMedico);
									printf("¿Desea salir de 'leer informacion'? De ser asi pulse 0");
									scanf("%d",&seg);
									fflush(stdin);
								}while(seg!=0);
								break;
					}printf("¿Desea salir y no realizar mas acciones? De ser asi pulse 0");
					scanf("%d",&seguir);
					fflush(stdin);
					}while(entrar<1||entrar>2||seguir!=0);
				}
				printf("¿Finalizar modificacion de ficha? S/N\n");
				scanf ("%c",modificarFicha);
			}while(modificarFicha=='s'||modificarFicha=='S');
		}
		else {
			printf("No esta esta especialidad\n");
		}
	}
}
void aniadirInfo(FILE *fichaMedico){
	char dat;
	fichaMedico=fopen("fNumeros.txt","a");
	scanf("%c",&dat);
	fflush(stdin);
	fprintf(fichaMedico,"%c",dat);
}
void leerInfo(FILE *fichaMedico){
	char dat;
	fichaMedico=fopen("fNumeros.txt","r");
	fscanf(fichaMedico,"%c",&dat);
	while(!feof (fichaMedico)){
		printf("%c",&dat);
		fscanf(fichaMedico,"%d",&dat);
	}
	fclose(fichaMedico);

}
