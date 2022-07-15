#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

class NonCopyable
{
    protected:
        NonCopyable() {}
        ~NonCopyable() {}
    
    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator=(const NonCopyable&);

};

#endif