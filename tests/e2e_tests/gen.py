import random
import os

def generate_test_case(n, min_dim=10, max_dim=50):
    """Генерирует случайные размеры матриц для теста."""
    dims = [random.randint(min_dim, max_dim) for _ in range(n)]
    return dims

def matrix_chain_order(p):
    """Решает задачу о порядке умножения матриц динамическим программированием."""
    n = len(p) - 1
    m = [[float('inf')] * n for _ in range(n)]
    s = [[0] * n for _ in range(n)]

    for i in range(n):
        m[i][i] = 0  # Стоимость умножения одной матрицы = 0

    for l in range(2, n + 1):  # Длина подпоследовательности
        for i in range(n - l + 1):
            j = i + l - 1
            for k in range(i, j):
                q = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1]
                if q < m[i][j]:
                    m[i][j] = q
                    s[i][j] = k  # Запоминаем точку разбиения

    return m[0][n - 1], s

def get_optimal_order(s, i, j):
    """Рекурсивно восстанавливает порядок умножения."""
    if i == j:
        return []
    k = s[i][j]
    return get_optimal_order(s, i, k) + get_optimal_order(s, k + 1, j) + [k]

# Создаем папку для тестов
os.makedirs("data", exist_ok=True)

num_tests = 5
for test_id in range(1, num_tests + 1):
    n = random.randint(3, 10)  # Количество матриц
    dims = generate_test_case(n + 1)
    min_operations, s = matrix_chain_order(dims)
    order = get_optimal_order(s, 0, n - 1)

    # Запись входных данных
    with open(f"data/test{test_id}.in", "w") as f_in:
        f_in.write(f"{n + 1}\n")  # Количество элементов массива (размерности)
        f_in.write(" ".join(map(str, dims)) + "\n")

    # Запись выходных данных (оптимальный порядок умножения)
    with open(f"data/test{test_id}.out", "w") as f_out:
        f_out.write(" ".join(map(str, order)) + "\n")

    print(f"Тест {test_id} сохранен: data/test{test_id}.in и data/test{test_id}.out")

print("✅ Все тесты сгенерированы!")
