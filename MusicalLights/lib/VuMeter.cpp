#include <vector>

class VuMeter {

    public:
        VuMeter(int numberOfLevels, int startingPin);

    private:
        std::vector<int> led_pins_;

};

VuMeter::VuMeter(int numberOfLevels, int startingPin) {
    led_pins_.reserve(numberOfLevels);
    
}