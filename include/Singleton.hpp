#ifndef EPIC_SINGLETON_HPP
#define EPIC_SINGLETON_HPP

namespace Epic
{
    namespace Pattern
    {
        // Meyers Singleton
        template <class T>
        class Singleton
        {
            public:
                static T & instance()
                {
                    static T m_instance;
                    return m_instance;
                }

            private:
                Singleton();
                Singleton(const Singleton&);
                Singleton& operator = (const Singleton&);
                ~Singleton();
        };
    } // Epic::Pattern
} // Epic

#endif /*ndef EPIC_SINGLETON_HPP */
