#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

// list /home ./ /home/morano

char TipoFichero (mode_t m)
{
	switch (m&S_IFMT) { //and bit a bit con los bits de formato,0170000
		case S_IFSOCK: return 's'; /*socket */
		case S_IFLNK:  return 'l'; /*symbolic link*/
		case S_IFREG:  return '-'; /* fichero normal*/
		case S_IFBLK:  return 'b'; /*block device*/
		case S_IFDIR:  return 'd';/*directorio */
		case S_IFCHR:  return 'c'; /*char device*/
		case S_IFIFO:  return 'p'; /*pipe*/
		default: return '?'; /*desconocido, no deberia aparecer*/
	}
}

//~ char * ConvierteModo (mode_t m, char *permisos)
//~ {
	//~ strcpy (permisos,"---------- ");
	
	//~ permisos[0]=TipoFichero(m);
	//~ if (m&S_IRUSR) permisos[1]='r'; /*propietario*/	
	//~ if (m&S_IWUSR) permisos[2]='w';
	//~ if (m&S_IXUSR) permisos[3]='x';
	//~ if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
	//~ if (m&S_IWGRP) permisos[5]='w';
	//~ if (m&S_IXGRP) permisos[6]='x';
	//~ if (m&S_IROTH) permisos[7]='r'; /*resto*/
	//~ if (m&S_IWOTH) permisos[8]='w';
	//~ if (m&S_IXOTH) permisos[9]='x';
	//~ if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
	//~ if (m&S_ISGID) permisos[6]='s';
	//~ if (m&S_ISVTX) permisos[9]='t';
	//~ return permisos;
//~ }

char * ConvierteModo2 (mode_t m)
{
	static char permisos[12];
	strcpy (permisos,"----------");
	
	permisos[0]=TipoFichero(m);
	if (m&S_IRUSR) permisos[1]='r'; /*propietario*/	
	if (m&S_IWUSR) permisos[2]='w';
	if (m&S_IXUSR) permisos[3]='x';
	if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
	if (m&S_IWGRP) permisos[5]='w';
	if (m&S_IXGRP) permisos[6]='x';
	if (m&S_IROTH) permisos[7]='r'; /*resto*/
	if (m&S_IWOTH) permisos[8]='w';
	if (m&S_IXOTH) permisos[9]='x';
	if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
	if (m&S_ISGID) permisos[6]='s';
	if (m&S_ISVTX) permisos[9]='t';
	return (permisos);
}


//~ char * ConvierteModo3 (mode_t m)
//~ {
	//~ char * permisos;
	//~ permisos=(char *) malloc (12);
	//~ strcpy (permisos,"---------- ");
	
	//~ permisos[0]=TipoFichero(m);
	//~ if (m&S_IRUSR) permisos[1]='r'; /*propietario*/	
	//~ if (m&S_IWUSR) permisos[2]='w';
	//~ if (m&S_IXUSR) permisos[3]='x';
	//~ if (m&S_IRGRP) permisos[4]='r'; /*grupo*/
	//~ if (m&S_IWGRP) permisos[5]='w';
	//~ if (m&S_IXGRP) permisos[6]='x';
	//~ if (m&S_IROTH) permisos[7]='r'; /*resto*/
	//~ if (m&S_IWOTH) permisos[8]='w';
	//~ if (m&S_IXOTH) permisos[9]='x';
	//~ if (m&S_ISUID) permisos[3]='s'; /*setuid, setgid y stickybit*/
	//~ if (m&S_ISGID) permisos[6]='s';
	//~ if (m&S_ISVTX) permisos[9]='t';
	//~ return (permisos);
//~ }


//----------------------------------------------------------------------

// DIVIDIR ESTA FUNCIÓN EN VARIAS
// HACER LO DE LOS HARD LINKS

