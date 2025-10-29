#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BOOKS 50
#define MAX_TITLE_LENGTH 100
#define MAX_LIBRARY_NAME 50

// ---------------- STRUCT DEFINITIONS ----------------
typedef struct {
    char title[MAX_TITLE_LENGTH];
    int pages;
    bool is_borrowed;
} Book;

typedef struct {
    char name[MAX_LIBRARY_NAME];
    Book books[MAX_BOOKS];
    int num_books;
} Library;

// ---------------- FUNCTION DECLARATIONS ----------------
void add_book(Library *lib, const char *title, int pages);
void print_books(const Library *lib);
void search_book(const Library *lib, const char *title);
void borrow_book(Library *lib, const char *title);
void return_book(Library *lib, const char *title);

// ---------------- FUNCTION IMPLEMENTATIONS ----------------
void add_book(Library *lib, const char *title, int pages) {
    if (lib->num_books >= MAX_BOOKS) {
        printf("The library is full!\n");
        return;
    }

    Book new_book;
    strncpy(new_book.title, title, MAX_TITLE_LENGTH);
    new_book.pages = pages;
    new_book.is_borrowed = false;

    lib->books[lib->num_books++] = new_book;
}

void print_books(const Library *lib) {
    if (lib->num_books == 0) {
        printf("The library has no books yet.\n");
        return;
    }

    printf("\n--- %s Book List ---\n", lib->name);
    for (int i = 0; i < lib->num_books; i++) {
        printf("%d. %s (%d pages) - %s\n",
               i + 1,
               lib->books[i].title,
               lib->books[i].pages,
               lib->books[i].is_borrowed ? "Borrowed" : "Available");
    }
}

void search_book(const Library *lib, const char *title) {
    for (int i = 0; i < lib->num_books; i++) {
        if (strcasecmp(lib->books[i].title, title) == 0) {
            printf("Found: %s (%d pages) - %s\n",
                   lib->books[i].title,
                   lib->books[i].pages,
                   lib->books[i].is_borrowed ? "Borrowed" : "Available");
            return;
        }
    }
    printf("Book not found: %s\n", title);
}

void borrow_book(Library *lib, const char *title) {
    for (int i = 0; i < lib->num_books; i++) {
        if (strcasecmp(lib->books[i].title, title) == 0) {
            if (lib->books[i].is_borrowed) {
                printf("Sorry, '%s' is already borrowed.\n", title);
            } else {
                lib->books[i].is_borrowed = true;
                printf("You have borrowed '%s'.\n", title);
            }
            return;
        }
    }
    printf("Book not found: %s\n", title);
}

void return_book(Library *lib, const char *title) {
    for (int i = 0; i < lib->num_books; i++) {
        if (strcasecmp(lib->books[i].title, title) == 0) {
            if (!lib->books[i].is_borrowed) {
                printf("'%s' was not borrowed.\n", title);
            } else {
                lib->books[i].is_borrowed = false;
                printf("You have returned '%s'.\n", title);
            }
            return;
        }
    }
    printf("Book not found: %s\n", title);
}

// ---------------- MAIN PROGRAM ----------------
int main() {
    Library myLibrary = {"City Library", {}, 0};

    // Add some example books
    add_book(&myLibrary, "The Hobbit", 310);
    add_book(&myLibrary, "1984", 328);
    add_book(&myLibrary, "Pride and Prejudice", 279);
    add_book(&myLibrary, "The Catcher in the Rye", 277);

    int choice;
    char title[MAX_TITLE_LENGTH];

    do {
        printf("\n--- Library Menu ---\n");
        printf("1. Show all books\n");
        printf("2. Search for a book\n");
        printf("3. Borrow a book\n");
        printf("4. Return a book\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1:
                print_books(&myLibrary);
                break;
            case 2:
                printf("Enter book title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; // remove newline
                search_book(&myLibrary, title);
                break;
            case 3:
                printf("Enter book title to borrow: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                borrow_book(&myLibrary, title);
                break;
            case 4:
                printf("Enter book title to return: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                return_book(&myLibrary, title);
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 5);

    return 0;
}
