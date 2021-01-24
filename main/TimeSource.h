#ifndef BA229F1B_9FE4_4847_AE8C_D6FB92481527
#define BA229F1B_9FE4_4847_AE8C_D6FB92481527

#include <chrono>

typedef std::chrono::milliseconds Timestamp;

class TimeSource{
public:

    static Timestamp& now() {
        static Timestamp t = std::chrono::milliseconds(0);

        t = std::chrono::duration_cast<std::chrono::milliseconds> (
                        std::chrono::system_clock::now().time_since_epoch()
                    );

        return t;
    }
};



#endif /* BA229F1B_9FE4_4847_AE8C_D6FB92481527 */
