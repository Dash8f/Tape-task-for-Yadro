# Эмуляиця работы с лентами памяти на примере сортировки массива данных

**Глушнёв К.**

## Задача

Имеется входная лента длины **N**, содержащая элементы типа **integer**.
Необходимо отсортировать числа с нее и записать их на выходную ленту того же размера.
Оперативная память **M**(количество чисел, которые можно одновременно загружать в память) ограничена.
Для реализации можно создавать временные ленты, на которых можно хранить какую-то временную
информацию, необходимую в процессе работы алгоритма.

## Коментарий

Числа на ленте будем записывать в виде двочиных чисел размера 32 бита, так любые числа будут иметь одинаковый размер и будет проще их читать и записывать.
Название входного и выходного файлов подаются в аргументы командной строки при запуске программы.
Настройка параметров программы осуществляется через файл **config.txt** (не требует перекомпиляции), там нужно указать размер входной ленты а также количество оперативной памяти.


## Сборка

1. Клонируйте мой репозиторий, используя

```
git clone git@github.com:Dash8f/Tapes.git
```
2. Соберите проект

```
make
```
3. Создайте файл с данными

```
make new_data
```
и внесите соответсвующие изменения в config файл
4. Запустите

```
make run
```


