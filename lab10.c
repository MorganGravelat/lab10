#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// Trie structure
struct Trie {
    struct Trie *children[26];
    int occurrences;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, const char *word) {
    struct Trie *pCrawl = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = calloc(1, sizeof(struct Trie));
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->occurrences++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, const char *word) {
    struct Trie *pCrawl = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->occurrences;
}

// deallocate the trie structure
void deallocateTrie(struct Trie *pTrie) {
    for (int i = 0; i < 26; i++) {
        if (pTrie->children[i])
            deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie *createTrie() {
    return calloc(1, sizeof(struct Trie));
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(const char *filename, char ***pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int numWords;
    fscanf(file, "%d\n", &numWords);
    *pInWords = malloc(numWords * sizeof(char*));

    char word[MAX_WORD_LENGTH];
    for (int i = 0; i < numWords; i++) {
        fscanf(file, "%99s\n", word);
        (*pInWords)[i] = strdup(word);
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char **inWords;
    
    int numWords = readDictionary("dictionary.txt", &inWords);
    if (numWords < 0) {
        printf("Failed to read dictionary\n");
        return 1;
    }
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    deallocateTrie(pTrie);
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }
    free(inWords);
    return 0;
}
