#ifndef COMPUTER_H_INCLUDED
#define COMPUTER_H_INCLUDED


class computer {
public:
    computer();
    ~computer();
    void setspeed(int p);
    int readspeed();
protected:
    int processorspeed;
};


#endif // COMPUTER_H_INCLUDED
