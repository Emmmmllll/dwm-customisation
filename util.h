
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

void die(const char *fmt, ...);
bool checkFile(const char * path);
void printBits(size_t const size, void const * const ptr);
void writeBitsToFile(size_t const size, void const * const ptr, const char *path);
void writeIntToFile(const int number, const char *path, int append);
void writeToFile(const char *text, const char *path, int append);
void echo(const char *format, ...);

static bool isDebug = false;