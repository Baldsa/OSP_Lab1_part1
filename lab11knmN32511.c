#define _XOPEN_SOURCE 500   // определяет значение, которое должна использовать компиляция 

#include <stdlib.h>         // стандартная библиотека C для управления памятью, строками и прочим
#include <stdio.h>          // стандартная библиотека C для ввода/вывода
#include <string.h>         // стандартная библиотека C для работы со строками
#include <errno.h>          // стандартная библиотека C, которая определяет макросы для вывода сообщений об ошибках
#include <getopt.h>         // стандартная библиотека C для работы с опциями командной строки
#include <ftw.h>            // стандартная библиотека C для обхода файловой системы
#include <sys/param.h>      // системный заголовочный файл для определения константы MIN() и структуры данных struct stat

#define MAX_INDENT_LEVEL 128    // макрос, определяющий максимальный уровень вложенности директорий в обходе файловой системы
//Прототипы функций
void searching_func(const char *, const char* );
void print_entry(int __attribute__((unused)), int __attribute__((unused)), const char *);
int walk_func(const char *,const struct stat *sb __attribute__((unused)), int, struct FTW *ftwbuf);
void walk_dir(char *);
void print_help(const char*);
void print_version(const char*);
char *hex_to_bytes(const char *hexstr);
const char *program_name;  //Переменная для имени программы 
char *bytes; //Переменная для искомых байтов

void searching_func(const char *fpath, const char* bytes) { //Функция для поиска байтов в файле.
    FILE *fp; // Указатель на начало файла.
    char buffer[1024]; // Переменная char массива символов для чтения файла
    int offset = 0; // Пременная для хранения текущего смещения в файле
    int read_len; // Переменная для хранения количества прочитанных байт
    int count = 0; // Переменная для хранения количества найденных подстрок
    if ((fp = fopen(fpath, "rb")) == NULL) { // Открываем файл
        printf("Cannot open file %s\n", fpath); // Ошибка открытия файла
        return;
    }
    char* debug = getenv("LAB11DEBUG"); /// получаем значение переменной окружения LAB11DEBUG
    while ((read_len = fread(buffer, 1, sizeof(buffer), fp)) > 0) { // Цикл для считывания данных из файла
        for (int i = 0; i < read_len - 5; i++) { // Цикл для обработки данных из файла 
            if (memcmp(buffer + i, bytes, 3) == 0) { // Сравнение данных из файла и нужной подстроки 
                count++; // Счетчик++
                if (debug != NULL){ // Проверка на режим отладки 
                    printf("Found at offset %d\n", offset + i); // Где найдено совпадение
                }
            }
        }
        offset += read_len; // Прибавляем к смещению длину подстроки. 

    }
    fclose(fp);  // Закрытие файла
    if (count != 0){ 
        printf("%s: Found %d times\n", fpath, count); // Вывод сколько раз встретилась подстрока.
    }
}

void print_entry(int level __attribute__((unused)), int type __attribute__((unused)), const char *path) {
    if (!strcmp(path, ".") || !strcmp(path, "..")) // Проверяем, является ли запись текущей директорией или родительской директорией, если да, то пропускаем
        return;
    searching_func(path,bytes); // Вызываем функцию поиска searching_func для поиска байтовой последовательности в содержимом файла
}

int walk_func(const char *fpath, const struct stat *sb __attribute__((unused)), int typeflag, struct FTW *ftwbuf) {
    print_entry(ftwbuf->level, typeflag, fpath); // Функция для обработки текущей записи 
    return 0;
}

void walk_dir(char *dir) { 
    int res = nftw(dir, walk_func, 10, FTW_PHYS); // Вызов функции nftw() для обхода директории
    if (res < 0) { // Если директория не была найдена, выводим сообщение об ошибке
        fprintf(stderr, "ntfw() failed: %s\n", strerror(errno));
    }
}

int main(int argc, char *argv[]) { // main with argv(directory)
    
    
    int opt = 0;
    program_name = argv[0]; // Получаем имя программы для использования в выводе сообщений
    static struct option long_options[] = {{"help", no_argument, 0, 'h'},{"version", no_argument, 0, 'v'},{0, 0, 0, 0}}; // Определение длинных версий опций -h -v
    while ((opt = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) { // Обрабатываем аргументы командной строки
        switch (opt) {
            case 'h':
                print_help(program_name); // Выводим информацию о помощи
                exit(EXIT_SUCCESS); // Выходим из программы с успешным кодом
            case 'v':
                print_version(program_name); // Выводим информацию о версии
                exit(EXIT_SUCCESS); // Выходим из программы с успешным кодом
            default:
                 printf("Try '%s --help' for more information.\n", program_name); // Выводим сообщение о том, что переданы неправильные аргументы
                 exit(EXIT_FAILURE); // Выходим из программы с ошибкой
        }
    }
    if (argc != 3) { // Проверяем, что количество переданных аргументов правильное
        printf("Usage: %s <dir> <string>\n\n", argv[0]); // Выводим информацию о том, как использовать программу
        exit(EXIT_FAILURE);
    }

    bytes = hex_to_bytes(argv[2]); // Получаем байтовую последовательность из переданной строки
    walk_dir(argv[1]); // Запускаем обход директории
    free(bytes); // Освобождаем выделенную память
    return EXIT_SUCCESS; // Возвращаем успешный код завершения программы
}

char *hex_to_bytes(const char *hexstr) {
     // Получаем длину строки
     size_t len = strlen(hexstr);
     // Проверяем строку на валидность (четность длины, префикс "0x", минимальная длина)
     if (len % 2 != 0 || len < 2 || hexstr[0] != '0' || hexstr[1] != 'x') {
        perror("Invalid hex string");
        exit(EXIT_FAILURE);
     }
     // Вычисляем длину строки без префикса "0x"
     size_t hex_len = strlen(hexstr) - 2; // Exclude "0x" prefix
     // Выделяем память под буфер, в который будут записаны байты
     char *hex_buf = malloc(hex_len / 2);
     // Проверяем, удалось ли выделить память
     if (hex_buf == NULL) {
         perror("Memory allocation error");
         exit(EXIT_FAILURE);
     }
     // Преобразуем каждые два символа в соответствующий байт и записываем его в буфер
     for (size_t i = 0; i < hex_len; i += 2) {
        sscanf(hexstr + 2 + i, "%2hhx", &hex_buf[i / 2]);
     }
     // Возвращаем указатель на буфер
     return hex_buf;
}
    
void print_help(const char* program_name) {
     printf("Usage: %s [OPTION]...\n", program_name);
     printf("Options:\n");
     printf(" -h, --help display this help and exit\n");
     printf(" -v, --version output version information and exit\n");
     exit(EXIT_SUCCESS);
}
void print_version(const char* program_name ) {
     printf("%s version 1.0\n", program_name);
     printf("info: Finds bytes in all files of the directory\n");
     printf("Author: Kopylov Nikita\n");
     printf("Group: N32511\n");
     printf("Lab: 1.01 variant 4\n");
     exit(EXIT_SUCCESS);
}
