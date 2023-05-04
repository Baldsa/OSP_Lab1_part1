# OSP_Lab1_part1


**Цель работы**: 
-----------------------------------
Написать программу, которая будет выполнять 
рекурсивный поиск в каталогах и находить заданные байты в файлах.


**Описание проекта**:
-----------------------------------
Лабораторная работа предполагает написание программы на языке 
программирования C, которая будет осуществлять рекурсивный поиск в 
каталогах при помощи функции nftw() и находить заданные байты в файлах.
Для выполнения поиска в каталогах будет использоваться функция nftw(), 
которая позволяет обойти все файлы и подкаталоги в заданном каталоге и его 
подкаталогах. Для каждого найденного файла программа будет выполнять 
проверку на наличие заданных байт внутри файла.
Программа на вход принимает 3 аргумента ***<program_name>, <path_to_start_search>, <Bytes_to_search>***, после программа начинает рекурсивно искать заданные байты, начиная с **<path_to_start_search>**. Так же в проект добавлены опции:

<li>
-h(--help) – выводит справочную информацию о программе
<li>
-v(--version) – выводит версию программы. 


И добавлена возможность использовать переменную окружения 
LAB11DEBUG. С помощью ее можно вывести отладочную информацию на 
консоль. А именно вывести точное место, где был найдено совпадение байтов. 

**Пример использования программы** 
-----------------------------------
1) Собрать при помощи make из архива программу 
2) ./lab11knmN32511 <DIRECTORY_TO_START> <STRING_TO_SEARCH> 

**Обработка ошибок**
-----------------------------------
Программа выводит ошибки, если файл не возможно открыть 


- [x] Лабораторная выполнена полностью :dragon:
