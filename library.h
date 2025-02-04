#pragma once


typedef struct  {
    int id;
    char* title;
    char* authors;
    char* publisher;
    char* pubDate;
    int pages;
    int qty;
} Book;

Book* fetchBooks(char*, int*);
void search(int** idx, int *numResults, Book** books, int nbooks, char* searchStr);
void advancedSearch(int** idx, int *numResults, Book** books, int nbooks, char* title, char* author, char* publisher);
int issueBook(char* uname, int bookID, int nweeks);
void updateBooks(Book *, int, int);
