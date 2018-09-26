#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXED
	char* w = nextWord(file);
	while (w != NULL) {
		for (int i=0; w[i]; i++) {
			w[i] = tolower(w[i]);
		}
		
		hashMapPut(map, w, 0);
		free(w);
		w = nextWord(file);
	}
}

int minimum(int a, int b, int c) {
	if (a == b && a == c) {
		return a;
	}else if (a <= b && a <= c) {
		return a;
	}else if (b <= a && b <= c) {
		return b;
	}else {
		return c;
	}
}

//This function taken from RosettaCode.org.
int LevenshteinDistance(const char *s, const char *t)
{
	int ls = strlen(s), lt = strlen(t);
	int d[ls + 1][lt + 1];
 
	for (int i = 0; i <= ls; i++)
		for (int j = 0; j <= lt; j++)
			d[i][j] = -1;
 
	int dist(int i, int j) {
		if (d[i][j] >= 0) return d[i][j];
 
		int x;
		if (i == ls)
			x = lt - j;
		else if (j == lt)
			x = ls - i;
		else if (s[i] == t[j])
			x = dist(i + 1, j + 1);
		else {
			x = dist(i + 1, j + 1);
 
			int y;
			if ((y = dist(i, j + 1)) < x) x = y;
			if ((y = dist(i + 1, j)) < x) x = y;
			x++;
		}
		return d[i][j] = x;
	}
	return dist(0, 0);
}



/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    HashMap* map = hashMapNew(1000);
    
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        
        // Implemented the spell checker code here..
		
		        
        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
			break;
        }
		
		
		int found = 0;
		
		if (hashMapContainsKey(map, inputBuffer) == 1) {
			found = 1;
			printf("The inputted word %s is spelled correctly.\n", inputBuffer);
		}
		
		else {
		
			//Find LevenshteinDistance of each element
			for (int i = 0; i<map->capacity && !found; i++) {
				HashLink* currLink = map->table[i];
				while (currLink != NULL) {
					currLink->value = LevenshteinDistance(inputBuffer, currLink->key);
					currLink = currLink->next;
				}
			}
		
			
			// Loop through 5 times, setting the value of the lowest LD to 99999 each time round
			HashLink ** suggestions = malloc(sizeof(struct HashLink *) * 5);
			
			for (int i = 0; i<5; i++) {
				suggestions[i] = map->table[1];
				
				for (int j = 0; j<map->capacity; j++) {
					HashLink* currLink = map->table[j];
					while (currLink != NULL) {
						if (currLink->value < suggestions[i]->value) {
							suggestions[i] = currLink;
						}
						currLink = currLink->next;
					}
				}
				suggestions[i]->value = 99999;
			}
			printf("The inputted word %s is spelled incorrectly.\n", inputBuffer);
			printf("Did you mean one of these?\n");
			for (int k=0; k<5; k++) {
				printf("\t%s\n", suggestions[k]->key);
			}
		}
    }
    
    hashMapDelete(map);
    return 0;
}


