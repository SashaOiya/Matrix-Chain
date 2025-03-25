#!/bin/bash

# Проверяем, переданы ли аргументы (1 — путь к бинарнику, 2 — путь к тестам)
if [ $# -lt 2 ]; then
    echo "❌ Использование: $0 <путь_к_бинарнику> <путь_к_папке_с_тестами>"
    exit 1
fi

binary="$1"        # Путь к бинарнику
data_dir="$2"      # Путь к папке с тестами

if [ ! -d "$data_dir" ]; then
    echo "❌ Папка $data_dir не найдена!"
    exit 1
fi

files=("$data_dir"/*.in)

if [ ${#files[@]} -eq 0 ] || [ "${files[0]}" == "$data_dir/*.in" ]; then
    echo "❌ Нет файлов с расширением .in в папке $data_dir."
    exit 1
fi

for file in "${files[@]}"; do 
    base_name=$(basename "$file" .in)  # Получаем имя файла без расширения
    expected_file="$data_dir/$base_name.out"  # Полный путь к файлу .out

    echo "📂 Обработка файла: $file"

    if [ ! -f "$expected_file" ]; then
        echo "❌ Файл с ожидаемым результатом ($expected_file) не найден!"
        exit 1
    fi

    echo "🔢 Оптимальный порядок умножения:"
    result=$("$binary" < "$file")  # Запускаем программу и сохраняем результат
    expected=$(cat "$expected_file")  # Читаем ожидаемый результат

    echo "🔹 Результат: $result"
    echo "🔹 Ожидалось: $expected"

    if [ "$result" == "$expected" ]; then
        echo "✅ Тест пройден!"
    else
        echo "❌ Ошибка!"
        exit 1
    fi
    echo "-----------------------------------"
done