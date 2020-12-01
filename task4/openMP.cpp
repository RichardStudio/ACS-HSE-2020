#include <iostream>
#include <thread>
#include <omp.h>
#include <map>
#include <fstream>
#include <string>

// Т.к. char - это целочисленный тип, то технически условие не нарушено,
// Но я решил использовать интересные символы, т.к. мне кажется это лучше подходит под данную задачу.
std::map<char, char> charSet;
// Изначальный текст
static std::string inputStr;
// Кодированный текст для вывода
static std::string outputStr;

// Создаем словарь для кодирования.
static void makeCharSet() {
	for (int i = 'a'; i < 'z'; i++)
	{
		charSet[i] = i + 338;
	}
}
// Переводим все буквы в строчные
static std::string toLowerCase(std::string input) {
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] >= 'A' && input[i] <= 'Z') {
			input[i] = tolower(input[i]);
		}
	}
	return input;
}
// Кодируем
static void encode(std::string input, int begin) {
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] >= 'a' && input[i] <= 'z') {
			outputStr[i+begin] = charSet[input[i]];
		}
	}
}

/// Портфель задач
static int begin = 0;

void* func() {
	//Начало работы потока.
	int threadBegin;
	// Каждый поток обрабатывает по 5 символов (или меньше, если текст кончился), сразу создаю нужное кол-во потоков.
	int numOfThreads = ceil(inputStr.size() / 5.0);
	// Создаем потоки приватный threadBegin - это символ, с которого поток начинает кодировать.
	// Общий begin показывает, с какого символа начнет работать следующий поток.
#pragma omp parallel num_threads(numOfThreads) private(threadBegin) shared(begin)
	{
		//Распараллеливаем цикл for по потокам.
#pragma omp parallel for
		for (int i = 0; i < numOfThreads; i++) {
			// В этой секции происходит выделение и выполнение задачи для потока
#pragma omp critical
			{
				threadBegin = begin;
				begin += 5;
				if (threadBegin < inputStr.size()) {
					encode(inputStr.substr(threadBegin, 5), threadBegin);
				}
			}
		}
	}
}
int main()
{
	makeCharSet();
	std::cout << "Input text:" << std::endl;
	std::getline(std::cin, inputStr);
	if (inputStr.size() != 0) {
		inputStr = toLowerCase(inputStr);
		outputStr = inputStr;
		func();
		std::cout << outputStr;
	}
	else {
		std::cout << "text is missing" << std::endl;
	}
}

