# OSP_Lab1_part1


**Задание для первой части лабораторной работы по ОСП**

Написать программу, которая рекурсивно при помощи nftw() обойти директории и найти все файлы, в исходном коде, которых находятся заданые байты. 




Пример использования программы 
1) Собрать при помощи make из архива программу 
2) ./lab11knmN32511 <DIR> <STRING_TO_SEARCH> 


Также программа поддерживает переменную окружения LAB11DEBUG. Если она установлена, тогда выводится отладочная информация, в которой выводится offset совпадения строки с подстрокой.

Обработка ошибок

Программа выводит ошибки, если файл не возможно открыть
