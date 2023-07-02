extern "C" {
    void* memset(void* src, int c, size_t count);
    void* memcpy(void* dest, const void* src, size_t count);
    int memcmp(const void* s1, const void* s2, size_t n);

    void strcpy(char* dest, const char* src);
    void strncpy(char* dest, const char* src, size_t n);
    int strcmp(const char* s1, const char* s2);

    char* strtok_r(char* str, const char* delim, char** saveptr);

    size_t strlen(const char* str);
    char* strcat(char* dest, const char* src);
    char* strncat(char* dest, const char* src, size_t n);

    int strncmp(const char* s1, const char* s2, size_t n);

    char* strupr(char* s);
    char* strnupr(char* s, size_t n);

    char* strchr(const char* s, int c);
    char* strnchr(const char* s, int c, size_t n);
    char* strrchr(const char* s, int c);

    char* strdup(const char* s);
}

class String {
private:
    int length;
    char *str;

public:
    String() {
        this->length = 0;
        this->str = nullptr;
    }
    String(const char* _str) {
        this->length = strlen(str);
        this->str = new char[length + 1];
        strcpy(this->str, _str);
    }
    ~String() {
        if (this->str != nullptr) {
            delete[] this->str;
            this->str = nullptr;
        }
    }
}