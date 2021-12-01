
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/*
	2000992
	Borisenko Iuliia
	Kein/e Partner/in
	Note benoetigt
	kein Papierschein
	macOS-Platform
	Titel "Hangman"
	Beschreibung: Man kann in einmaligem Turn Hangman in dem Terminal spielen,
	in dem das Program folgendermassen Kompiliert: gcc -std=c11 -Wall -Wimplicit-fallthrough -Wpedantic -o eig Eigenes.c
	und startet: ./eig

	Es wird gleich ein Wort geraten, in dem die entsprechende, neben liegende Datei "Woerter.txt" gefunden wird
	und daraus wird ein Wort zufaellig rausgesucht und fuer das Erraten vorgeschlagen.
	Man hat insgesamt 6 Versuche - "Lebens", um alle existierenden Buchstaben zu erraten.
	Wenn alle 6 Versuche durchgefallen sind, dann wird man "erhaengt".
	Und das Spiel kommt mit einem entsprechenden Ergebnis zu Ende: entweder gewonnen oder verloren.
*/

//--Globale Variable
const int globalWordCounter = 33; //Anzahl an Woertern in "Woerter.txt"-Datei
char ** allWordsFromFile; //Array, wo alle Woerter asu "Woerter.txt"-Datei
char chGlobal = ' '; //jedes, vom Nutzer eingetragene Zeichen
char * body = NULL; //wird jedes "Hangman-Koerperteil" hingespeichert
char * secretWordGlobal = NULL; //zufaellig generiertes Wort
char * decryptedWordGlobal = NULL; //immer wieder aktuell aussehendes, erratenes Wort


//--Funktionen
//generateRandomWordFromFile generiert zuerst ein Zahl-Index zwischen 0 und 32 inklusive.
//Danach wird entsprechend unter diesem Index das Wort aus allWordsFromFile-Array ausgewaehlt und geraten.
char * generateRandomWordFromFile() {
	srand(time(0));
	int randomInt = rand() % globalWordCounter; //0-32 einschliesslich beide
	//printf("Die Zufallszahl ist: %d\n", randomInt);
	char * secretWord = malloc(strlen(allWordsFromFile[randomInt]));
	secretWord = allWordsFromFile[randomInt];
	//printf("%s\n", secretWord);
	printf("\n");
	printf("Die Länge des verschlüßelten Wortes: %lu\n", strlen(secretWord));
	free(secretWord);
	return secretWord;
}

//die Datei "Woerter.txt" durchgehen,
//die Zeilenumrueche beachten und
//entsprechend jedes einzelne Wort extrahieren und in die Array reinspeichern
FILE * openFile() {
	FILE * textFile=fopen("Woerter.txt", "r"); //read-Modus
  if(textFile == NULL){
    printf("%s\n", "Es wurde keine Datei gefunden! Bitte den Pfad ueberpruefen!");
    return 0;
  }

  int ch;
  int ifNewLine = 1; //true
	int k = 0;
	int l = 0;

	allWordsFromFile = (char **) malloc(globalWordCounter * sizeof(char *));
	for(int i = 0; i < globalWordCounter; i++){
		allWordsFromFile[i] = (char *) malloc(100 * sizeof(char));
	}

	while ((ch = fgetc(textFile)) != EOF) { //bis End Of File
		if (ch == (char) '\n') {
			ifNewLine = 1;
			k++;
			l = 0;
		}
		else {
			ifNewLine = 0;
			allWordsFromFile[k][l] = ch;
			l++;
		}
	}
  fclose(textFile);
	free(allWordsFromFile);
	return textFile;
}

//ueberpruefen, ob der Nutzer genau eine Buchstabe eingetragen hat.
//Die Lower-/ und UpperCase wird zu LowerCase ueberfuehrt.
int ifTheLetter() {
	char ch = ' ';
	printf("\nEs darf nur eine Buchstabe eingetragen werden!\n");
	scanf("%c", &ch);
	fflush(stdin);
	if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
		chGlobal = tolower(ch);
		return 1; //true
	}
	else { //wenn falsch eingetragen, dann rekursiv wieder dem Nutzer das Eintragen anbieten
		return ifTheLetter();
	}
}

//ueberpruefen, ob das eingetragene Zeichen in dem geratenen Wort vorhanden ist.
//Falls der Nutzer das schon wiederholt eingetragene Zeichen wieder mal eintraegt,
//das im geratenen Wort vorhanden ist,
//wird dieser Fall nicht als Fehler wahrgenommen.
int proveIfContains() {
	char * c = (char *) malloc(sizeof(char));
	c[0] = chGlobal;
	if(strstr(secretWordGlobal, c) != NULL){ //quasi contains-Ueperpruefen
		free(c);
		return 1;
	}
	else {
		free(c);

		return 0;
	}
}