void impresion(char *directory){	
	
	DIR *dp;
	struct dirent *ep;   
	struct stat *info = malloc(sizeof(*info));
	struct passwd *pw;
	struct group *gr;
	time_t timeDate;
	struct tm *date;
	char buff[80];
	char path[256];
	int spaceSize=0;
	int spaceLinks=0;
	
	// Impresión del nombre del directorio (si no es el actual)
	if (strcmp(directory,"./")!=0) printf("%s:\n",directory);
	// "Apertura" del directorio introducido
	dp = opendir(directory);
	
	// Bucle para saber la longitud del entero que define el tamaño del
	// mayor de los archivos del directorio y la del entero que define
	// el número de hard links del archivo con mayor cantidad
	while ((ep = readdir(dp))!=NULL){

		if ((ep->d_name)[0]!='.'){
			
			// Elaboración del array que contendrá el path completo
			strcpy(path,directory);
			if (path[strlen(path) - 1]!='/') strcat(path,"/");
			strcat(path,(ep->d_name));
			
			if(stat(path,info) != 0)    
				printf("Error\n");
			int s=((int)(log10((info->st_size))+1));
			int l=((int)(log10((info->st_nlink))+1));
			if (s>spaceSize) spaceSize=s;
			if (l>spaceLinks) spaceLinks=l;
			//~ printf("%d \n",space);
		}
	}
	
	dp = opendir (directory);
	
	if (dp != NULL){

		while ((ep = readdir(dp))!=NULL){
			if ((ep->d_name)[0]!='.'){
				
				strcpy(path,directory);
				if (path[strlen(path) - 1]!='/') strcat(path,"/");
				strcat(path,(ep->d_name));
				
				if(stat(path,info) != 0)    
					printf("Error\n");

				pw = getpwuid(info->st_uid);
				gr = getgrgid(info->st_gid);
				timeDate = (info->st_mtime);
				date = localtime(&timeDate);
				strftime(buff, sizeof(buff), "%b %e %Y %H:%M", date);
				
				printf("%ju %s %*lu %s %s %*lu %s %s\n",
						(ep->d_ino),
						ConvierteModo2(info->st_mode),
						spaceLinks,
						(info->st_nlink),
						(pw->pw_name),
						(gr->gr_name),
						spaceSize,
						(info->st_size),
						buff,
						(ep->d_name)
					);
			}
		}
		(void) closedir (dp);
	} else
		perror ("Couldn't open the directory");	
	
	free(info);
	printf("\n");
}	

void funlist(char *comList[256]){
	
	int i=1;
	if (comList[1]==NULL) impresion("./");
	else{
		while (comList[i]!=NULL){
			impresion(comList[i]);
			i++;
		}
	}
}

void removeDir(char *directory) {
	
	DIR *dp;
	
	struct dirent *ep;   
	struct stat *info = malloc(sizeof(*info));
	
	char path[256];
	
	// "Apertura" del directorio introducido
	dp = opendir(directory);
	
	if (dp != NULL){

		while ((ep = readdir(dp))!=NULL){
			if ((ep->d_name)[0]!='.'){
				
				strcpy(path,directory);
				if (path[strlen(path) - 1]!='/') strcat(path,"/");
				strcat(path,(ep->d_name));
				
				if(stat(path,info) != 0) {    
					printf("Error\n");
				}
				
				if (((info->st_mode)&S_IFMT)==S_IFDIR) {
					printf("open   DIR: %s\n",path);
					removeDir(path);
				} else {
					//~ unlink(path);
					printf("delete ARC: %s\n",path);
				}

			}
		}
		(void) closedir (dp);
	} else {
		perror ("Couldn't open the directory");
	}
	printf("delete DIR: %s\n",directory);
	
}

void fundeltree(char *comDeltree[256]){
	
	// rmdir
	// unlink
	//~ system("rm -r");
	//~ Ejemplo: deltree /home/wndigo/Documentos/00_FIC/2o/SO/P1/a
	
	int i=1;
	if (comDeltree[1]==NULL) printf("deltree: falta un operando\n");
	else{
		while (comDeltree[i]!=NULL){
			removeDir(comDeltree[i]);
			i++;
		}
	}
	
}

void interpreter(char *com[256]){

	if (strcmp(com[0],"list")==0) {
		//~ printf("1st comand: list\n");
		funlist(com);
	} else if (strcmp(com[0],"deltree")==0){
		//~ printf("1st comand: deltree\n");
		fundeltree(com);	
	} else 
		printf("Comand not specified\n");		
}

void comands(){
	// Variable encargada de almacenar todo lo introducido por teclado:
	char buf[256];
	// Variable encargada de almacenar toda la cadena menos el 
	// salto de línea:
	char *string;
	// Variable encargada de almacenar todas las "palabras" de "forma 
	// separada":
	char *word[256];        
	int i=0;
	
    printf("$ ");
    fgets (buf, 256, stdin);
	string = strtok(buf, "\n");
	word[i] = strtok(string, " ");
   
	while (word[i]!=NULL){
		word[i+1] = strtok(NULL, " ");
		i++;
	}
    interpreter(word);
}

int main(){
	
	comands();
	
    return(0);
}

