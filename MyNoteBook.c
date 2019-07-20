#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 
#include <string.h> 
#include <conio.h>
#include <stdlib.h>

struct note
{
	int timestamp[5];
	char dayWeek[10];
	char description[250];
	char location[100];
	int importance;

	struct note *leftByImportance;
	struct note *rightByImportance;

	struct note *leftByDate;
	struct note *rightByDate;
};

char daysOfWeek[7][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
char programName[] = "MyNoteBook";
char separator[] = "--------------------------------------------------------------------------------";
int notesCount = 0;
int globalIteration = 1;
struct note *root = NULL;

int textValidation(char *text) {
	for (int i = 0; i < strlen(text); i++)
	{
		if (text[i] == ';') {
			return 0;
		}
	}
	return 1;
}
int dateValidation(int dd, int mm, int yy, int hh, int mn) {
	int dateFlag = 0;
	if (yy >= 1900 && yy <= 9999)
	{
		if (mm >= 1 && mm <= 12)
		{
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
				dateFlag = 1;
			else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				dateFlag = 1;
			else if ((dd >= 1 && dd <= 28) && (mm == 2))
				dateFlag = 1;
			else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
				dateFlag = 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else
		return 0;

	if (dateFlag == 1) {
		if ((hh >= 0 && hh <= 23) && (mn >= 0 && mn <= 59))
			return 1;
		else
			return 0;
	}
}
int dateComparing(int *first, int *second) {

	if (first[0] == second[0] && first[1] == second[1] && first[2] == second[2] && first[3] == second[3] && first[4] == second[4])
		return 3;


	if (first[2] < second[2])
		return 1;
	if (first[2] > second[2])
		return 0;
	if (first[2] == second[2]) {
		if (first[1] < second[1])
			return 1;
		if (first[1] > second[1])
			return 0;
		if (first[1] == second[1]) {
			if (first[0] < second[0])
				return 1;
			if (first[0] > second[0])
				return 0;
			if (first[0] == second[0]) {
				if (first[3] < second[3])
					return 1;
				if (first[3] > second[3])
					return 0;
				if (first[3] == second[3]) {
					if (first[4] < second[4])
						return 1;
					if (first[4] > second[4])
						return 0;
				}
			}
		}
	}
	return 0;
}

struct note *assignByDate(struct note *tree, struct note *note) {
	if (tree == NULL)
	{
		tree = note;
		tree->leftByDate = NULL;
		tree->rightByDate = NULL;
	}
	else
		if (dateComparing(note->timestamp, tree->timestamp) == 1)
			tree->leftByDate = assignByDate(tree->leftByDate, note);
		else if (dateComparing(note->timestamp, tree->timestamp) == 0)
			tree->rightByDate = assignByDate(tree->rightByDate, note);
	return(tree);


}
struct note *addNote(struct note *tree, int dd, int mm, int yy, int hh, int mn, char *dw, char *description, char *location, int importance) {
	if (tree == NULL)
	{
		tree = (struct note*)malloc(sizeof(struct note));

		tree->dayWeek[0] = '\0';
		tree->timestamp[0] = dd;
		tree->timestamp[1] = mm;
		tree->timestamp[2] = yy;
		tree->timestamp[3] = hh;
		tree->timestamp[4] = mn;
		strcpy(tree->dayWeek, dw);
		strcpy(tree->description, description);
		strcpy(tree->location, location);
		tree->importance = importance;

		tree->leftByImportance = NULL;
		tree->rightByImportance = NULL;

		assignByDate(root, tree);

	}
	else
		if (importance < tree->importance)
			tree->leftByImportance = addNote(tree->leftByImportance, dd, mm, yy, hh, mn, dw, description, location, importance);
		else
			tree->rightByImportance = addNote(tree->rightByImportance, dd, mm, yy, hh, mn, dw, description, location, importance);
	return(tree);
}
void addController() {

	int dd, mm, yy, hh, mn, dw; // day, month, year, hours, minutes, day of week 
	char scanfix;
	char description[250] = { 0 };
	char location[100] = { 0 };
	char dayWeek[10] = { 0 };
	int importance;

	system("cls");
	printf("%s# %s / Add\n%s\n\n", separator, programName, separator);

	// Получаем дату и проверяем на валидность
A:
	printf("Date and time [DD/MM/YYYY, HH:MM]: ");
	scanf("%d/%d/%d, %d:%d", &dd, &mm, &yy, &hh, &mn);
	if (!dateValidation(dd, mm, yy, hh, mn)) {
		printf("Date invalid!\n\n");
		_getch();
		goto A;
	}

	// Получаем день недели
	printf("%s", separator);
	dw = (dd + (yy - ((14 - mm) / 12)) + (yy - ((14 - mm) / 12)) / 4 - (yy - ((14 - mm) / 12)) / 100 + (yy - ((14 - mm) / 12)) / 400 + (31 * (mm + 12 * ((14 - mm) / 12) - 2)) / 12) % 7;
	printf("[%d] Day of the week: %s\n", dw, daysOfWeek[dw]);
	strcpy(dayWeek, daysOfWeek[dw]);

	printf("%d - %s\n", dw, dayWeek);

	// Получаем описание 
C:
	printf("%s", separator);
	printf("Description [Max 250 characters]: \n");
	scanf("%c", &scanfix);
	scanf(" %250[^\n]", &description);

	if (!textValidation(description) || strlen(location) > 250) {
		printf("!!! Description must not contain a character ';' and max lenth 250 characters.. Try again.\n\n\n");
		goto C;
	}


	// Получаем местоположение 
D:
	printf("%s", separator);
	printf("Location [Max 100 characters]: \n");
	scanf(" %150[^\n]", &location);

	if (!textValidation(location) || strlen(location) > 100) {
		printf("!!! Location must not contain a character ';' and max lenth 100 characters. Try again.\n\n\n");
		goto D;
	}

	// Получаем важность
	printf("%s", separator);
B:
	printf("Importance [0-10]: \n");
	scanf("%d", &importance);

	if (importance < 0 || importance > 10) {
		printf("!!! Importance must be from 0 to 10.\n\n");
		goto B;
	}

	root = addNote(root, dd, mm, yy, hh, mn, dayWeek, description, location, importance);

	notesCount++;

	printf("\n\n\n%sDone!\n%s", separator, separator);

	_getch();

}

void treePrintImportanceAsc(struct note *tree)
{
	if (tree != NULL) {
		treePrintImportanceAsc(tree->leftByImportance);
		printf("[#%d | %d | %s] %d/%d/%d, %d:%d - %s\n%s\n\n\n", globalIteration, tree->importance, tree->dayWeek, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->location, tree->description);

		if (globalIteration == notesCount) {
			printf("\n\n\n%sThat's all. Press any key to back menu.\n%s\n", separator, separator);
			_getch();
			return;
		}
		else {
			if (globalIteration % 10 == 0) {
				printf("\n%sPress any key to display next 10 notes.\n%s\n\n\n", separator, separator);
				_getch();
			}
		}

		globalIteration++;

		treePrintImportanceAsc(tree->rightByImportance);
	}
}
void treePrintImportanceDesc(struct note *tree)
{
	if (tree != NULL) {
		treePrintImportanceDesc(tree->rightByImportance);
		printf("[#%d | %d | %s] %d/%d/%d, %d:%d - %s\n%s\n\n\n", globalIteration, tree->importance, tree->dayWeek, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->location, tree->description);

		if (globalIteration == notesCount) {
			printf("\n\n\n%sThat's all. Press any key to back menu.\n%s\n", separator, separator);
			_getch();
			return;
		}
		else {
			if (globalIteration % 10 == 0) {
				printf("\n%sPress any key to display next 10 notes.\n%s\n\n\n", separator, separator);
				_getch();
			}
		}

		globalIteration++;
		treePrintImportanceDesc(tree->leftByImportance);
	}
}
void treePrintDateDesc(struct note *tree)
{
	if (tree != NULL) {
		treePrintDateDesc(tree->rightByDate);
		printf("[#%d | %d | %s] %d/%d/%d, %d:%d - %s\n%s\n\n\n", globalIteration, tree->importance, tree->dayWeek, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->location, tree->description);

		if (globalIteration == notesCount) {
			printf("\n\n\n%sThat's all. Press any key to back menu.\n%s\n", separator, separator);
			_getch();
			return;
		}
		else {
			if (globalIteration % 10 == 0) {
				printf("\n%sPress any key to display next 10 notes.\n%s\n\n\n", separator, separator);
				_getch();
			}
		}

		globalIteration++;
		treePrintDateDesc(tree->leftByDate);
	}
}
void treePrintDateAsc(struct note *tree)
{
	if (tree != NULL) {
		treePrintDateAsc(tree->leftByDate);
		printf("[#%d | %d | %s] %d/%d/%d, %d:%d - %s\n%s\n\n\n", globalIteration, tree->importance, tree->dayWeek, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->location, tree->description);

		if (globalIteration == notesCount) {
			printf("\n\n\n%sThat's all. Press any key to back menu.\n%s\n", separator, separator);
			_getch();
			return;
		}
		else {
			if (globalIteration % 10 == 0) {
				printf("\n%sPress any key to display next 10 notes.\n%s\n\n\n", separator, separator);
				_getch();
			}
		}

		globalIteration++;
		treePrintDateAsc(tree->rightByDate);
	}
}
void showController() {

	globalIteration = 1;

	system("cls");
	printf("%s# %s / Show\n%s\n\n", separator, programName, separator);

	if (notesCount == 0) {
		printf("No availible notes.\nWould you like to add new note? [y/n]");
		switch (_getch()) {
		case 'y':
			addController();
			return;

		default:
			return;
			break;
		}
	}

	printf("Choose sorting type:\n");
	printf("	1. ASC by Date\n");
	printf("	2. DESC by Date\n");
	printf("	3. ASC by Importance\n");
	printf("	4. DESC by Importance\n");

	switch (_getch())
	{
	case '1':
		system("cls");
		printf("%s# %s / Show [by Date Asc]\n%s\n\n", separator, programName, separator);
		treePrintDateAsc(root);
		break;
	case '2':
		system("cls");
		printf("%s# %s / Show [by Date Desc]\n%s\n\n", separator, programName, separator);
		treePrintDateDesc(root);
		break;
	case '3':
		system("cls");
		printf("%s# %s / Show [by Importance Asc]\n%s\n\n", separator, programName, separator);
		treePrintImportanceAsc(root);
		break;
	case '4':
		system("cls");
		printf("%s# %s / Show [by Importance Desc]\n%s\n\n", separator, programName, separator);
		treePrintImportanceDesc(root);
		break;
	default:
		return;
	}

	//treePrint(root);


	_getch();

}

void treeExport(struct note *tree, char *exported) {
	if (tree != NULL) {
		treeExport(tree->leftByImportance, exported);
		sprintf(exported, "%s%d;%d;%d;%d;%d;%d;%s;%s;%s\n", exported, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->importance, tree->dayWeek, tree->location, tree->description);
		treeExport(tree->rightByImportance, exported);
	}
}
void treeExportByDate(struct note *tree, char *exported) {

	if (tree != NULL) {

		treeExportByDate(tree->leftByDate, exported);
		sprintf(exported, "%s%d;%d;%d;%d;%d;%d;%s;%s;%s\n", exported, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
			tree->timestamp[3], tree->timestamp[4], tree->importance, tree->dayWeek, tree->location, tree->description);
		treeExportByDate(tree->rightByDate, exported);
	}
}
void exportController() {

	FILE *exportFile;
	char exportPath[150];
	char exported[20000] = { '\0' };

	system("cls");
	printf("%s# %s / Export\n%s\n\n", separator, programName, separator);

	printf("Enter path for source file:\n");
	scanf("%s", exportPath);
	if ((exportFile = fopen(exportPath, "w")) == NULL)
	{
		printf("\nFile does not exist");
		_getch();
		return;
	}
	treeExport(root, exported);

	printf("\nExporting...");

	fwrite(exported, sizeof(char), strlen(exported), exportFile);
	fclose(exportFile);

	printf("\n\nDone!\nExported notes: %d", notesCount);
	_getch();

}

void importController(char *path) {

	FILE *importFile;
	char importPath[150];
	char imported[20000];
	int importedLen = 0;

	int added = 0;

	if (path == NULL) {
		system("cls");
		printf("%s# %s / Import\n%s\n\n", separator, programName, separator);

		printf("Enter path for source file:\n");
		scanf("%s", importPath);
		if ((importFile = fopen(importPath, "rb")) == NULL)
		{
			printf("\nFile does not exist");
			_getch();
			return;
		}

		printf("\nImporting...");
	}
	else {
		importFile = fopen(path, "rb");
	}


	fseek(importFile, 0, SEEK_END);
	importedLen = ftell(importFile);
	rewind(importFile);

	char sep[10] = ";";
	while (fgets(imported, importedLen, importFile)) {

		char temp[10][250] = { { '\0' } };

		char *istr;
		istr = strtok(imported, sep);
		int item = 1;
		int skipFlag = 0;
		while (istr != NULL)
		{
			strcpy(temp[item], istr);
			istr = strtok(NULL, sep);
			item++;
		}

		int dd = 0, mm = 0, yy = 0, hh = 0, mn = 0, importance = 0;
		char location[150] = { 0 }, description[250] = { 0 }, dayWeek[10] = { 0 };

		for (int i = 1; i < 10; i++)
			if (temp[i][0] != '\0' && temp[i][0] != 13) {
				if (i == 1) {
					dd = atoi(temp[i]);
					continue;
				}
				if (i == 2) {
					mm = atoi(temp[i]);
					continue;
				}
				if (i == 3) {
					yy = atoi(temp[i]);
					continue;
				}
				if (i == 4) {
					hh = atoi(temp[i]);
					continue;
				}
				if (i == 5) {
					mn = atoi(temp[i]);
					continue;
				}
				if (i == 6) {
					importance = atoi(temp[i]);
					continue;
				}

				if (i == 7) {
					strcpy(dayWeek, temp[i]);
					continue;
				}
				if (i == 8) {
					strcpy(location, temp[i]);
					continue;
				}
				if (i == 9) {
					strcpy(description, temp[i]);
					continue;
				}
			}
			else {
				skipFlag = 1;
			}


			if (skipFlag == 0) {
				root = addNote(root, dd, mm, yy, hh, mn, dayWeek, description, location, importance);
				added++;
			}
	}
	fclose(importFile);

	notesCount += added;
	if (path == NULL)
		printf("\n\nDone!\nImported notes: %d", added);

	_getch();

}

void filterPrint(struct note *tree, char *mask)
{
	if (tree != NULL) {
		filterPrint(tree->rightByImportance, mask);
		if (strstr(tree->location, mask))
			printf("[#%d | %d | %s] %d/%d/%d, %d:%d - %s\n%s\n\n\n", globalIteration, tree->importance, tree->dayWeek, tree->timestamp[0], tree->timestamp[1], tree->timestamp[2],
				tree->timestamp[3], tree->timestamp[4], tree->location, tree->description);
		globalIteration++;
		filterPrint(tree->leftByImportance, mask);
	}
}
void filterController() {

	char locationQuery[150] = { 0 };

	system("cls");
	printf("%s# %s / Filtration by location\n%s\n\n", separator, programName, separator);

	printf("Enter location:\n");
	scanf("%s", locationQuery);

	system("cls");
	printf("%s# %s / Filtration by location [%s]\n%s\n\n", separator, programName, locationQuery, separator);

	filterPrint(root, locationQuery);


	_getch();

}

struct note *clearTree(struct note *tree)
{
	if (tree == NULL) return tree;

	clearTree(tree->leftByDate);
	clearTree(tree->rightByDate);

	free(tree);
}
struct note *assignByImportance(struct note *tree, struct note *note) {
	if (tree == NULL)
	{
		tree = note;
		tree->leftByImportance = NULL;
		tree->rightByImportance = NULL;

	}
	else
		if (note->importance < tree->importance)
			tree->leftByImportance = assignByImportance(tree->leftByImportance, note);
		else
			tree->rightByImportance = assignByImportance(tree->rightByImportance, note);
	return(tree);
}
void rebuildTree(struct note *tree) {
	if (tree != NULL) {
		rebuildTree(tree->leftByDate);
		tree->leftByImportance = NULL;
		tree->rightByImportance = NULL;
		assignByImportance(root, tree);
		rebuildTree(tree->rightByDate);
	}
}
struct note* findMinimum(struct note *tree)
{
	if (tree == NULL)
		return NULL;
	else if (tree->leftByDate != NULL)
		return findMinimum(tree->leftByDate);
	return tree;
}
struct note *deleteNote(struct note *tree, int *timestamp) {

	if (tree == NULL) {
		printf("Not found\n\n");
		return tree;
	}

	if (dateComparing(timestamp, tree->timestamp) == 1)
		tree->leftByDate = deleteNote(tree->leftByDate, timestamp);
	else if (dateComparing(timestamp, tree->timestamp) == 0)
		tree->rightByDate = deleteNote(tree->rightByDate, timestamp);
	else
	{

		if (tree->leftByDate == tree->rightByDate)
		{
			notesCount--;
			printf("\n%sNote deleted!\n%s", separator, separator);
			free(tree);
			return NULL;
		}
		else if (tree->leftByDate == NULL || tree->rightByDate == NULL)
		{
			struct note *temp;

			if (tree->leftByDate == NULL)
				temp = tree->rightByDate;
			else {
				temp = tree->leftByDate;
			}

			notesCount--;
			printf("\n%sNote deleted!\n%s", separator, separator);
			free(tree);
			return temp;
		}
		else {

			struct note *temp = findMinimum(tree->rightByDate);

			tree->timestamp[0] = temp->timestamp[0];
			tree->timestamp[1] = temp->timestamp[1];
			tree->timestamp[2] = temp->timestamp[2];
			tree->timestamp[3] = temp->timestamp[3];
			tree->timestamp[4] = temp->timestamp[4];
			tree->importance = temp->importance;

			strcpy(tree->dayWeek, temp->dayWeek);
			strcpy(tree->location, temp->location);
			strcpy(tree->description, temp->description);

			tree->rightByDate = deleteNote(tree->rightByDate, temp->timestamp);
		}
	}

	return tree;

}
void deleteController() {

	int timestamp[5];

	system("cls");
	printf("%s# %s / Delete\n%s\n\n", separator, programName, separator);

	printf("Enter deleting note's date [DD/MM/YYYY, HH:MN]");

	scanf("%d/%d/%d, %d:%d", &timestamp[0], &timestamp[1], &timestamp[2], &timestamp[3], &timestamp[4]);
	if (!dateValidation(timestamp[0], timestamp[1], timestamp[2], timestamp[3], timestamp[4])) {
		printf("Date invalid!\n\n");
		_getch();
		return;
	}

	root = deleteNote(root, timestamp);
	if (notesCount != 0) {
		root->leftByImportance = NULL;
		root->rightByImportance = NULL;
		rebuildTree(root->leftByDate);
		rebuildTree(root->rightByDate);
	}

	_getch();

}

void main() {
A:
	system("cls");

	printf("%s# %s [%d notes]\n%s\n\n", separator, programName, notesCount, separator);

	printf("Choose, what you want:");
	printf("\n	1. Show my notes");
	printf("\n	2. Add note");
	printf("\n	3. Delete note");
	printf("\n	4. Filtration by location");
	printf("\n	5. Export notes");
	printf("\n	6. Import notes");

	printf("\n\n\n	7. Delete all notes");
	printf("\n	8. Exit");

	switch (_getch())
	{
	case '1':
		showController();
		break;
	case '2':
		addController();
		break;
	case '3':
		deleteController();
		break;
	case '4':
		filterController();
		break;
	case '5':
		exportController();
		break;
	case '6':
		importController(NULL);
		break;
	case '7':
		clearTree(root);
		notesCount = 0;
		root = NULL;
		break;
	case '8':
		return;
		break;
	default:
		break;
	}

	goto A;

	_getch();
}
