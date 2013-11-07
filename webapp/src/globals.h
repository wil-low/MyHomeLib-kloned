#ifndef GLOBALS_H
#define GLOBALS_H
#include <sqlite3.h>

typedef struct database_s {
	sqlite3* db;
	sqlite3_stmt* st_authors_by_letter;
	sqlite3_stmt* st_authors_by_letters;
	sqlite3_stmt* st_authors_by_first_letters;
	sqlite3_stmt* st_author_by_id;
	sqlite3_stmt* st_books_by_author;
	sqlite3_stmt* st_book_file_by_id;
} database_t;

extern database_t* db_s;

#endif
