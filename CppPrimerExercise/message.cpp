#include"message.h"
#include<iostream>
Message::Message(const Message & rhs):contents(rhs.contents),folders(rhs.folders)
{
    add_to_folder(*this);
}

Message& Message::operator =(const Message& rhs)
{
    remove_from_folders();
    contents=rhs.contents;
    folders= rhs.folders;
    add_to_folder(*this);
    return *this;
}

Message::~Message()
{
    remove_from_folders();
}

Message::Message(Message && rhs):contents(std::move(rhs.contents))
{
    moveFolders(&rhs);
}

Message& Message::operator =(Message&& rhs)
{
    if(this != &rhs)
    {
        remove_from_folders();
        contents = std::move(rhs.contents);
        moveFolders(&rhs);
    }
    return *this;
}
void Message::save(Folder & f)
{
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.rmvMsg(this);
}

void Message::printInfo()
{
    std::cout<<"Message ["<<contents<<"] appears "<<folders.size()<<" folder(s)."<<std::endl;
}
void Message::add_to_folder(const Message &)
{
    for(auto f:folders)
    {
        f->addMsg(this);
    }
}

void Message::remove_from_folders()
{
    for(auto f:folders)
    {
        f->rmvMsg(this);
    }
    folders.clear();
}

void Message::moveFolders(Message * m)
{
    folders = std::move(m->folders);
    for(auto f:folders)
    {
        f->rmvMsg(m);
        f->addMsg(this);
    }
    m->folders.clear();
}
void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    for(auto f:lhs.folders)
    {
        f->rmvMsg(&lhs);
    }
    for(auto f:rhs.folders)
    {
        f->rmvMsg(&rhs);
    }

    swap(lhs.contents,rhs.contents);
    swap(lhs.folders,rhs.folders);
    for(auto f:lhs.folders)
    {
        f->addMsg(&lhs);
    }
    for(auto f:rhs.folders)
    {
        f->addMsg(&rhs);
    }
}


Folder::Folder(const Folder & rhs):msgs(rhs.msgs)
{
    add_to_messages(rhs);
}

Folder& Folder::operator =(const Folder& rhs)
{
    remove_from_messages();
    msgs =rhs.msgs;
    add_to_messages(rhs);
    return *this;
}
Folder::~Folder()
{
    remove_from_messages();
}
void Folder::save(Message & m)
{
    msgs.insert(&m);
    //add m to folder
    m.addFolder(this);

}

void Folder::remove(Message & m)
{
    msgs.erase(&m);
    //remove m from folder
    m.rmvFolder(this);
}

void Folder::printInfo()
{
    std::cout<<"Folder contains "<<msgs.size()<<" messages."<<std::endl;
    int i=0;
    for(auto m:msgs)
    {
        std::cout<<"Message "<<++i<<":"<<m->contents<<std::endl;
    }
}
void Folder::add_to_messages(const Folder &f)
{
    for(auto m:f.msgs)
    {
        m->addFolder(this);
    }
}

void Folder::remove_from_messages()
{
    while(!msgs.empty())
    {
        (*msgs.begin())->remove(*this);
    }

}
void Folder::addMsg(Message * m)
{
    msgs.insert(m);
}

void Folder::rmvMsg(Message *m)
{
    msgs.erase(m);
}
