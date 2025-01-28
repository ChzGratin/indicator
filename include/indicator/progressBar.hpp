#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <indicator/utility.hpp>

namespace indicator
{
    class ProgressBar
    {
        private:
        char bracket_l, fill, remainder, bracket_r;
        int width; // width = (# of fill) + (# of remainder)
        float min, max, value;

        // flag definition
        const int8_t FLAG_PLAY = (0b1 << 0); // thrd is running
        const int8_t FLAG_STOP = (0b1 << 1); // stop() is called
        const int8_t FLAG_DRAW = (0b1 << 2); // play() or setValue() is called
        int8_t flag = 0;

        std::mutex m;
        std::condition_variable cv;
        std::thread thrd;

        private:
        void update(); // print progress bar. run on child thread

        public:
        ProgressBar()
        : bracket_l('['), fill('#'), remainder('-'), bracket_r(']'), width(31),
        min(0), max(1), value(0) {};

        ProgressBar(char _bracket_l, char _fill, char _remainder, char _bracket_r, int _width,
        float _min, float _max)
        : bracket_l(_bracket_l), fill(_fill), remainder(_remainder), bracket_r(_bracket_r), width(_width),
        min(_min), max(_max), value(_min) {};

        ~ProgressBar() { stop(); };

        // setter
        void setValue(float);

        void play(); // play progress bar
        void stop(); // stop progress bar
    };

    void ProgressBar::update()
    {
        float progress;
        int fill_count;

        while(1)
        {
            // wait for event (cv.notify_one())
            std::unique_lock<std::mutex> lck(m);
            cv.wait(lck, [this]{ return flag; });

            // handle event
            if(flag & FLAG_STOP) { break; }
            if(~flag & FLAG_DRAW)
            {
                flag = FLAG_PLAY;
                continue;
            }

            // calculate progress
            progress = (value - min) / (max - min);
            fill_count = width * progress;

            // print progress bar
            putchar(bracket_l);
            for(int i=0; i<width; i++)
            {
                if(i < fill_count) { putchar(fill); }
                else { putchar(remainder); }
            }
            printf("%c %3d%%", bracket_r, int((value - min) / (max - min) * 100));

            // move cursor to bracket_l
            // [#####----]  50%_
            // 1  width  234567^
            for(int i=0; i<width+7; i++) { putchar('\b'); }
        }

        // terminate thread
        putchar('\n');
        flag = 0;
    }

    void ProgressBar::setValue(float newValue)
    {
        value = std::clamp<float>(newValue, min, max);

        if(flag & FLAG_PLAY)
        {
            flag |= FLAG_DRAW;
            cv.notify_one();
        }
    }

    void ProgressBar::play()
    {
        if(flag & FLAG_PLAY) { return; }

        flag = FLAG_PLAY | FLAG_DRAW;
        thrd = std::thread(&ProgressBar::update, this);
        cv.notify_one();
    }

    void ProgressBar::stop()
    {
        if(~flag & FLAG_PLAY) { return; }

        // wait until progress bar is printed
        m.lock();
        flag = FLAG_STOP;
        m.unlock();

        cv.notify_one();
        thrd.join();
    }
}

#endif