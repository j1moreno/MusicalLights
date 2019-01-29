#define AVERAGE_COUNT_BUFFER_SIZE 3

class MusicalMeter {

    public:
        MusicalMeter(uint8_t numberOfLevels, uint8_t startingPin);
        uint8_t GetNumberOfLevels();
        void CycleLevels(uint32_t delay_between_levels);
        void DisplayAdaptive(uint16_t audio_level);
        void Display(uint16_t audio_level);
        void SetZeroLevel(uint16_t zero_level);

    private:
        uint16_t zero_level_;
        uint8_t *led_pins_;
        uint8_t number_of_levels_;
        uint16_t current_max_;
        uint16_t average_buffer_[AVERAGE_COUNT_BUFFER_SIZE];
        uint8_t average_index_;
        void WriteToBuffer(uint16_t level);
        uint16_t GetBufferAverage();
        void DisplayAudioLevelBasic(uint16_t audio_level, uint16_t max_value);
};