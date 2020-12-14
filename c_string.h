#define STRING_HEADER(byte) class String##byte : public String{private:char buffer[byte];public:String##byte();String##byte(const char *str);char* Chars();int Capacity();};

namespace c_string
{
    class String
    {

        public:
        void Print();
        void Set(const char *str);
        int Length();
        bool operator==(String& other);
        virtual char* Chars() = 0;
        virtual int Capacity() = 0;

    };

    STRING_HEADER(64)
    STRING_HEADER(4096)
}