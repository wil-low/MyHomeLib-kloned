#include <klone/context.h>

const char UNPACK_DIR[] = "/tmp/kloned";

const char SELECT_AUTHORS_BY_LETTER[] = 
	"select substr(SearchName,1,1) as alpha, count(1) from Authors group by substr(SearchName,1,1)";

const char SELECT_AUTHORS_BY_LETTERS[] = 
	"select substr(SearchName,1,:count) as alpha, count(1) from Authors where substr(SearchName,1,:count - 1) = :letters  group by substr(SearchName,1,:count)";

const char SELECT_AUTHORS_BY_FIRST_LETTERS[] = 
	"select Authors.AuthorID, LastName, FirstName, count(BookID) from Authors join Author_List on Authors.AuthorID = Author_List.AuthorID group by Author_List.AuthorID having substr(SearchName,1,?) = ? order by SearchName";

const char SELECT_AUTHOR_BY_ID[] = 
	"select LastName, FirstName from Authors where AuthorID = ?";

const char SELECT_BOOKS_BY_AUTHOR[] = 
	"select Books.BookID, Title, BookSize from Books join Author_List on Books.BookID = Author_List.BookID where AuthorID = ? order by SearchTitle";

const char SELECT_BOOK_FILE_BY_ID[] = 
	"select Folder, FileName, Ext from Books where BookID = ?";

void prepare_statement (sqlite3* db, sqlite3_stmt** stmt, const char* sql)
{
	int rc = sqlite3_prepare_v2 (db, sql, 1024, stmt, 0);
	if (rc) {
		err ("Can't prepare %s: %s\n", sql, sqlite3_errmsg(db));
		sqlite3_close (db);
	}
}

int server_init()
{
	const char* db_file = u_config_get_subkey_value (ctx->config, "database");
	int rc = sqlite3_open (db_file, &db_s->db);
	sqlite3* db = db_s->db;
	if (rc) {
		err ("Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close (db);
	}
	prepare_statement (db, &db_s->st_authors_by_letter, SELECT_AUTHORS_BY_LETTER);
	prepare_statement (db, &db_s->st_authors_by_letters, SELECT_AUTHORS_BY_LETTERS);
	prepare_statement (db, &db_s->st_authors_by_first_letters, SELECT_AUTHORS_BY_FIRST_LETTERS);
	prepare_statement (db, &db_s->st_author_by_id, SELECT_AUTHOR_BY_ID);
	prepare_statement (db, &db_s->st_books_by_author, SELECT_BOOKS_BY_AUTHOR);
	prepare_statement (db, &db_s->st_book_file_by_id, SELECT_BOOK_FILE_BY_ID);
	mkdir (UNPACK_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return 0;
}

int server_term()
{
	sqlite3_close (db_s->db);
	char syscmd[100];
	sprintf (syscmd, "rm -rf %s", UNPACK_DIR);
	system (syscmd);
	//err ("server_term complete\n");
	return 0;             
}

void hooks_setup()
{
	hook_server_init (server_init);
	hook_server_term (server_term);
}
