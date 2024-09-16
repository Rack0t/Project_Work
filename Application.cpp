//
// Created by Rafael
//

#include "Application.h"
#include <iostream>

Application::Application() : dbManager("notes.db"), noteManager(dbManager) {}

void Application::run() {
    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить новую заметку или отчет\n";
        std::cout << "2. Просмотреть все заметки и отчеты\n";
        std::cout << "3. Удалить заметку или отчет\n";
        std::cout << "4. Поиск по заметкам и отчетам\n";
        std::cout << "5. Выход\n";
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
                break;
            case 5:
                std::cout << "Выход из программы...\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 5);
}

void Application::addNewNote() {
    std::string content;
    std::cout << "Введите вашу заметку или отчет (завершите ввод строкой с одной точкой '.'): \n";
    std::string line;
    while (std::getline(std::cin, line)){
        if (line == ".")
            break;
        content += line + "\n";
    }

    Note newNote(content);
    noteManager.addNote(newNote);
}

void Application::deleteNote() {
    int id;
    std::cout << "Введите ID заметки для удаления: ";
    std::cin >> id;
    noteManager.deleteNoteById(id);
}