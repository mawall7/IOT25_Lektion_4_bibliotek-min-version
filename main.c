#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BOOKS 50  //kommentar// hur används define och inte const? svar: är ett makro och har ej en typ utan ersätts med en text där namnet står och sker innan kompilering, kan vara svårare att debugga, men, sparar minne eftersom inget minne allokeras för en typ.
#define MAX_TITLE_LENGTH 100
#define MAX_LIBRARY_NAME 50
//kommentar//obs man kan även skapa struct med struct Book { char title, int pages.. }, utan typdef
//men med typedef blir det enklare att skapa den istället för struct Book myBook = {...} kan man 
//nu skriva Book myBook = {..}
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
void add_book(Library *lib, const char *title, int pages); //hade inte behövts här eftersom vi även har definitioner, men hade använts av en header fil.
void print_books(const Library *lib);
void search_book(const Library *lib, const char *title);
void borrow_book(Library *lib, const char *title);
void return_book(Library *lib, const char *title);

// ---------------- FUNCTION IMPLEMENTATIONS ----------------
//kommentar// -> Är samma som (*lib) för att komma åt det lådan som pekaren pekar på  
void add_book(Library *lib, const char *title, int pages) {
    if (lib->num_books >= MAX_BOOKS) {  
        printf("The library is full!\n");
        return;
    }
    //kommentar// strncpy används för att tilldela en char array en ny sträng, i c inte tillåtet att tilldela en array en ny sträng efter deklarationen av arrayen, då man kan skriva t.ex. char myStr[5] = "hejsan", men strncpy för att ändra på värdet efteråt.   
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
void remove_book(Library *lib, const char *title){  //kommentar//obs tänk på att lib är en pekare och användning av sizeof!(https://www.geeksforgeeks.org/cpp/using-sizof-operator-with-array-paratmeters-in-c/) men i det här fallet går det bra att använda om man vill eftersom parametern inte direkt är en array utan en struct och i structen är books inte en pekare!

    
     for (int i = 0; i < lib->num_books; i++) {
         //kommentar//funktionen returnerar 0 om parametrarna är lika dvs strängen hittas
        if (strcasecmp(lib->books[i].title, title) == 0) {  
            if (lib->books[i].is_borrowed) {
                printf("'%s' cannot be removed , book is borrowed\n", title);
                return;
            } 
            else{
                //kommentar// får varning då sizeof kan vara unsigned och kan bli fel då man jämför en int signed och en unsigned variabel. Lösning att ändra int i till size_t  i
                // for(int x = i; x < (sizeof(lib-> books) / sizeof(lib->books[0]) - 1) ; x++ ){ // size/size hela memory size / en book size = antal element
                // for(size_t x = i; x < (sizeof(lib-> books) / sizeof(lib->books[0]) - 1) ; x++ ){
                //kommentar // obs använd num_books istället för sizeof(lib -> books)
                // for(int x = i; x < (sizeof(lib-> books) / sizeof(lib->books[0]) - 1) ; x++ ){ // size/size hela memory size / en book size = antal element
                   for(int x = i; x < (lib-> num_books - 1) ; x++ ){ // size/size hela memory size / en book size = antal element
             
                    lib -> books[x] = lib -> books[x + 1];
                }
                //kommentar// num_books-- används för att minska num_books-- i arrayen kommer det finnas dubbelt av den sista Book structen , men den vill vi inte använda (är bara skräp) , num_books får då säga hur många böcker vi ska iterera över och används i for looparna i metoderna
                lib -> num_books--;  
                return;
            }
        }
        printf("%s", "Boken går inte att hitta!\n");
    
    }
   
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
        printf("5. Remove a book\n");
        printf("6. Exit\n");
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
                
                printf("Enter book title to remove: ");
                //kommentar// fgets läser hela raden även /n! vilket vi vill få bort
                fgets(title, sizeof(title), stdin);
                //kommentar strcspn(title, "\n") returnerar indexet för \n som sätt till 0 dvs tar bort det? 0 är samma sak som '\0' ? 
                title[strcspn(title, "\n")] = 0; 
                remove_book(&myLibrary, title);
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
