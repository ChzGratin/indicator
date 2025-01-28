#ifndef SPINNER_HPP
#define SPINNER_HPP

#include <vector>
#include <indicator/utility.hpp>

namespace indicator
{
    class Spinner
    {
        private:
        std::vector<char> anim; // spinner animation
        int intv_ms; // frame interval [ms]
        
        bool isPlaying = false;
        std::thread thrd;

        private:
        void update(); // print spinner animation. run on child thread

        public:
        Spinner()
        : anim({'-','\\','|','/'}), intv_ms(250) {};

        Spinner(std::vector<char> spinnerAnimation, int frameInterval_ms)
        : anim(spinnerAnimation), intv_ms(frameInterval_ms) {};
        
        void play(); // play spinner animation
        void stop(); // stop spinner animation
    };

    void Spinner::update()
    {
        // length of animation must be greater than 0
        if(anim.size() <= 0) { return; }

        std::vector<char>::iterator iter; // loop variable
        for(iter = anim.begin(); isPlaying; iter++)
        {
            // prevent out of range error
            if(iter == anim.end()) { iter = anim.begin(); }

            // print current frame
            printf("%c", *iter);
            delay_ms(intv_ms);
            putchar('\b');
        }
    }

    void Spinner::play()
    {
        if(isPlaying) { return; }

        isPlaying = true;
        thrd = std::thread(&Spinner::update, this); // copy elision & thread with member function
    }

    void Spinner::stop()
    {
        if(!isPlaying) { return; }

        isPlaying = false;
        thrd.join();
    }
}

#endif