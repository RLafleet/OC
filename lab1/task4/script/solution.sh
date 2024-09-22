#!/usr/bin/env bash
# Предыдущая строка сообщает загрузчику программы, что надо использовать оболочку bash

# Если любая из следующих команд завершится неудачей, скрипт прекратит свою работу
set -e

if [ -d "out" ]; then
    echo "Каталог 'out' уже существует. Удаляем его содержимое."
    rm -r out/*
else
    echo "Создаем каталог 'out'."
    mkdir out
fi

cd out

whoami > me.txt

cp me.txt metoo.txt

man wc > wchelp.txt

cat wchelp.txt

wc -l wchelp.txt | cut -d ' ' -f 1 > wchelp-lines.txt

tac wchelp.txt > wchelp-reversed.txt

cat wchelp.txt wchelp-reversed.txt me.txt metoo.txt wchelp-lines.txt > all.txt

tar -cf result.tar *.txt

gzip result.tar

cd ..

if [ -f "result.tar.gz" ]; then
    echo "Файл 'result.tar.gz' уже существует. Удаляем его."
    rm result.tar.gz
fi

mv out/result.tar.gz .

rm -r out

echo "Скрипт выполнен успешно :=)"
