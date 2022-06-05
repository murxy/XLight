#if !defined(__SOFTWARE__)
#define __SOFTWARE__

#include "LightShow/LightShow.h"

namespace murxy {

class Software {
    using Handler = void(Software::*)();
public:

    void initialize();
    void loop();

private:
    enum States {
        Loading,
        Ready,
        Count
    };

    States mCurrentState = States::Loading;
    LightShow mLightShow;
    uint32_t mTimer = 0;

    void onLoading();
    void onReady();

    static const Handler mStateHandlers[States::Count];

    void setState(States newState);
};

} // namespace murxy

#endif
