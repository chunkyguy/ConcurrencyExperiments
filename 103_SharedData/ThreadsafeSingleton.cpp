//
//  ThreadsafeSingleton.cpp
//  TryCPP
//
//  Created by Sid on 13/10/14.
//  Copyright (c) 2014 whackylabs. All rights reserved.
//

#include "ThreadsafeSingleton.h"
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <thread>

class Library {
public:

    static Library *SharedInstance()
    {
        std::call_once(onceFlag_, []{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            shared_ = new Library();
        });
        return shared_;
    }
    
    static void Dealloc()
    {
        if (shared_) {
            delete shared_;
        }
    }
    
    Library() :
    books_({
        {"Ernest Hemingway", "Old man and the sea"},
        {"Aldous Huxley", "Brave new world"},
        {"Aldous Huxley", "The Genius and the Goddess"},
        {"Aldous Huxley", "Antic Hay"},
        {"Salman Rushdie", "Midnight's children"},
        {"Fyodor Dostovesky", "Crime and punishment"},
        {"Boris Pasternak", "Doctor Zhivago"},
        {"Bernard Shaw", "Arms and the Man"},
        {"Bernard Shaw", "Man and Superman"}
    })
    {
        std::cout << "Library " << this << std::endl;
    }
    
    ~Library()
    {
        std::cout << "~Library " << this << std::endl;
    }
    
    std::vector<std::string> GetBooks(const std::string &author) const
    {
        typedef std::multimap<std::string, std::string>::const_iterator BookIt;
        std::pair<BookIt, BookIt> range =  books_.equal_range(author);
        std::vector<std::string> bookList;
        for (BookIt it = range.first; it != range.second; ++it) {
            bookList.push_back(it->second);
        }
        return bookList;
    }
    
    std::size_t GetSize() const
    {
        return books_.size();
    }
    
private:
    std::multimap<std::string, std::string> books_;
    static Library *shared_;
    static std::once_flag onceFlag_;
};
Library *Library::shared_ = nullptr;
std::once_flag Library::onceFlag_;

void PrintBooks(const std::string &author)
{
    std::cout << "Searching for author: " << author << std::endl;
    std::vector<std::string> list = Library::SharedInstance()->GetBooks(author);
    std::copy(list.begin(), list.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
}

void PrintSize()
{
    std::cout << "Library Size: " << Library::SharedInstance()->GetSize() << " books" << std::endl;
}

void SerialImpl()
{
    std::string searchAuthor("Aldous Huxley");
    PrintBooks(searchAuthor);
    PrintSize();
}

void ConcurrentImpl()
{
    std::string searchAuthor("Aldous Huxley");
    std::thread bookSearchThread(PrintBooks, std::ref(searchAuthor));
    std::thread libSizeThread(PrintSize);
    
    bookSearchThread.join();
    libSizeThread.join();
}

void main()
{
    //        SerialImpl();
    ConcurrentImpl();
    
    Library::Dealloc();
}
