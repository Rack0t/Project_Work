//
// Created by Rafael
//

#include "Application.h"
#include <iostream>

#include "NoteSearchManager.h"

Application::Application() : dbManager("notes.db"), noteManager(dbManager) {}

void Application::run() {
    int choice;
    do {
        std::cout << std::endl << "Меню:" << std::endl;
        std::cout << "1. Добавить новую заметку или отчет" << std::endl;
        std::cout << "2. Просмотреть все заметки и отчеты" << std::endl;
        std::cout << "3. Удалить заметку или отчет" << std::endl;
        std::cout << "4. Поиск по заметкам и отчетам" << std::endl;
        std::cout << "5. Выход" << std::endl;
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;
        std::cin.ignore(); // Очищаем буфер обмена

        switch (choice) {
            case 1:
                addNewNote();
                break;
            case 2:
                noteManager.viewAllNotes();
                break;
            case 3:
                deleteNote();
                break;
            case 4:
                searchNotes();
                break;
            case 5:
                std::cout << "Выход из программы..." << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
    } while (choice != 5);
}

void Application::addNewNote() {
    std::string content;
    std::cout << "Введите вашу заметку или отчет (завершите ввод строкой с одной точкой '.'): " << std::endl;
    std::string line;
    while (std::getline(std::cin, line)){
        if (line == ".")
            break;
        content += line + "\n";
    }

    NoteSearchManager searchManager(dbManager); // Создание FTS-таблицы, если её нет
    searchManager.createFTSTable();
    Note newNote(content);
    noteManager.addNote(newNote);
}

void Application::deleteNote() {
    int id;
    std::cout << "Введите ID заметки для удаления: ";
    std::cin >> id;
    noteManager.deleteNoteById(id);
}

void Application::searchNotes() {
    NoteSearchManager searchManager(dbManager);
    searchManager.createFTSTable(); // Создание FTS-таблицы, если её нет

    std::string query;
    std::cout << "Введите поисковый запрос: " << std::endl;
    std::cin.ignore();  // Очищаем буфер
    std::getline(std::cin, query);
    searchManager.searchNotes(query);   // Выполняем поиск
}