//ueberpruefen, ob es noch leere Zeichen gibt, die im Wort noch zu erraten sind
int ifAllLettersGuessed() {
	char * c = (char *) malloc(sizeof(char));
	c[0] = '_';
	if(strstr(decryptedWordGlobal, c) == NULL) {
		printf("%s\n", "Du hast gewonnen!!!");
		free(c);
		return 1; //All Woerter sind richtig geraten
	}
	free(c);

	return 0;
}

//die eingetragene Buchstabe wird dem decryptieretn Wort uebergeben um anzeigen zu lassen
void displaceLetter() {
	for(int i = 0; i < strlen(secretWordGlobal); i++) {
		if(secretWordGlobal[i] == chGlobal) {
			decryptedWordGlobal[i] = chGlobal;
		}
	}
}

//ausprinten, wie das verschluesselte Wort aktuell aussieht
void printGuessedWordInDecryption() {
	printf("Das verschlüßelte Wort: %s\n", decryptedWordGlobal);
}

void printScore(int score) {
	printf("SCORE: %d\n", score);
}

//das Ausprinten des Hangmans wurd inspiriert hier: https://gist.github.com/saroj22322/aa2f0849f33736395544c2d341ab3722
void printHangman(int mistakes) {
	switch(mistakes) {
		case 6: body[6] = '\\';
			break;
		case 5: body[5] = '/';
			break;
		case 4: body[4] = '\\';
			break;
		case 3: body[3] = '|';
			break;
		case 2: body[2] = '/';
			break;
		case 1: body[1] = ')', body[0] = '(';
			break;
		default: break;
	}

	printf("\t _________\n"
	       "\t|         |\n"
	       "\t|        %c %c\n"
	       "\t|        %c%c%c\n"
	       "\t|        %c %c\n"
	       "\t|             \n"
	       "\t|             ", body[0], body[1], body[2],
	       body[3], body[4], body[5], body[6]);
}

void displayChanges(int mistakes, int score) {
	displaceLetter();
	printGuessedWordInDecryption();
	printHangman(mistakes);
	printScore(score);
}

//Hauptmethode, wo das Spiel gesteuert wird
void guessTheWord() {
	int score=6;
	int mistakes=0;
	printf("\n");
	while (score != 0) {
		if(ifTheLetter() == 1) {
			if(proveIfContains() == 1) {
				displayChanges(mistakes, score);
				if(ifAllLettersGuessed()) {
					break;
				}
			}
			else {
				score--;
				mistakes++;
				displayChanges(mistakes, score);
				if(score == 0){
					printf("%s\n", "Du hast verloren!!!");
					printf("Das verschlüßelte Wort war: %s\n", secretWordGlobal);
				}
			}
		}
	}
}

int main (void) {
		printf("\n");
		printf("%s\n", "Hallo! Lass uns mal Hangman spielen!");
		printf("%s\n", "Du sollst das von mir ausgedachte und verschluesselte deutsche Wort erraten. Dabei hast du Versuche!");
		printf("%s\n", "Bitte beachte folgende Regeln: Umlautbuchstaben 'ä', 'ö', 'ü' werden entsprechend 'ae', 'ou', 'ue' kodiert.");
		printf("%s\n", "Und die Eszett-Buchstabe 'ß' wird als 'ss' verschluesselt.");
		printf("%s\n", "Es wird nur ein einzelnes Wort geraten, ohne Bindestriche, ohne Leerzeichen, ohne irgendwelche anderen nicht-alphabetischen Zeichen.");
		printf("%s\n", "Die Groß- oder Kleinschreibung spielt keine Rolle");

		printf("\n");

		printf("%s\n", "Ich habe für dich ein Wort geraten. Probiere mal das zetzt zu erfahren!");
		//hier kommt ein warning
		FILE * textFile = openFile(); // warning: unused variable 'textFile' [-Wunused-variable]
		char * secretWord = generateRandomWordFromFile();
		//printf("Random generiertes Wort ist: %s\n", secretWord);

		secretWordGlobal = malloc(sizeof(char) * strlen(secretWord));
		for(int i = 0; i < strlen(secretWord); i++){
			secretWordGlobal[i] = tolower(secretWord[i]); //das geratene Wort an LowerCase ueberfuehren
		}

		decryptedWordGlobal = malloc(sizeof(char) * strlen(secretWordGlobal));
		for(int i = 0; i < strlen(secretWord); i++){
			decryptedWordGlobal[i] = '_';
			printf("%c ", decryptedWordGlobal[i]);
		}

		body = malloc(6 * sizeof(char));
		for(int i = 0; i < strlen(body); i++){
			body[i] = ' ';
		}

		guessTheWord();
		free(secretWordGlobal);
		free(decryptedWordGlobal);
		free(body);
}
