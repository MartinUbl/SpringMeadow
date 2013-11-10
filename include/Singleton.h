#ifndef SM_SINGLETON_H
#define SM_SINGLETON_H

template <class T>
class Singleton
{
    public:
        static T* getInstance()
        {
            if (instance == NULL)
                instance = new T;

            return instance;
        }

    private:
        static T* instance;
};

template <class T> T* Singleton<T>::instance = NULL;

#endif
