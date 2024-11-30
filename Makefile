# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE=200809L
LDFLAGS =

# Директории
SRCDIR = .
OBJDIR = obj
BINDIR = .

# Имя итогового исполняемого файла
TARGET = social_network

# Список исходных файлов
SRCS = $(wildcard $(SRCDIR)/*.c)

# Создание списка объектных файлов
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Правила сборки
all: $(BINDIR)/$(TARGET)

# Создание итогового исполняемого файла
$(BINDIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Компиляция исходных файлов в объектные
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR) # Создаем папку obj, если её нет
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка всех скомпилированных файлов
clean:
	@rm -rf $(OBJDIR) # Удаление папки obj и её содержимого
	@rm -f $(BINDIR)/$(TARGET) # Удаление исполняемого файла
	@echo "All cleaned up!"

# Фиктивные цели
.PHONY: all clean
